#include "solver.hpp"

#include "config.hpp"
#include "utils.hpp"

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
  const auto &rho = this->fluid->n[e];
  const auto &phi = this->fluid->phi;
  // 计算泊松方程

  /// 1. 直接构造系数矩阵 A
  //  需要指出，这里的系数矩阵 A 是一个五对角线稀疏矩阵，因此我们可以使用稀疏矩阵的存储方式来存储它
  auto A = ESM(x_cells * y_cells, x_cells * y_cells);
  A.setIdentity();
  // eq Math: A = 2(\frac{1}{dx^2} + \frac{1}{dy^2})
  A *= 2 * ((1 / pow(this->fluid->dx, 2)) + (1 / pow(this->fluid->dy, 2)));
  utils::Set_Matrix_Diagonal(A, -1 / pow(this->fluid->dx, 2), 1);
  utils::Set_Matrix_Diagonal(A, -1 / pow(this->fluid->dx, 2), -1);
  utils::Set_Matrix_Diagonal(A, -1 / pow(this->fluid->dy, 2), x_cells);
  utils::Set_Matrix_Diagonal(A, -1 / pow(this->fluid->dy, 2), -x_cells);
  /// 2. 构造方程右端项 f
  //  2.1 首先构造矩阵 D，用于初始化 f
  auto D = ESM(x_cells, x_cells);
  D.setIdentity();
  D *= (-(1 / pow(this->fluid->dx, 2)));
  //  2.2 然后构造 f
  Matrix f_element = (-(rho / config::EPSILON0));
  f_element.topRows(1).array() += phi.topRows(1).array() / pow(this->fluid->dy, 2);
  f_element.bottomRows(1).array() += phi.bottomRows(1).array() / pow(this->fluid->dy, 2);
  f_element.leftCols(1) += D * phi.leftCols(1);
  f_element.rightCols(1) += D * phi.rightCols(1);
  // 2.3 将赋值完成的 f_element 转换为 VectorXd
  auto f = Eigen::Map<Eigen::VectorXd>(f_element.data(), f_element.size());
  /// 3. 求解方程
  //  3.1 构造一个稀疏矩阵的求解器
  Eigen::SimplicialLDLT<ESM> solver(A);
  //  3.2 求解方程
  Eigen::VectorXd phi_vector = solver.solve(f);

  //  3.3 如果成功求解，则将结果赋值给 phi
  if (solver.info() == Eigen::Success) {
    this->fluid->phi = Eigen::Map<Matrix>(phi_vector.data(), x_cells, y_cells);
  } else {
    throw std::runtime_error("Solver::Calculate_PossionEquation: Failed to solve possion equation.");
  }
}

}  // namespace gds