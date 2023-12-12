#include <benchmark/benchmark.h>

#include "config.hpp"
#include "utils.hpp"

static void BM_Gradient_x(benchmark::State& state) {
  gds::Matrix input_matrix(state.range(0), state.range(1));
  gds::Matrix output_matrix(state.range(0), state.range(1));
  for (auto _ : state) {
    gds::utils::Gradient_x(input_matrix, output_matrix);
  }
}

static void BM_Gradient_y(benchmark::State& state) {
  gds::Matrix input_matrix(state.range(0), state.range(1));
  gds::Matrix output_matrix(state.range(0), state.range(1));
  for (auto _ : state) {
    gds::utils::Gradient_y(input_matrix, output_matrix);
  }
}

BENCHMARK(BM_Gradient_x)->Args({gds::config::x_cells, gds::config::y_cells});
BENCHMARK(BM_Gradient_y)->Args({gds::config::x_cells, gds::config::y_cells});

void Gradient_x_no_parallel(const gds::Matrix& input_matrix, gds::Matrix& output_matrix) {
  for (int j = 0; j < input_matrix.cols() - 1; j++) {
    for (int i = 0; i < input_matrix.rows(); i++) {
      output_matrix(i, j) = (input_matrix(i, j + 1) - input_matrix(i, j)) / gds::config::dx;
    }
  }
}

void Gradient_y_no_parallel(const gds::Matrix& input_matrix, gds::Matrix& output_matrix) {
  for (int j = 0; j < input_matrix.cols(); j++) {
    for (int i = 0; i < input_matrix.rows() - 1; i++) {
      output_matrix(i, j) = (input_matrix(i + 1, j) - input_matrix(i, j)) / gds::config::dy;
    }
  }
}

static void BM_Gradient_x_no_parallel(benchmark::State& state) {
  gds::Matrix input_matrix(state.range(0), state.range(1));
  gds::Matrix output_matrix(state.range(0), state.range(1));
  for (auto _ : state) {
    Gradient_x_no_parallel(input_matrix, output_matrix);
  }
}

static void BM_Gradient_y_no_parallel(benchmark::State& state) {
  gds::Matrix input_matrix(state.range(0), state.range(1));
  gds::Matrix output_matrix(state.range(0), state.range(1));
  for (auto _ : state) {
    Gradient_y_no_parallel(input_matrix, output_matrix);
  }
}

BENCHMARK(BM_Gradient_x_no_parallel)->Args({gds::config::x_cells, gds::config::y_cells});
BENCHMARK(BM_Gradient_y_no_parallel)->Args({gds::config::x_cells, gds::config::y_cells});