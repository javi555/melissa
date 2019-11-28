
#include "queen_bee_svc.h"
#include "robo_bee_svc.h"
#include "yaml-cpp/yaml.h"
#include <iostream>
#include <klepsydra/core/publisher.h>
#include <klepsydra/core/subscriber.h>
#include <klepsydra/core/yaml_environment.h>
#include <klepsydra/high_performance/event_loop_middleware_provider.h>

int main(int argc, char **argv) {

  int iters = std::stoi(argv[1]);

  // 1.- Load config

  std::string yaml_path =
      "/home/javi/projects/melissa/modules/config/yaml/image.yaml"; // TODO: put
                                                                    // this in a
                                                                    // env var
  kpsr::YamlEnvironment yamlEnv(yaml_path);

  float imgWidth;
  float imgHeight;

  yamlEnv.getPropertyFloat("img_width", imgWidth);
  yamlEnv.getPropertyFloat("img_height", imgHeight);

  spdlog::info("MemApp: loaded width: {:03.0f}", imgWidth);
  spdlog::info("MemApp: loaded height: {:03.0f}", imgHeight);

  // 2.- Create EventLoop

  kpsr::high_performance::EventLoopMiddlewareProvider<16> eventloop(nullptr);

  eventloop.start();

  // 3.- Publishers and subscribers

  kpsr::Subscriber<kpsr::vision_ocv::ImageData> *imageDataSubscriber =
      eventloop.getSubscriber<kpsr::vision_ocv::ImageData>("ImageData");
  kpsr::Publisher<kpsr::vision_ocv::ImageData> *imageDataPublisher =
      eventloop.getPublisher<kpsr::vision_ocv::ImageData>("ImageData", 0,
                                                          nullptr, nullptr);

  kpsr::Subscriber<mls::Waypoint> *waypointSubscriber =
      eventloop.getSubscriber<mls::Waypoint>("Waypoint");
  kpsr::Publisher<mls::Waypoint> *waypointPublisher =
      eventloop.getPublisher<mls::Waypoint>("Waypoint", 0, nullptr, nullptr);

  // 4.- Launch services

  mls::RoboBeeSvc roboBeeSvc(nullptr, imageDataPublisher, waypointSubscriber,
                             imgWidth, imgHeight, "/var/tmp/images", true);
  mls::QueenBeeSvc queenBeeSvc(nullptr, imageDataSubscriber, waypointPublisher);

  roboBeeSvc.startup();
  spdlog::info("MemApp: RoboBee Service started");
  queenBeeSvc.startup();
  spdlog::info("MemApp: QueenBee Service started");

  for (int i=0;i<iters;i++) {
    roboBeeSvc.runOnce();
    queenBeeSvc.runOnce();
  }

  eventloop.getSubscriber<kpsr::vision_ocv::ImageData>("img")->removeListener(
      "ImageData");
  eventloop.getSubscriber<mls::Waypoint>("wp")->removeListener("Waypoint");
  eventloop.stop();

  return 0;
} // end main()