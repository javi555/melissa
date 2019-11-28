
#include "gtest/gtest.h"
#include <stdio.h>
#include <thread>
#include <unistd.h>
#include "robo_bee_svc.h"

TEST(RoboBeeSvc, TestCase1) {

  mls::RoboBeeSvc robobee(nullptr, nullptr, nullptr, 480, 240, "/var/tmp/images", false);

  spdlog::info("TestCase1 completed.");
}