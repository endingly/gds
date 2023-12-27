#pragma once
#include <string>

namespace gds::utils {

/// @brief 反应解析器，负责解析 json 中的化学反应方程式
class ReactiveParser {
 private:
  /// @brief 解析反应表达式
  void parse_formura(std::string_view formula);

 public:
   ReactiveParser(/* args */);
  ~ReactiveParser();
};

};  // namespace gds::utils
