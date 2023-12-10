#include <gtest/gtest.h>

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
  std::cout << sparse_matrix << std::endl;
}