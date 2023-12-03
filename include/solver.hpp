#pragma once
#include <memory>

#include "fluid.hpp"

namespace gds {

class Solver {
 private:
  std::unique_ptr<Fluid> fluid;

 public:
  Solver(/* args */);
  ~Solver();

 private:
  void Calculate_ue();
  void Calculate_De();
};

}  // namespace gds