#include <iostream>
#include "robo_bee_svc.h"
#include <klepsydra/core/yaml_environment.h>
#include <klepsydra/high_performance/event_loop_middleware_provider.h>

#include <klepsydra/zmq_core/zhelpers.hpp>
#include <klepsydra/zmq_core/from_zmq_middleware_provider.h>
#include <klepsydra/zmq_core/to_zmq_middleware_provider.h>

#include <klepsydra/serialization/binary_cereal_mapper.h>
#include <klepsydra/serialization/json_cereal_mapper.h>

#include <klepsydra/zmq_vision_ocv/image_event_data_zmq_mapper.h>

int main(int argc, char **argv)
{

  std::string yaml_path = argv[1];

  kpsr::YamlEnvironment yamlEnv(yaml_path + "/rb_config.yaml");

  std::string containerName = argc > 2 ? argv[2] : "RBApp";
  std::string queenBeePort = argc > 3 ? argv[3] : "9001";
  int adminPort = argc > 4 ? std::atoi(argv[4]) : 9090;
  int systemPort = argc > 5 ? std::atoi(argv[5]) : 9292;

  float imgWidth;
  float imgHeight;
  int roboBeePrefix;

  yamlEnv.getPropertyFloat("img_width", imgWidth);
  yamlEnv.getPropertyFloat("img_height", imgHeight);
  yamlEnv.getPropertyInt("robo_bee_prefix", roboBeePrefix);

  spdlog::info("RBApp: width: {:03.0f}", imgWidth);
  spdlog::info("RBApp: height: {:03.0f}", imgHeight);
  spdlog::info("RBApp: robo_bee_prefix: {}", roboBeePrefix);

  // ZMQ socket
  std::string qbUrl = "tcp://*:" + queenBeePort;
  std::string topic = "image_data";

  spdlog::info("qbURL: {}", qbUrl);

  //  Prepare context and publisher
  zmq::context_t context(1);
  zmq::socket_t publisher(context, ZMQ_PUB);
  publisher.bind(qbUrl);

  kpsr::zmq_mdlw::ToZMQMiddlewareProvider toZMQMiddlewareProvider(nullptr, publisher);

  kpsr::Publisher<kpsr::vision_ocv::ImageData> *toZMQPublisher = toZMQMiddlewareProvider
                                                                     .getBinaryToMiddlewareChannel<kpsr::vision_ocv::ImageData>(topic, 10);

  mls::RoboBeeSvc roboBeeSvc(&yamlEnv, toZMQPublisher, nullptr,
                             imgWidth, imgHeight, "/var/tmp/images", true, roboBeePrefix);

  roboBeeSvc.startup();
  spdlog::info("RBApp: RoboBee Service started");

  roboBeeSvc.runOnce();

  roboBeeSvc.shutdown();

  return 0;
} // end main()