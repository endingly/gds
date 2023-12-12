#include <gtest/gtest.h>

#include "config.hpp"
#include "utils.hpp"
using namespace gds;

TEST(Init_Diagonal_Matrix, Normal) {
  gds::ESM sparse_matrix(5, 5);
  gds::utils::Set_Matrix_Diagonal(sparse_matrix, 1.0, 0);
  //   EXPECT_EQ(sparse_matrix.coeff(0, 0), 1.0);
  //   EXPECT_EQ(sparse_matrix.coeff(1, 1), 1.0);
  //   EXPECT_EQ(sparse_matrix.coeff(2, 2), 1.0);
  //   EXPECT_EQ(sparse_matrix.coeff(3, 3), 1.0);
  //   EXPECT_EQ(sparse_matrix.coeff(4, 4), 1.0);
  std::cout << sparse_matrix << std::endl;
}

TEST(Init_Diagonal_Matrix, Tri_Diag_Matrix) {
  gds::ESM sparse_matrix(5, 5);
  gds::utils::Set_Matrix_Diagonal(sparse_matrix, 1.0, 0);
  gds::utils::Set_Matrix_Diagonal(sparse_matrix, 2.0, 1);
  gds::utils::Set_Matrix_Diagonal(sparse_matrix, 3.0, -1);
  std::cout << sparse_matrix << std::endl;
}

TEST(Utils, Gradient_x) {
  Eigen::VectorXd input_vector(gds::config::y_cells);
  for (int i = 0; i < input_vector.size(); ++i) {
    input_vector(i) = i;
  }
  gds::Matrix input_matrix(gds::config::x_cells, gds::config::y_cells);
  for (int i = 0; i < input_matrix.rows(); ++i) {
    input_matrix.row(i) = input_vector;
  }
  // 将 matrix 写入 csv 文件
  gds::utils::Write_Matrix_To_CSV(input_matrix, "input_matrix.csv");

  gds::Matrix output_matrix = gds::Matrix::Zero(gds::config::x_cells, gds::config::y_cells);
  gds::utils::Gradient_x(input_matrix, output_matrix);
  gds::utils::Write_Matrix_To_CSV(output_matrix, "output_matrix.csv");
}

TEST(Utils, Gradient_y) {
  Eigen::VectorXd input_vector(gds::config::x_cells);
  for (int i = 0; i < input_vector.size(); ++i) {
    input_vector(i) = i;
  }
  gds::Matrix input_matrix(gds::config::x_cells, gds::config::y_cells);
  for (int i = 0; i < input_matrix.cols(); ++i) {
    input_matrix.col(i) = input_vector;
  }
  // 将 matrix 写入 csv 文件
  gds::utils::Write_Matrix_To_CSV(input_matrix, "input_matrix.csv");

  gds::Matrix output_matrix = gds::Matrix::Zero(gds::config::x_cells, gds::config::y_cells);
  gds::utils::Gradient_y(input_matrix, output_matrix);
  gds::utils::Write_Matrix_To_CSV(output_matrix, "output_matrix.csv");
}