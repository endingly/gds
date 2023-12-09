#include "solver.hpp"

#include "config.hpp"

namespace gds {

Solver::Solver(/* args */) {}

Solver::~Solver() {}

void Solver::Calculate_ue() {
  auto e = ParticelType::e;
  using namespace config;

  // 计算粒子迁移速度
  for (int i = 0, j = 0; j < x_cells, i < y_cells; i++, j++) {
    auto  Kte = this->fluid->Kte(i, j);
    auto &uex = (this->fluid->ux[e](i, j));

    if (Kte < 0.04868) uex = 1.839e22 / init_MN;
    if (Kte >= 0.04868 && Kte < 0.6217) uex = (1.27948E22 + 1.06616E23 * Kte - 8.706E22 * pow(Kte, 2)) / init_MN;
    if (Kte >= 0.6217 && Kte < 0.7368) uex = 1.839e22 / init_MN;
    if (Kte >= 0.7368 && Kte < 85.56)
      uex = (6.21706E22 - 4.84082E22 * (1 - exp(-Kte / 1.78886)) - 1.06655E22 * (1 - exp(-Kte / 9.31518))) / init_MN;
    if (Kte >= 85.56 && Kte < 200) uex = (1.64251E18 * Kte + 2.9575E21) / init_MN;
    if (Kte >= 200) uex = (1.64251E18 * Kte + 2.9575E21) / init_MN;
  }
  this->fluid->uy[e] = this->fluid->ux[e];
}

void Solver::Calculate_De() {
  auto e = ParticelType::e;
  using namespace config;

  // 计算粒子迁移速度
  for (int i = 0, j = 0; j < x_cells, i < y_cells; i++, j++) {
    auto  Kte = this->fluid->Kte(i, j);
    auto &Dex = (this->fluid->Dx[e](i, j));

    if (Kte < 0.1) Dex = 1.00515 * 1.0E22 / init_MN;
    if (Kte >= 0.1 && Kte < 1.159) Dex = (exp(52.25793 - 1.12899 / (Kte + 0.20674))) / init_MN;
    if (Kte >= 1.159 && Kte < 28.3)
      Dex = (-3.72599 * 1.0E22 * exp(-Kte / 3.99714) - 5.68012 * 1.0E22 * exp(-Kte / 36.35961) + 1.02742 * 1.0E23) /
            init_MN;
    if (Kte >= 28.3 && Kte < 31.7) Dex = (5.36739 * 1.0E22 + 8.11836 * 1.0E20 * Kte) / init_MN;
    if (Kte >= 31.7)
      Dex = (2.95262 * 1.0E23 + (4.28465 * 1.0E22 - 2.95262 * 1.0E23) / (1.0 + exp((Kte - 94.82187) / 35.28612))) /
            init_MN;
  }
  this->fluid->Dy[e] = this->fluid->Dx[e];
}

void Solver::Calculate_PossionEquation() {
  using namespace config;
  using ParticelType::e;

  // 计算泊松方程
  for (int i = 0, j = 0; j < x_cells, i < y_cells; i++, j++) {
    auto &phi = this->fluid->phi(i, j);
    auto &rho = this->fluid->n[e](i, j);
    auto &Ex  = this->fluid->Ex(i, j);
    auto &Ey  = this->fluid->Ey(i, j);

    // phi = (rho * pow(dx, 2) + Ex * pow(dy, 2)) / (2 * (pow(dx, 2) + pow(dy, 2)));
  }
  /// 1. 构造系数矩阵 D
  /// 需要指出，这里的系数矩阵 D 是一个稀疏矩阵，因此我们可以使用稀疏矩阵的存储方式来存储它
  Matrix D = Matrix::Identity(x_cells, x_cells) * (-1 / pow(this->fluid->dy, 2));
  /// 2. 构造系数矩阵 C，此矩阵是一个三对角矩阵
  Matrix C = Matrix::Identity(x_cells, x_cells) * 2 * (1 / pow(this->fluid->dx, 2) + 1 / pow(this->fluid->dy, 2));
  for (int i = 0; i < x_cells; i++) {
    if (i == 0)
      C(i, i + 1) = -1 / pow(this->fluid->dx, 2);
    else if (i == x_cells - 1)
      C(i, i - 1) = -1 / pow(this->fluid->dx, 2);
    else {
      C(i, i - 1) = -1 / pow(this->fluid->dx, 2);
      C(i, i + 1) = -1 / pow(this->fluid->dx, 2);
    }
  }
  /// 3. 构造系数矩阵 f
}

}  // namespace gds