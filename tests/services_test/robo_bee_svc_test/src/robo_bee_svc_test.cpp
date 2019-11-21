
#include "gtest/gtest.h"
#include <stdio.h>
#include <thread>
#include <unistd.h>
#include "robo_bee_svc.h"

TEST(RoboBeeSvc, TestCase1) {

  mls::RoboBeeSvc robobee(nullptr, nullptr, 480, 240);
  //robobee.startCapturing();

  //ASSERT_TRUE(robobee.isCapturing());
  std::cout << "TestCase1 completed." << std::endl;
}