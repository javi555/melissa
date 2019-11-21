
#include "queen_bee_svc.h"
#include "robo_bee_svc.h"
#include <iostream>
#include <klepsydra/core/publisher.h>
#include <klepsydra/core/subscriber.h>
#include <klepsydra/high_performance/event_loop_middleware_provider.h>
#include <klepsydra/mem_core/mem_env.h>

int main(int argc, char **argv) {

  int imgWidth = 480;
  int imgHeight = 240;
  
  // 1.- Env
  kpsr::mem::MemEnv environment;

  // 2.- Create EventLoop

  kpsr::high_performance::EventLoopMiddlewareProvider<2> eventloop(nullptr);

  eventloop.getSubscriber<kpsr::vision_ocv::ImageData>("test")->registerListener(
      "ImageData", [](const kpsr::vision_ocv::ImageData &image) {
        std::cout << "Image received" << std::endl;
      });

  eventloop.start();

  // 3.- Publishers and subscribers

  kpsr::Subscriber<kpsr::vision_ocv::ImageData> *imageDataSubscriber =
      eventloop.getSubscriber<kpsr::vision_ocv::ImageData>("ImageData");
  kpsr::Publisher<kpsr::vision_ocv::ImageData> *imageDataPublisher =
      eventloop.getPublisher<kpsr::vision_ocv::ImageData>("ImageData", 0,
                                                          nullptr, nullptr);

  // 4.- Launch services

  mls::RoboBeeSvc roboBeeSvc(nullptr, imageDataPublisher, imgWidth, imgHeight);
  mls::QueenBeeSvc queenBeeSvc(nullptr, imageDataSubscriber);

  roboBeeSvc.startup();
  std::cout << "RoboBee Service started" << std::endl;
  queenBeeSvc.startup();
  std::cout << "QueenBee Service started" << std::endl;

  eventloop.getSubscriber<kpsr::vision_ocv::ImageData>("test")->removeListener(
      "ImageData");
  eventloop.stop();

  return 0;
} // end main()