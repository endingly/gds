#include "solver.hpp"

int main(void) {
  auto fluid  = std::make_unique<gds::Fluid>();
  auto solver = std::make_unique<gds::Solver>(std::move(fluid));
}
