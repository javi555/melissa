
#include "robo_bee_svc.h"
#include "gtest/gtest.h"
#include <stdio.h>
#include <thread>
#include <unistd.h>

#include <klepsydra/core/cache_listener.h>
#include <klepsydra/core/event_emitter_middleware_provider.h>
#include <klepsydra/core/event_emitter_publisher.h>
#include <klepsydra/core/event_emitter_subscriber.h>
#include <klepsydra/mem_core/mem_env.h>

TEST(RoboBeeSvc, TestCase1Waypoint) {

  kpsr::EventEmitterMiddlewareProvider<kpsr::vision_ocv::ImageData>
      imageDataProvider(nullptr, "ImageData", 0, nullptr, nullptr);

  kpsr::EventEmitterMiddlewareProvider<mls::Waypoint> waypointProvider(
      nullptr, "Waypoint", 0, nullptr, nullptr);

  kpsr::mem::MemEnv environment;

  int imgWidth = 320;
  int imgHeight = 240;

  mls::RoboBeeSvc roboBeeSvc(&environment, imageDataProvider.getPublisher(),
                             waypointProvider.getSubscriber(), imgWidth,
                             imgHeight, "/var/tmp/images", true, 1);

  mls::Waypoint testWaypoint(1234, 10, 10, 30);

  roboBeeSvc.startup();

  waypointProvider.getPublisher()->publish(testWaypoint);

  mls::Waypoint recvWaypoint = roboBeeSvc._waypoint;

  roboBeeSvc.shutdown();

  ASSERT_EQ(recvWaypoint.seq, testWaypoint.seq);
  ASSERT_EQ(recvWaypoint.x, testWaypoint.x);
  ASSERT_EQ(recvWaypoint.y, testWaypoint.y);
  ASSERT_EQ(recvWaypoint.z, testWaypoint.z);

  spdlog::info("TestCase2Waypoint completed.");
}