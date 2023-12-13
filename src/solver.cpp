#include "solver.hpp"

#include "config.hpp"
#include "utils.hpp"

namespace gds {
using namespace gds::config;

Solver::Solver(/* args */) = default;

Solver::Solver(std::unique_ptr<Fluid> fluid) : fluid(std::move(fluid)) {}

Solver::~Solver() = default;

void Solver::Calculate_ue() {
  auto e = ParticelType::e;

  // 计算粒子扩散速度
  auto         *Kte_s  = this->fluid->Kte.data();
  auto         *uex_s  = this->fluid->ux[e].data();
  constexpr int length = x_cells * y_cells;
  for (int i = 0; i < length; i++) {
    auto  Kte = Kte_s[i];
    auto &uex = uex_s[i];
    if (Kte < 0.04868) {
      uex = 1.839e22 / init_MN;
    }
    if (Kte >= 0.04868 && Kte < 0.6217) {
      uex = (1.27948E22 + 1.06616E23 * Kte - 8.706E22 * pow(Kte, 2)) / init_MN;
    }
    if (Kte >= 0.6217 && Kte < 0.7368) {
      uex = 1.839e22 / init_MN;
    }
    if (Kte >= 0.7368 && Kte < 85.56) {
      uex = (6.21706E22 - 4.84082E22 * (1 - exp(-Kte / 1.78886)) - 1.06655E22 * (1 - exp(-Kte / 9.31518))) / init_MN;
    }
    if (Kte >= 85.56 && Kte < 200) {
      uex = (1.64251E18 * Kte + 2.9575E21) / init_MN;
    }
    if (Kte >= 200) {
      uex = (1.64251E18 * Kte + 2.9575E21) / init_MN;
    }
  }
}

void Solver::Calculate_De() {
  auto e = ParticelType::e;
  using namespace config;

  // 计算粒子迁移速度
  auto         *Kte_s  = this->fluid->Kte.data();
  auto         *Dex_s  = this->fluid->Dx[e].data();
  constexpr int length = x_cells * y_cells;
  for (int i = 0; i < length; i++) {
    auto  Kte = Kte_s[i];
    auto &Dex = Dex_s[i];
    if (Kte < 0.1) {
      Dex = 1.00515 * 1.0E22 / init_MN;
    }
    if (Kte >= 0.1 && Kte < 1.159) {
      Dex = (exp(52.25793 - 1.12899 / (Kte + 0.20674))) / init_MN;
    }
    if (Kte >= 1.159 && Kte < 28.3) {
      Dex = (-3.72599 * 1.0E22 * exp(-Kte / 3.99714) - 5.68012 * 1.0E22 * exp(-Kte / 36.35961) + 1.02742 * 1.0E23) /
            init_MN;
    }
    if (Kte >= 28.3 && Kte < 31.7) {
      Dex = (5.36739 * 1.0E22 + 8.11836 * 1.0E20 * Kte) / init_MN;
    }
    if (Kte >= 31.7) {
      Dex = (2.95262 * 1.0E23 + (4.28465 * 1.0E22 - 2.95262 * 1.0E23) / (1.0 + exp((Kte - 94.82187) / 35.28612))) /
            init_MN;
    }
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

void Solver::Calculate_E() {
  using namespace config;
  // 电势的梯度就是电场强度
  const Matrix &phi = this->fluid->phi;
  auto         &Ex  = this->fluid->Ex;
  auto         &Ey  = this->fluid->Ey;
  auto         &E   = this->fluid->E;

  utils::Gradient_x(phi, Ex);
  utils::Gradient_y(phi, Ey);
  // 计算合场强
  for (int j = 0; j < E.cols(); j++)
    for (int i = 0; i < E.rows(); i++) {
      if ((j == 0 || j == E.cols() - 1) && i >= top_wall && i <= bot_wall)
        E(i, j) = 0;
      else
        E(i, j) = sqrt(pow(Ex(i, j), 2) + pow(Ey(i, j), 2));
    }
}

void Solver::Calculate_v_surf() {

}

void Solver::Calculate_Gamma() {
  using namespace gds::config;

  // eq Math: \boldsymbol{\Gamma}_{\mathrm{i}}=-n_{\mathrm{i}} \mu_{\mathrm{i}} \mathbf{E}-D_{\mathrm{i}} \nabla
  // n_{\mathrm{i}}
  // 1. 计算除电子外的粒子的密度通量
  for (int i = 0; i < static_cast<int>(ParticelType::e); i++) {
    auto  pType = static_cast<ParticelType>(i);
    auto &E     = this->fluid->E;
    auto &n     = this->fluid->n[pType];
    auto &ux    = this->fluid->ux[pType];
    auto &uy    = this->fluid->uy[pType];
    auto &Dx    = this->fluid->Dx[pType];
    auto &Dy    = this->fluid->Dy[pType];

    // 结果用后即焚，所以结果分配在栈堆上，缩减堆分配的时间
    stack_matrix n_x     = stack_matrix::Zero();
    stack_matrix n_y     = stack_matrix::Zero();
    stack_matrix Gamma_x = stack_matrix::Zero();
    stack_matrix Gamma_y = stack_matrix::Zero();

    utils::Gradient_x(n, n_x);
    utils::Gradient_y(n, n_y);
    if ((int)pType < 10) {
      Gamma_x = -Dx.array() * n_x.array();
      Gamma_y = -Dy.array() * n_y.array();
    } else {
      auto sig = 0;
      (int)pType < 20 ? sig = 1 : sig = -1;
      Gamma_x = sig * n.array() * ux.array() * E.array() - Dx.array() * n_x.array();
      Gamma_y = sig * n.array() * uy.array() * E.array() - Dy.array() * n_y.array();
    }

    // 根据上面两个方向的通量合成总通量
    this->fluid->Gamma[pType] = (Gamma_x.pow(2) + Gamma_y.pow(2)).sqrt();
  }
}

void Solver::Calculate_S() {
  // eq Math: S_i = \sum(\alpha_i|\Gamma_i|)
  // 1. 计算除电子外的粒子的带电粒子源项
  for (int i = 10; i < static_cast<int>(ParticelType::e); i++) {
    auto  pType = static_cast<ParticelType>(i);
    auto &S     = this->fluid->S[pType];
    
  }
}

void Solver::Advance() {
  // 1. 计算电子迁移速度与扩散系数
  Calculate_ue();
  Calculate_De();
  // 2. 计算泊松方程得到电势分布
  Calculate_PossionEquation();
  // 3. 计算电场强度
  Calculate_E();
  // 4. 计算表面速度
  Calculate_v_surf();
  // 5. 计算粒子通量
  Calculate_Gamma();
}

}  // namespace gds