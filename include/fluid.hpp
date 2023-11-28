#pragma once
#include <unordered_map>

#include "matrix.hpp"

namespace gds {

/// @brief 粒子类型
enum class ParticelType {
  O2,      // 氧气
  CO2,     // 二氧化碳
  CO,      // 一氧化碳
  O,       // 氧原子
  C,       // 碳原子
  e,       // 电子
  pO,      // 氧离子
  pO2,     // 氧气离子
  pCO2,    // 二氧化碳离子
  eO,      // 氧负离子
  eO2,     // 氧气负离子
  eO3,     // 臭氧负离子
  eCO2,    // 二氧化碳负离子
  eCO2V1,  // 二氧化碳负离子第一种电子亚能级
  eCO2V2,  // 二氧化碳负离子第二种电子亚能级
  eCO2V3,  // 二氧化碳负离子第三种电子亚能级
  eCO2V4   // 二氧化碳负离子第四种电子亚能级
};

class Fluid {
 public:
  std::unordered_map<ParticelType, Matrix> n;   // 粒子密度
  std::unordered_map<ParticelType, Matrix> ux;  // x 方向粒子迁移速度
  std::unordered_map<ParticelType, Matrix> uy;  // y 方向粒子迁移速度
  std::unordered_map<ParticelType, Matrix> Jx;  // x 方向粒子流密度
  std::unordered_map<ParticelType, Matrix> Jy;  // y 方向粒子流密度
  std::unordered_map<ParticelType, Matrix> Dx;  // x 方向粒子的扩散系数
  std::unordered_map<ParticelType, Matrix> Dy;  // y 方向粒子的扩散系数
  std::unordered_map<ParticelType, Matrix> S;   // 源项

  // 以下是各粒子的五点系数
  std::unordered_map<ParticelType, Matrix> AW;
  std::unordered_map<ParticelType, Matrix> AS;
  std::unordered_map<ParticelType, Matrix> AC;
  std::unordered_map<ParticelType, Matrix> AN;
  std::unordered_map<ParticelType, Matrix> AE;

  std::vector<Matrix> Kh;  // 各反应的反应速率系数
  std::vector<Matrix> Kr;  // 各反应的反应速率系数
  std::vector<Matrix> Ki;  // 各反应的反应速率系数
  std::vector<Matrix> Ei;  // 各反应的反应速率系数

 public:
  Fluid();
  ~Fluid();
};

}  // namespace gds
