#include "reaction_manager.hpp"

namespace gds {

void ReactionManager::init() {
  // 反应总数提前申请好
  auto &Kr = reaction_cofficients;
  Kr.resize(100);
  // 分别初始化每个反应的反应系数
  // 1. 弹性碰撞
  Kr[0] = 1.0e-10;
  Kr[1] = 1.0e-10;
  Kr[2] = 1.0e-10;
  // 2. 电离反应
  // 3. 激发反应
}

}  // namespace gds