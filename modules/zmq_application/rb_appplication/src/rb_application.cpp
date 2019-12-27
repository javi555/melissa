#include <iostream>
#include "robo_bee_svc.h"
#include <klepsydra/core/yaml_environment.h>
#include <klepsydra/socket_interface/hp_socket_admin_container_provider.h>
#include <klepsydra/socket_interface/hp_socket_system_container_provider.h>
#include "zmq_middleware_facility.h"
#include "config.h"

int main(int argc, char **argv)
{
  std::string yamlFile = argv[1];
  kpsr::YamlEnvironment yamlEnv(yamlFile);

  std::string containerName = argc > 2 ? argv[2] : "RBApp";
  std::string queenBeeImgPort = argc > 3 ? argv[3] : "9001";
  std::string queenBeeWpPort = argc > 4 ? argv[4] : "9002";

  int imgWidth;
  int imgHeight;
  int roboBeePrefix;
  std::string imageTopic;
  int poolSize;
  std::string qbImgUrl = "tcp://*:" + queenBeeImgPort;

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
                                                                                     "RBApp_admin");

  kpsr::high_performance::EventLoopMiddlewareProvider<128> systemEventLoop(nullptr);
  int systemPort;
  yamlEnv.getPropertyInt("server_system_port", systemPort);
  kpsr::admin::socket_mdlw::EventLoopSocketSystemContainerProvider<128> systemProvider(systemPort,
                                                                                    systemEventLoop,
                                                                                    &yamlEnv,
                                                                                    "RBApp_system");
  adminEventLoop.start();
  adminProvider.start();
  systemProvider.start();


  kpsr::high_performance::EventLoopMiddlewareProvider<EVENT_LOOP_SIZE> eventloop(&adminProvider.getContainer());
  eventloop.start();

  spdlog::info("queenbee_URL: {}", qbImgUrl);
  spdlog::info("RBApp: width: {}", imgWidth);
  spdlog::info("RBApp: height: {}", imgHeight);
  spdlog::info("RBApp: robo_bee_prefix: {}", roboBeePrefix);
  spdlog::info("image_topic: {}", imageTopic);
  spdlog::info("pool_size: {}", poolSize);
  spdlog::info("server_admin_port: {}", adminPort);
  spdlog::info("server_system_port {}", systemPort);

  zmq::context_t context(1);
  zmq::socket_t publisher(context, ZMQ_PUB);
  publisher.bind(qbImgUrl);

  kpsr::zmq_mdlw::ToZMQMiddlewareProvider toZMQMiddlewareProvider(nullptr, publisher);

  kpsr::Publisher<kpsr::vision_ocv::ImageData> *toZMQPublisher = toZMQMiddlewareProvider
                                                                     .getBinaryToMiddlewareChannel<kpsr::vision_ocv::ImageData>(imageTopic, 10);

  mls::RoboBeeSvc roboBeeSvc(&yamlEnv, toZMQPublisher, eventloop.getSubscriber<mls::Waypoint>("wpSubs"),
                             imgWidth, imgHeight, "/var/tmp/images", true, roboBeePrefix);

  roboBeeSvc.startup();
  spdlog::info("RBApp: RoboBee Service started");

 while (true)
  {
    roboBeeSvc.runOnce();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
  }

  roboBeeSvc.shutdown();

  return 0;
} // end main()