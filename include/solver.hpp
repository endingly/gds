#pragma once
#include <memory>

#include "fluid.hpp"

namespace gds {

class Solver {
 private:
  std::unique_ptr<Fluid> fluid;

 public:
  Solver();
  Solver(std::unique_ptr<Fluid> fluid);
  ~Solver();

 private:
  /// @brief 计算电子迁移速度
  void Calculate_ue();
  /// @brief 计算电子扩散系数
  void Calculate_De();
  /// @brief 计算泊松方程
  void Calculate_PossionEquation();
};

}  // namespace gds