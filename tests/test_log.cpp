#include <gtest/gtest.h>

#include "log.hpp"
using namespace gds::utils;

TEST(Log, print) {
  Logger::Info("Hello, World!");
  Logger::Info("Hello, World!");
  Logger::Info("Hello, World!");
}

TEST(Log, print2file) {
  Logger::is_log_to_file_ = true;
  Logger::Info("Hello, World!");
  Logger::Info("Hello, World!");
  Logger::Info("Hello, World!");
}