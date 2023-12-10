#include "solver.hpp"

int main(void) {
  auto fluid  = std::make_shared<gds::Fluid>();
  auto solver = std::make_shared<gds::Solver>(fluid);
  return 0;
}
