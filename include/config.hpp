#pragma once
namespace gds::config {

constexpr float  xmax     = 0.24f;  // x 轴最大长度
constexpr float  ymax     = 0.2f;   // y 轴最大长度
constexpr int    x_cells  = 73;     // x 轴网格数
constexpr int    y_cells  = 61;     // y 轴网格数
constexpr int    pressure = 15;     // 压强
constexpr int    voltage  = 800;    // 电压
constexpr double dt       = 1e-20;  // 时间步长
constexpr int    gama     = 0;      // 二次电子发射系数
constexpr int    top_wall = 7;      // 顶壁
constexpr int    bot_wall = 67;     // 底壁
constexpr float  f_He     = 0.0f;   // 氦气分子比例
constexpr float  f_CO2    = 1.0f;   // 二氧化碳分子比例
constexpr double init_MN = pressure * 133.3 * 1e-6 / (1.38 * 1e-23 * 300);  // 初始中性粒子数

constexpr double Qe       = 1.6 * 1e-19;    // 电子电量
constexpr double EPSILON0 = 8.854 * 1e-12;  // 真空介电常数
constexpr double Omega    = 1.19 * 1e-4;    // 电子碰撞频率

}  // namespace gds::config
