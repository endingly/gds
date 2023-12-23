#pragma once
#include <vector>

// [ ]: 计算电离率之前需要完善对反应体系的构建

namespace gds {
using std::vector;
using ReactionCofficient = double;

class ReactionManager {
 private:
  vector<ReactionCofficient> reaction_cofficients;

 public:
  ReactionManager()  = default;
  ~ReactionManager() = default;

 private:
  void init();
};

};  // namespace gds