#pragma once
#include <string>
#include <vector>

// [ ]: 计算电离率之前需要完善对反应体系的构建
// [ ]: 准备利用 yaml 文件来确定反应以及反应速率系数

namespace gds {
using std::vector;
using ReactionCofficient = double;

/// @brief 反应类型
enum class ReactionType { elastic, ionization, excitation };

/// @brief 反应结构体
struct Reaction {
  ReactionType reaction_type;        // 反应类型
  double       reaction_cofficient;  // 反应速率系数
  std::string  reaction_expression;  // 反应表达式
};

/// @brief 化学反应管理器，负责化学反应的进行以及反应速率系数的计算，更新 `n_e` 矩阵以及 `N` 矩阵
class ReactionManager {
 private:
  vector<ReactionCofficient> reaction_cofficients;

 public:
  ReactionManager()  = default;
  ~ReactionManager() = default;

 private:
  /// @brief 初始化反应
  void init();
  /// @brief 读取反应信息
  void read_reaction_info();
};

};  // namespace gds