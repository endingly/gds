#pragma once
#include <fstream>
#include <type_traits>

#include "matrix.hpp"

namespace gds::utils {

/// @brief Eigen 矩阵的概念
template <typename T>
concept EigenMatrix = requires { std::is_same_v<T, gds::ESM> || std::is_same_v<T, gds::Matrix>; };

/// @brief 为一个对角线稀疏方阵设置其偏移对角线上的值，且此对角线是对称的
/// @param size 对角矩阵的大小
/// @param constant 对角线上都是相同的值
/// @param distance 对角线的偏移量，默认为 0，主对角线右侧为正，左侧为负
/// @return 稀疏矩阵
template <EigenMatrix T>
void Set_Matrix_Diagonal(T& sparse_matrix, double constant, int distance = 0) {
  auto size = sparse_matrix.rows();
  if (distance < -(size - 1) || distance > size - 1) {
    throw std::range_error("Error: distance is out of range.");
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

/// @brief 计算域 x 方向上的梯度
/// @param input_matrix 计算输入矩阵
/// @param output_matrix 计算输出结果
void Gradient_x(const Matrix& input_matrix, Matrix& output_matrix);

/// @brief 计算域 y 方向上的梯度
/// @param input_matrix 计算输入矩阵
/// @param output_matrix 计算输出结果
void Gradient_y(const Matrix& input_matrix, Matrix& output_matrix);

/// @brief 将矩阵写入 csv 文件
/// @tparam T 密集矩阵或者稀疏矩阵
/// @param matrix 将要写入的矩阵
/// @param file_name 文件名
/// @return 是否成功
template <EigenMatrix T>
bool Write_Matrix_To_CSV(const T& matrix, const std::string& file_name) {
  std::ofstream file;
  file.open(file_name, std::ios::out | std::ios::binary);
  if (!file.is_open()) {
    return false;
  }
  file << matrix << std::endl;
  file.close();
  return true;
}

}  // namespace gds::utils
