
#include <stdio.h>
#include <thread>
#include <unistd.h>
#include "gtest/gtest.h"
#include "robo_bee.h"

TEST(RoboBee, TestCase1) {

  mls::RoboBee robobee("robobee1");
  robobee.startCapturing();

  ASSERT_TRUE(robobee.isCapturing());
  std::cout << "Test completed." << std::endl;
}