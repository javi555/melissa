#include <iostream>
#include "queen_bee_svc.h"
#include <klepsydra/core/yaml_environment.h>
#include <klepsydra/socket_interface/hp_socket_admin_container_provider.h>
#include <klepsydra/socket_interface/hp_socket_system_container_provider.h>
#include "zmq_middleware_facility.h"

int main(int argc, char **argv)
{
  static const int EVENT_LOOP_SIZE = 16;

  std::string yamlFile = argv[1];
  kpsr::YamlEnvironment yamlEnv(yamlFile);

  std::string containerName = argc > 2 ? argv[2] : "QBApp";
  std::string roboBeePort = argc > 3 ? argv[3] : "9001";

  std::string imageTopic;
  int poolSize;
  std::string rbUrl = "tcp://*:" + roboBeePort;

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

  kpsr::high_performance::EventLoopMiddlewareProvider<EVENT_LOOP_SIZE> eventloop(nullptr);
  eventloop.start();

  spdlog::info("robobee_URL: {}", rbUrl);
  spdlog::info("image_topic: {}", imageTopic);
  spdlog::info("pool_size: {}", poolSize);
  spdlog::info("server_admin_port: {}", adminPort);
  spdlog::info("server_system_port {}", systemPort);

  zmq::context_t context(1);
  ZmqMiddlewareFacility zmf(context, rbUrl, imageTopic, eventloop, poolSize);
  mls::QueenBeeSvc queenBeeSvc(&yamlEnv, zmf._imageDataSubscriber, nullptr);

  queenBeeSvc.startup();
  spdlog::info("QBApp: QueenBee Service started");

  while (true)
  {
    queenBeeSvc.runOnce();
  }
  queenBeeSvc.shutdown();

  systemProvider.stop();
  adminProvider.stop();
  adminEventLoop.stop();

  zmf.stop();

  return 0;
} // end main()