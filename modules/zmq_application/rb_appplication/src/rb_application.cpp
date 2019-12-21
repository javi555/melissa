#include <iostream>
#include "robo_bee_svc.h"
#include <klepsydra/core/yaml_environment.h>
#include <klepsydra/socket_interface/hp_socket_admin_container_provider.h>
#include <klepsydra/socket_interface/hp_socket_system_container_provider.h>
#include "zmq_middleware_facility.h"

int main(int argc, char **argv)
{
  std::string yamlFile = argv[1];
  kpsr::YamlEnvironment yamlEnv(yamlFile);

  std::string containerName = argc > 2 ? argv[2] : "RBApp";
  std::string queenBeePort = argc > 3 ? argv[3] : "9001";

  int imgWidth;
  int imgHeight;
  int roboBeePrefix;
  std::string imageTopic;
  int poolSize;
  std::string qbUrl = "tcp://*:" + queenBeePort;

  yamlEnv.getPropertyInt("img_width", imgWidth);
  yamlEnv.getPropertyInt("img_height", imgHeight);
  yamlEnv.getPropertyInt("robo_bee_prefix", roboBeePrefix);
  yamlEnv.getPropertyString("image_topic", imageTopic);
  yamlEnv.getPropertyInt("pool_size", poolSize);

  kpsr::high_performance::EventLoopMiddlewareProvider<128> adminEventLoop(nullptr);
  int adminPort;
  yamlEnv.getPropertyInt("server_admin_port", adminPort);
  kpsr::admin::socket_mdlw::EventLoopSocketAdminContainerProvider<128> adminProvider(adminPort,
                                                                                     adminEventLoop,
                                                                                     &yamlEnv,
                                                                                     "admin_server");

  kpsr::high_performance::EventLoopMiddlewareProvider<128> systemEventLoop(nullptr);
  int systemPort;
  yamlEnv.getPropertyInt("server_system_port", systemPort);
  kpsr::admin::socket_mdlw::EventLoopSocketSystemContainerProvider<128> systemProvider(systemPort,
                                                                                    systemEventLoop,
                                                                                    &yamlEnv,
                                                                                    "system_server");
  adminEventLoop.start();
  adminProvider.start();
  systemProvider.start();

  spdlog::info("queenbee_URL: {}", qbUrl);
  spdlog::info("RBApp: width: {}", imgWidth);
  spdlog::info("RBApp: height: {}", imgHeight);
  spdlog::info("RBApp: robo_bee_prefix: {}", roboBeePrefix);
  spdlog::info("image_topic: {}", imageTopic);
  spdlog::info("pool_size: {}", poolSize);
  spdlog::info("server_admin_port: {}", adminPort);
  spdlog::info("server_system_port {}", systemPort);

  zmq::context_t context(1);
  zmq::socket_t publisher(context, ZMQ_PUB);
  publisher.bind(qbUrl);

  kpsr::zmq_mdlw::ToZMQMiddlewareProvider toZMQMiddlewareProvider(nullptr, publisher);

  kpsr::Publisher<kpsr::vision_ocv::ImageData> *toZMQPublisher = toZMQMiddlewareProvider
                                                                     .getBinaryToMiddlewareChannel<kpsr::vision_ocv::ImageData>(imageTopic, 10);

  mls::RoboBeeSvc roboBeeSvc(&yamlEnv, toZMQPublisher, nullptr,
                             imgWidth, imgHeight, "/var/tmp/images", true, roboBeePrefix);

  roboBeeSvc.startup();
  spdlog::info("RBApp: RoboBee Service started");

 while (true)
  {
    roboBeeSvc.runOnce();
  }

  roboBeeSvc.shutdown();

  return 0;
} // end main()