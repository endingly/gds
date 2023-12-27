#include "reactive_parser.hpp"

#include "utils.hpp"

namespace gds::utils {

ReactiveParser::ReactiveParser(/* args */) {}

ReactiveParser::~ReactiveParser() {}

void ReactiveParser::parse_formura(std::string_view formula) {
  int              index = utils::find_sign(formula, "->");             // 找到反应符号的位置
  std::string_view left  = std::string_view(formula).substr(0, index);  // 反应物
  std::string_view right = std::string_view(formula).substr(index + 2, formula.size() - index - 2);  // 生成物
  // 开始解析生成物
  auto lw = utils::split(left, "+");
  auto rw = utils::split(right, "+");
}

};  // namespace gds::utils