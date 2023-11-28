#include "fluid.hpp"

gds::Fluid::Fluid() {
  n[ParticelType::O2]  = Matrix::Constant(1, 1, 1.0);
  n[ParticelType::CO2] = Matrix();
}