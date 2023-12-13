#pragma once
#include <unordered_map>

#include "matrix.hpp"

namespace gds {

/// @brief 粒子类型
/// @details 用于标识粒子的类型，以便于在计算时使用
///          0~9 为中性粒子，10~19 为正离子，20~29 为负离子
enum class ParticelType {
  O2 = 0,   // 氧气
  CO2,      // 二氧化碳
  CO,       // 一氧化碳
  O,        // 氧原子
  C,        // 碳原子
  pO = 10,  // 氧离子
  pO2,      // 氧气离子
  pCO2,     // 二氧化碳离子
  eO = 20,  // 氧负离子
  eO2,      // 氧气负离子
  eO3,      // 臭氧负离子
  eCO2,     // 二氧化碳负离子
  eCO2V1,   // 二氧化碳负离子第一种电子亚能级
  eCO2V2,   // 二氧化碳负离子第二种电子亚能级
  eCO2V3,   // 二氧化碳负离子第三种电子亚能级
  eCO2V4,   // 二氧化碳负离子第四种电子亚能级
  e,        // 电子
};

class Fluid {
 public:
  std::unordered_map<ParticelType, Matrix> n;       // 粒子密度
  std::unordered_map<ParticelType, Matrix> ux;      // x 方向粒子迁移速度
  std::unordered_map<ParticelType, Matrix> uy;      // y 方向粒子迁移速度
  std::unordered_map<ParticelType, Matrix> Jx;      // x 方向粒子流密度
  std::unordered_map<ParticelType, Matrix> Jy;      // y 方向粒子流密度
  std::unordered_map<ParticelType, Matrix> Dx;      // x 方向粒子的扩散系数
  std::unordered_map<ParticelType, Matrix> Dy;      // y 方向粒子的扩散系数
  Matrix                                   Ix, Iy;  // x, y 方向的电流
  std::unordered_map<ParticelType, Matrix> Gamma;   // 各粒子的密度通量
  std::unordered_map<ParticelType, Matrix> S;       // 源项
  std::unordered_map<ParticelType, ESM>    v_surf;  // 各粒子介质层表面速度
  Matrix                                   Sener;   // 能量源项

  // 下面两个量只有正离子才有
  std::unordered_map<ParticelType, Matrix> Zx;  // 用于计算五点系数的耦合量
  std::unordered_map<ParticelType, Matrix> Zy;  // 用于计算五点系数的耦合量

  // 以下是各粒子的五点系数
  std::unordered_map<ParticelType, Matrix> AW;
  std::unordered_map<ParticelType, Matrix> AS;
  std::unordered_map<ParticelType, Matrix> AC;
  std::unordered_map<ParticelType, Matrix> AN;
  std::unordered_map<ParticelType, Matrix> AE;

  std::vector<Matrix> Kh;   // 各反应的反应速率系数
  std::vector<Matrix> Kr;   // 各反应的反应速率系数
  std::vector<Matrix> Ki;   // 各反应的反应速率系数
  std::vector<Matrix> Ei;   // 各反应的反应速率系数
  Matrix              Kte;  // 电子的初始能量密度

  Matrix phi;        // 电势
  Matrix E, Ex, Ey;  // 电场强度

  double dx, dy;  // 网格尺寸

 private:
  /// @brief 初始化边界条件
  void init();

 public:
  Fluid();
  ~Fluid() = default;
  friend class Solver;
};

}  // namespace gds
