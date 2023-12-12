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

  /// @brief 计算域整体时间步前进一个单位
  void Advance();

 private:
  /// @brief 计算电子迁移速度
  void Calculate_ue();
  /// @brief 计算电子扩散系数
  void Calculate_De();
  /// @brief 计算泊松方程
  void Calculate_PossionEquation();
  /// @brief 计算电场强度
  void Calculate_E();
  /// @brief 计算各粒子介质层表面速度
  void Calculate_v_surf();
};

}  // namespace gds