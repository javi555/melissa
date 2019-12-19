#include <iostream>
#include "queen_bee_svc.h"
#include <klepsydra/core/yaml_environment.h>
#include "zmq_middleware_facility.h"

int main(int argc, char **argv)
{
  static const int EVENT_LOOP_SIZE = 16;

  std::string yaml_path = argv[1];

  kpsr::YamlEnvironment yamlEnv(yaml_path + "/qb_config.yaml");

  std::string imageTopic;
  int poolSize;

  yamlEnv.getPropertyString("image_topic", imageTopic);
  yamlEnv.getPropertyInt("pool_size", poolSize);

  std::string containerName = argc > 1 ? argv[1] : "QBApp";
  std::string roboBeePort = argc > 2 ? argv[2] : "9001";
  int adminPort = argc > 3 ? std::atoi(argv[3]) : 9090;
  int systemPort = argc > 4 ? std::atoi(argv[4]) : 9292;
  
  kpsr::high_performance::EventLoopMiddlewareProvider<EVENT_LOOP_SIZE> eventloop(nullptr);
  eventloop.start();

  std::string rbUrl = "tcp://*:" + roboBeePort;

  spdlog::info("rbURL: {}", rbUrl);
  zmq::context_t context(1);
  ZmqMiddlewareFacility zmf(context, rbUrl, imageTopic, eventloop, poolSize);

  mls::QueenBeeSvc queenBeeSvc(&yamlEnv, zmf.imageDataSubscriber, nullptr);

  queenBeeSvc.startup();
  spdlog::info("QBApp: QueenBee Service started");

  while (!queenBeeSvc._receivedImage)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
  queenBeeSvc.shutdown();

  zmf.stop(imageTopic);

  return 0;
} // end main()