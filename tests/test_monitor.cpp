#include <gtest/gtest.h>

#include "monitor.hpp"

TEST(Monitor, PrintConfig) { gds::utils::Monitor::PrintConfig(); }
TEST(Monitor, PrintInfo) { gds::utils::Monitor::PrintInfo(); }