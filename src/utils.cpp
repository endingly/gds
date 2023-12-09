#include "utils.hpp"

#include <stdexcept>

namespace gds::utils {

void Init_Diagonal_Matrix(ESM& sparse_matrix, double constant, int distance) {
  auto size = sparse_matrix.rows();
  if (distance < -size || distance > size) {
    std::range_error("Error: distance is out of range.");
  }
  for (int i = 0; i < size; ++i) {
    if (distance != size - 1) sparse_matrix.insert(i, i + distance) = constant;
    if (distance != 0) sparse_matrix.insert(i, i - distance) = constant;
  }
}

};  // namespace gds::utils