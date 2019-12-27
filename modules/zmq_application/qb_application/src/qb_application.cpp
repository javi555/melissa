#include <iostream>
#include "queen_bee_svc.h"
#include <klepsydra/core/yaml_environment.h>
#include <klepsydra/socket_interface/hp_socket_admin_container_provider.h>
#include <klepsydra/socket_interface/hp_socket_system_container_provider.h>
#include <klepsydra/vision_ocv/image_data_factory.h>
#include "zmq_middleware_facility.h"
#include "config.h"

int main(int argc, char **argv)
{

  std::string yamlFile = argv[1];
  kpsr::YamlEnvironment yamlEnv(yamlFile);

  std::string containerName = argc > 2 ? argv[2] : "QBApp";
  std::string roboBeeImgPort = argc > 3 ? argv[3] : "9001";
  std::string roboBeeWpPort = argc > 4 ? argv[4] : "9002";

  std::string imageTopic;
  int poolSize;
  int imgWidth;
  int imgHeight;
  std::string rbImgUrl = "tcp://localhost:" + roboBeeImgPort;
  std::string rbWpUrl = "tcp://localhost:" + roboBeeWpPort;

  yamlEnv.getPropertyString("image_topic", imageTopic);
  yamlEnv.getPropertyInt("img_height", imgHeight);
  yamlEnv.getPropertyInt("img_width", imgWidth);
  yamlEnv.getPropertyInt("pool_size", poolSize);

  kpsr::high_performance::EventLoopMiddlewareProvider<128> adminEventLoop(nullptr);
  int adminPort;
  yamlEnv.getPropertyInt("server_admin_port", adminPort);
  kpsr::admin::socket_mdlw::EventLoopSocketAdminContainerProvider<128> adminProvider(adminPort,
                                                                                     adminEventLoop,
                                                                                     &yamlEnv,
                                                                                     "QBApp_admin");

  kpsr::high_performance::EventLoopMiddlewareProvider<128> systemEventLoop(nullptr);
  int systemPort;
  yamlEnv.getPropertyInt("server_system_port", systemPort);
  kpsr::admin::socket_mdlw::EventLoopSocketSystemContainerProvider<128> systemProvider(systemPort,
                                                                                    systemEventLoop,
                                                                                    &yamlEnv,
                                                                                    "QBApp_system");
  adminEventLoop.start();
  adminProvider.start();
  systemProvider.start();

  kpsr::high_performance::EventLoopMiddlewareProvider<EVENT_LOOP_SIZE> eventloop(&adminProvider.getContainer());
  eventloop.start();

  spdlog::info("EventLoopSize: {}", EVENT_LOOP_SIZE);

  spdlog::info("robobee_URL: {}", rbImgUrl);
  spdlog::info("image_topic: {}", imageTopic);
  spdlog::info("pool_size: {}", poolSize);
  spdlog::info("server_admin_port: {}", adminPort);
  spdlog::info("server_system_port {}", systemPort);

  kpsr::vision_ocv::ImageDataFactory _factory(imgWidth, imgHeight, 0, "frame");

  kpsr::Publisher<mls::Waypoint> *waypointPublisher =
      eventloop.getPublisher<mls::Waypoint>("Waypoint", 0, nullptr, nullptr);

  zmq::context_t context(1);
  ZmqMiddlewareFacility zmf(context, rbImgUrl, imageTopic, eventloop, poolSize, _factory);
  mls::QueenBeeSvc queenBeeSvc(&yamlEnv, zmf._imageDataSubscriber, waypointPublisher);

  queenBeeSvc.startup();
  spdlog::info("QBApp: QueenBee Service started");

  while (true)
  {
    queenBeeSvc.runOnce();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
  queenBeeSvc.shutdown();

  systemProvider.stop();
  adminProvider.stop();
  adminEventLoop.stop();

  zmf.stop();

  return 0;
} // end main()