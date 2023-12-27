#pragma once
#include <fstream>
#include <optional>

#include "matrix.hpp"

namespace gds::utils {

using namespace gds::config;

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
      if (i < size - distance) {
        sparse_matrix.insert(i, i + distance) = constant;
      }
    }
  } else {
    for (int i = 0; i < size; ++i) {
      if (i >= abs(distance)) {
        sparse_matrix.insert(i, i + distance) = constant;
      }
    }
  }
}

/// @brief 计算域 x 方向上的梯度
/// @param input_matrix 计算输入矩阵
/// @param output_matrix 计算输出结果
template <EigenMatrix T>
void Gradient_x(const Matrix& input_matrix, T& output_matrix) {
  for (int i = 0; i < input_matrix.cols(); i++) {
    auto out = output_matrix.col(i).array();
    auto f   = input_matrix.col(i).array();
    if (i == 0) {
      auto l = input_matrix.col(i + 1).array();
      out    = (l - f) / dx;
    } else if (i == input_matrix.cols() - 1) {
      auto p = input_matrix.col(i - 1).array();
      out    = (f - p) / dx;
    } else {
      auto p = input_matrix.col(i - 1).array();
      auto l = input_matrix.col(i + 1).array();
      out    = (l - p) / (2 * dx);
    }
  }
}

/// @brief 计算域 y 方向上的梯度
/// @details 边界为两点前向差分，中间为三点中心差分
/// @param input_matrix 计算输入矩阵
/// @param output_matrix 计算输出结果
template <EigenMatrix T>
void Gradient_y(const Matrix& input_matrix, T& output_matrix) {
  // 为矩阵在 y 方向上计算梯度
  for (int i = 0; i < input_matrix.rows(); i++) {
    auto out = output_matrix.row(i).array();
    auto f   = input_matrix.row(i).array();
    if (i == 0) {
      auto l = input_matrix.row(i + 1).array();
      out    = (l - f) / dy;
    } else if (i == input_matrix.rows() - 1) {
      auto p = input_matrix.row(i - 1).array();
      out    = (f - p) / dy;
    } else {
      auto p = input_matrix.row(i - 1).array();
      auto l = input_matrix.row(i + 1).array();
      // out    = (-l + 4 * f - 3 * p) / 2 * dy;
      out = (l - p) / (2 * dy);
    }
  }
}

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

/// @brief 从字符串中定位某一子串
/// @param str 指定要搜索的字符串
/// @param substr 指定要搜索的子串
/// @return `->` 符号的位置
int find_sign(std::string_view str, std::string_view substr);

/// @brief 从字符串中定位某一子串
/// @param begin 指向开始位置的迭代器
/// @param end 指向结束位置的迭代器
/// @param substr 指定要搜索的子串
/// @return
int find_sign(const std::string::iterator& begin, const std::string::iterator& end, const std::string& substr);

/// @brief 拆分字符串，内部存在拷贝开销
/// @param str 需要拆分的字符串
/// @param delim 指定拆分依据
/// @return 返回拆分结果的视图
std::optional<std::vector<std::string>> split(const std::string_view& str, const std::string_view& delim);

/// @brief 去除字符串中的空格
/// @param str 需要去除空格的字符串
/// @return 返回去除空格后的字符串
std::optional<std::string_view> remove_space(std::string_view str);

}  // namespace gds::utils
