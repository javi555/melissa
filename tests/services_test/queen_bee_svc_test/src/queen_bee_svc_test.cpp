
#include "queen_bee_svc.h"
#include "gtest/gtest.h"
#include <stdio.h>
#include <thread>
#include <unistd.h>

#include <klepsydra/core/cache_listener.h>
#include <klepsydra/core/event_emitter_middleware_provider.h>
#include <klepsydra/core/event_emitter_publisher.h>
#include <klepsydra/core/event_emitter_subscriber.h>
#include <klepsydra/mem_core/mem_env.h>

TEST(QueenBeeSvc, TestCase1Image) {

  kpsr::EventEmitterMiddlewareProvider<kpsr::vision_ocv::ImageData>
      imageDataProvider(nullptr, "ImageData", 0, nullptr, nullptr);

  kpsr::EventEmitterMiddlewareProvider<mls::Waypoint> waypointProvider(
      nullptr, "Waypoint", 0, nullptr, nullptr);

  kpsr::mem::MemEnv environment;

  float imgWidth = 320;
  float imgHeight = 240;

  mls::QueenBeeSvc queenBeeSvc(&environment, imageDataProvider.getSubscriber(),
                               waypointProvider.getPublisher());

  std::string image_path = "/home/javi/projects/melissa/tests/data/dillenia.jpg";
  kpsr::vision_ocv::ImageData testImg;
  testImg.img = cv::imread(image_path);
  ;
  testImg.seq = 1234;
  testImg.frameId = "testFrame";

  queenBeeSvc.startup();

  imageDataProvider.getPublisher()->publish(testImg);
  imageDataProvider.getPublisher()->publish(testImg);

  queenBeeSvc.shutdown();

  ASSERT_NE(queenBeeSvc._circles.size(), 0);

  spdlog::info("TestCase1Image completed.");
}
