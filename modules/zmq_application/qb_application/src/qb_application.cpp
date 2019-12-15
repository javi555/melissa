#include <iostream>
#include "queen_bee_svc.h"
#include <klepsydra/core/yaml_environment.h>
#include <klepsydra/high_performance/event_loop_middleware_provider.h>

#include <klepsydra/zmq_core/zhelpers.hpp>
#include <klepsydra/zmq_core/from_zmq_middleware_provider.h>
#include <klepsydra/zmq_core/to_zmq_middleware_provider.h>
#include <klepsydra/vision_ocv/image_data_factory.h>

#include <klepsydra/serialization/binary_cereal_mapper.h>
#include <klepsydra/serialization/json_cereal_mapper.h>
#include <klepsydra/zmq_vision_ocv/image_event_data_zmq_mapper.h>
int main(int argc, char **argv)
{

  std::string yaml_path = "/home/javi/projects/melissa/modules/config/yaml";

  std::string containerName = argc > 1 ? argv[1] : "QBApp";
  std::string roboBeePort = argc > 2 ? argv[2] : "9001";
  int adminPort = argc > 3 ? std::atoi(argv[3]) : 9090;
  int systemPort = argc > 4 ? std::atoi(argv[4]) : 9292;
  
  // ZMQ
  std::string rbUrl = "tcp://*:" + roboBeePort;
  std::string topic = "image_data";

  spdlog::info("rbURL: {}", rbUrl);

  //  Prepare our context and subscriber
  zmq::context_t context(1);

  zmq::socket_t subscriber(context, ZMQ_SUB);

  subscriber.connect(rbUrl);
  subscriber.setsockopt(ZMQ_SUBSCRIBE, topic.c_str(), topic.size());

  kpsr::zmq_mdlw::FromZmqMiddlewareProvider _fromZmqMiddlewareProvider;
  kpsr::zmq_mdlw::FromZmqChannel<Base> *_binaryFromZMQProvider = _fromZmqMiddlewareProvider
                                                                     .getBinaryFromMiddlewareChannel<kpsr::vision_ocv::ImageData>(subscriber, 10);

  kpsr::vision_ocv::ImageDataFactory factory(320, 240, 10, "body");

  _binaryFromZMQProvider->start();

  kpsr::high_performance::EventLoopMiddlewareProvider<16> eventloop(nullptr);
  eventloop.start();

  kpsr::Subscriber<kpsr::vision_ocv::ImageData> *imageDataSubscriber =
      eventloop.getSubscriber<kpsr::vision_ocv::ImageData>(topic);
  kpsr::Publisher<kpsr::vision_ocv::ImageData> *imageDataPublisher =
      eventloop.getPublisher<kpsr::vision_ocv::ImageData>(topic, 10, factory.initializerFunction, nullptr);

  _binaryFromZMQProvider->registerToTopic(topic, imageDataPublisher);

  mls::QueenBeeSvc queenBeeSvc(nullptr, imageDataSubscriber, nullptr);

  queenBeeSvc.startup();
  spdlog::info("QBApp: QueenBee Service started");

  while (!queenBeeSvc._receivedImage)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
  queenBeeSvc.shutdown();

  _binaryFromZMQProvider->stop();
  _binaryFromZMQProvider->unregisterFromTopic(topic);

  return 0;
} // end main()