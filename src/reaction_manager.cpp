#include "reaction_manager.hpp"

namespace gds {

void ReactionManager::init() {
  // 反应总数提前申请好
  auto &Kr = reaction_cofficients;
  Kr.resize(100);
  // 分别初始化每个反应的反应系数
  // 1. 弹性碰撞
  // 2. 电离反应
  // 3. 激发反应

  // TODO: 目前为了方便，直接全都设置成一个值，后续需要根据反应类型进行设置
  for (auto &i : Kr) {
    i = 1.0e-10;
  }
}

}  // namespace gds