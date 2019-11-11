
#include <stdio.h>
#include <thread>
#include <unistd.h>
#include "gtest/gtest.h"
#include "queen_bee.h"

TEST(QueenBee, TestCase1) {

  mls::QueenBee queenbee("queenbee1");

  //queenbee.method();

  //ASSERT_TRUE(queenbee.method());
  std::cout << "Test completed." << std::endl;
}