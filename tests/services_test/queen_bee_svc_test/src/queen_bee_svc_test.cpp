
#include "gtest/gtest.h"
#include <stdio.h>
#include <thread>
#include <unistd.h>
#include "queen_bee_svc.h"

TEST(QueenBeeSvc, TestCase1) {

  mls::QueenBeeSvc queenbee(nullptr, nullptr, nullptr);

  spdlog::info("TestCase1 completed.");
}