#include "fluid.hpp"

#include "config.hpp"
using namespace gds::config;

constexpr double init_density = 1.0E5;

gds::Fluid::Fluid() {
  // 这里从氧气开始，因为氧气是第一个粒子，其值为0
  // 将所有的粒子参量初始化
  for (ParticelType p = ParticelType::O2; p <= ParticelType::C; p = ParticelType(int(p) + 1)) {
    // 初始化粒子密度，这里是因为 CO2 占主导地位，所以将其密度要高
    if (p == ParticelType::pCO2)
      n[p] = Matrix::Constant(x_cells, y_cells, init_MN);
    else
      n[p] = Matrix::Constant(x_cells, y_cells, init_density);
    // 初始化粒子迁移速度
    ux[p] = Matrix::Zero(x_cells, y_cells);
    uy[p] = Matrix::Zero(x_cells, y_cells);
    // 初始化粒子流密度
    Jx[p] = Matrix::Zero(x_cells, y_cells);
    Jy[p] = Matrix::Zero(x_cells, y_cells);
    // 初始化粒子的扩散系数
    Dx[p] = Matrix::Zero(x_cells, y_cells);
    Dy[p] = Matrix::Zero(x_cells, y_cells);
    // 初始化源项
    S[p] = Matrix::Zero(x_cells, y_cells);
    // 初始化五点系数
    AW[p] = Matrix::Zero(x_cells, y_cells);
    AS[p] = Matrix::Zero(x_cells, y_cells);
    AC[p] = Matrix::Zero(x_cells, y_cells);
    AN[p] = Matrix::Zero(x_cells, y_cells);
    AE[p] = Matrix::Zero(x_cells, y_cells);
  }
  // 初始化电流
  Ix = Matrix::Zero(x_cells, y_cells);
  Iy = Matrix::Zero(x_cells, y_cells);
  // 初始化能量源项
  Sener = Matrix::Zero(x_cells, y_cells);
  // 初始化电势，这里电极上还未加电压，所以为 0
  phi = Matrix::Zero(x_cells, y_cells);
  // 初始化电场强度
  E  = Matrix::Zero(x_cells, y_cells);
  Ex = Matrix::Zero(x_cells, y_cells);
  Ey = Matrix::Zero(x_cells, y_cells);
  // 初始化各反应的反应速率系数
  for (int i = 0; i < 100; i++) {
    Kh.push_back(Matrix::Zero(x_cells, y_cells));
    Kr.push_back(Matrix::Zero(x_cells, y_cells));
    Ki.push_back(Matrix::Zero(x_cells, y_cells));
    Ei.push_back(Matrix::Zero(x_cells, y_cells));
  }
  // 初始化电子的初始能量密度
  Kte = Matrix::Zero(x_cells, y_cells);
  // 初始化用于计算五点系数的耦合量
  // TODO: 这里的耦合量或许可以删除
  for (int i = 0; i < 8; i++) {
    Zx[ParticelType(i)] = Matrix::Zero(x_cells, y_cells);
    Zy[ParticelType(i)] = Matrix::Zero(x_cells, y_cells);
  }
}

void gds::Fluid::init() {
  phi.col(0) *= voltage;
  phi.rightCols(1) *= voltage;
}