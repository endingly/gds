#pragma once
#include <string>

namespace gds::utils {

enum class LogLevel { INFO, WARNING, ERROR };

class Logger {
 public:
  static inline std::string log_file_path_  = "./log/";
  static inline bool        is_log_to_file_ = false;

 private:
  static void WriteLog(LogLevel log_level, std::string log);

 public:
  static void Info(std::string log);
  static void Warning(std::string log);
  static void Error(std::string log);
};

}  // namespace gds::utils
