#include <gtest/gtest.h>

#include <fstream>
#include <nlohmann/json.hpp>

TEST(reaction_manager, write_info) {
  std::ofstream  ofs("reaction_info.json");
  nlohmann::json j;
  j["reaction_type"]       = "elastic";
  j["reaction_cofficient"] = 1.0e-10;
  ofs << j.dump(4) << std::endl;
  ofs.close();
}