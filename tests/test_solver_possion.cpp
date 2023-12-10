#include <gtest/gtest.h>

#include <fstream>

#include "config.hpp"
#include "matrix.hpp"
#include "utils.hpp"

using namespace gds::config;
using namespace gds::utils;
using gds::Matrix, gds::ESM;

void calculate_PossionEquation() {
  Matrix rho = Matrix::Constant(x_cells, y_cells, 1e5);
  Matrix phi = Matrix::Zero(x_cells, y_cells);
  phi.leftCols(1).array() += 800;
  phi.rightCols(1).array() += 800;
  auto dx = 0.01, dy = dx;
  // 计算泊松方程

  /// 1. 直接构造系数矩阵 A
  //  需要指出，这里的系数矩阵 A 是一个五对角线稀疏矩阵，因此我们可以使用稀疏矩阵的存储方式来存储它
  auto A = ESM(x_cells * y_cells, x_cells * y_cells);
  A.setIdentity();
  // eq Math: A = 2(\frac{1}{dx^2} + \frac{1}{dy^2})
  A *= 2 * ((1 / pow(dx, 2)) + (1 / pow(dy, 2)));
  Set_Matrix_Diagonal(A, -1 / pow(dx, 2), 1);
  Set_Matrix_Diagonal(A, -1 / pow(dx, 2), -1);
  Set_Matrix_Diagonal(A, -1 / pow(dy, 2), x_cells);
  Set_Matrix_Diagonal(A, -1 / pow(dy, 2), -x_cells);
  /// 2. 构造方程右端项 f
  //  2.1 首先构造矩阵 D，用于初始化 f
  auto D = ESM(x_cells, x_cells);
  D.setIdentity();
  D *= (-(1 / pow(dx, 2)));
  //  2.2 然后构造 f
  Matrix f_element = (-(rho / EPSILON0));
  f_element.topRows(1).array() += phi.topRows(1).array() / pow(dy, 2);
  f_element.bottomRows(1).array() += phi.bottomRows(1).array() / pow(dy, 2);
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
    Matrix       phi_next = Eigen::Map<Matrix>(phi_vector.data(), x_cells, y_cells);
    std::fstream file;
    file.open("phi_next.csv", std::ios::binary | std::ios::out);

    if (file.is_open()) {
      file << phi_next;
      file.close();
    }
    file.open("phi.csv", std::ios::binary | std::ios::out);
    if (file.is_open()) {
      file << phi;
      file.close();
    }
  } else {
    throw std::runtime_error("Solver::Calculate_PossionEquation: Failed to solve possion equation.");
  }
}

TEST(test_solver_possion, normal) { calculate_PossionEquation(); }