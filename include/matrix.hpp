#pragma once
#include <Eigen/Dense>
#include <Eigen//Sparse>
namespace gds {

using Matrix = Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::ColMajor>;
using ESM    = Eigen::SparseMatrix<double, Eigen::ColMajor>;

};  // namespace gds