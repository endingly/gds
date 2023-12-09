#pragma once
#include <Eigen/Core>
#include <Eigen/Sparse>

namespace gds::utils {
    
using ESM = Eigen::SparseMatrix<double, Eigen::RowMajor>;

/// @brief 为一个对角线稀疏方阵设置其偏移对角线上的值，且此对角线是对称的
/// @param size 对角矩阵的大小
/// @param constant 对角线上都是相同的值
/// @param distance 对角线的偏移量，默认为 0，始终为正
/// @return 稀疏矩阵
void Init_Diagonal_Matrix(ESM& sparse_matrix, double constant, int distance = 0);

}  // namespace gds::utils
