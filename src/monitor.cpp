#include "monitor.hpp"

#include <fmt/color.h>

#include <iostream>

#include "application_info.hpp"
#include "config.hpp"

template <typename T, typename U>
  requires std::is_arithmetic_v<T> && (std::is_same_v<U, std::string> || std::is_same_v<U, const char*>)
std::string to_color_string(T value, U unit) {
  auto v = fmt::styled(value, fmt::fg(fmt::color::green));
  auto u = fmt::styled(unit, fmt::fg(fmt::color::blue));
  return fmt::format("{0} {1}", v, u);
}

void gds::utils::Monitor::PrintInfo() {
  fmt::println("Application name:    {0}", gds::application_info::name);
  fmt::println("Application version: {0}", gds::application_info::version);
  fmt::println("Application author:  {0}", gds::application_info::author);
  fmt::println("Communication email: {0}", gds::application_info::email);
}

// clang-format off
void gds::utils::Monitor::PrintConfig() {
  fmt::println("max length of x axis:                    {0}", to_color_string(gds::config::xmax, "cm"));
  fmt::println("max length of y axis:                    {0}", to_color_string(gds::config::ymax, "cm"));
  fmt::println("cells number of x axis:                  {0}", to_color_string(gds::config::x_cells, ""));
  fmt::println("cells number of y axis:                  {0}", to_color_string(gds::config::y_cells, ""));
  fmt::println("pressure:                                {0}", to_color_string(gds::config::pressure, "torr"));
  fmt::println("voltage:                                 {0}", to_color_string(gds::config::voltage, "V"));
  fmt::println("time step:                               {0}", to_color_string(gds::config::dt, "s"));
  fmt::println("secondary electron emission coefficient: {0}", to_color_string(gds::config::gama, ""));
  fmt::println("top wall:                                {0}", to_color_string(gds::config::top_wall, ""));
  fmt::println("bottom wall:                             {0}", to_color_string(gds::config::bot_wall, ""));
  fmt::println("helium ratio:                            {0}", to_color_string(gds::config::f_He, ""));
  fmt::println("carbon dioxide ratio:                    {0}", to_color_string(gds::config::f_CO2, ""));
  fmt::println("initial neutral particle number:         {0}", to_color_string(gds::config::init_MN, ""));
  fmt::println("electron charge:                         {0}", to_color_string(gds::config::Qe, "C"));
  fmt::println("vacuum permittivity:                     {0}", to_color_string(gds::config::EPSILON0, "F/m"));
  fmt::println("electron collision frequency:            {0}", to_color_string(gds::config::Omega, "1/s"));
}
// clang-format on