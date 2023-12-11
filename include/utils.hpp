#pragma once
#include <type_traits>

#include "matrix.hpp"

namespace gds::utils {

/// @brief 为一个对角线稀疏方阵设置其偏移对角线上的值，且此对角线是对称的
/// @param size 对角矩阵的大小
/// @param constant 对角线上都是相同的值
/// @param distance 对角线的偏移量，默认为 0，主对角线右侧为正，左侧为负
/// @return 稀疏矩阵
template <typename T>
  requires std::is_same_v<T, gds::ESM> || std::is_same_v<T, gds::Matrix>
void Set_Matrix_Diagonal(T& sparse_matrix, double constant, int distance = 0) {
  auto size = sparse_matrix.rows();
  if (distance < -(size - 1) || distance > size - 1) {
    std::range_error("Error: distance is out of range.");
  }
  if (distance == 0) {
    sparse_matrix.setIdentity();
    sparse_matrix *= constant;
  } else if (distance > 0) {
    for (int i = 0; i < size; ++i) {
      if (i < size - distance) sparse_matrix.insert(i, i + distance) = constant;
    }
  } else {
    for (int i = 0; i < size; ++i) {
      if (i >= abs(distance)) sparse_matrix.insert(i, i + distance) = constant;
    }
  }
}

}  // namespace gds::utils
