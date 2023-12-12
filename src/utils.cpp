#include "utils.hpp"

#include "config.hpp"

namespace gds::utils {
using namespace gds::config;

void Gradient_x(const Matrix& input_matrix, Matrix& output_matrix) {
// 并行算法，72*60，使用 6 个核心
// 由于矩阵按列优先存储，所以按列遍历，每个核心处理 10 列，每列 72 个元素，共 720 个元素
#pragma omp parallel num_threads(6)
  {
    int tn    = omp_get_thread_num();
    int j     = tn * 10;
    int limit = j + 10;
    for (; j < limit; j++) {
      for (int i = 0; i < input_matrix.rows() - 1; i++) {
        output_matrix(i, j) = (input_matrix(i, j + 1) - input_matrix(i, j)) / dx;
      }
    }
  }
}

void Gradient_y(const Matrix& input_matrix, Matrix& output_matrix) {
  // 并行算法，72*60，使用 6 个核心
  // 由于矩阵按列优先存储，所以按列遍历，每个核心处理 10 列，每列 72 个元素，共 720 个元素
#pragma omp parallel num_threads(6)
  {
    int tn    = omp_get_thread_num();
    int j     = tn * 10;
    int limit = j + 10;
    for (; j < limit; j++) {
      for (int i = 0; i < input_matrix.rows() - 1; i++) {
        output_matrix(i, j) = (input_matrix(i + 1, j) - input_matrix(i, j)) / dy;
      }
    }
  }
}

};  // namespace gds::utils