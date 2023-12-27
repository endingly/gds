#include "log.hpp"

#include <fmt/format.h>

#include <filesystem>
#include <fstream>
#include <iostream>

using gds::utils::LogLevel, fmt::format;

std::string to_string(LogLevel log_level) {
  switch (log_level) {
    case LogLevel::INFO:
      return "INFO";
    case LogLevel::WARNING:
      return "WARNING";
    case LogLevel::ERROR:
      return "ERROR";
    default:
      return "UNKNOWN";
  }
}

std::string get_date(const tm& t) { return format("{:04d}-{:02d}-{:02d}", t.tm_year + 1900, t.tm_mon + 1, t.tm_mday); }

std::string get_time(const tm& t) { return format("{:02d}:{:02d}:{:02d}", t.tm_hour, t.tm_min, t.tm_sec); }

std::string get_date_time(const tm& t) {
  return format("{:04d}-{:02d}-{:02d} {:02d}:{:02d}:{:02d}", t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, t.tm_hour,
                t.tm_min, t.tm_sec);
}

void gds::utils::Logger::WriteLog(LogLevel log_level, const std::string& log) {
  auto  t       = std::chrono::system_clock::now();
  auto  ct      = std::chrono::system_clock::to_time_t(t);
  auto* time_tm = localtime(&ct);

  auto s = fmt::format("[{}][{}] {}\n", to_string(log_level), get_date_time(*time_tm), log);
  std::cout << s;
  if (is_log_to_file_) {
    if (!std::filesystem::exists(log_file_path_)) {
      std::filesystem::create_directory(log_file_path_);
    }
    std::ofstream log_file;
    log_file.open(log_file_path_ + get_date(*time_tm) + ".log", std::ios::app);
    log_file << s;
    log_file.close();
  }
}

void gds::utils::Logger::Info(const std::string& log) { WriteLog(LogLevel::INFO, log); }

void gds::utils::Logger::Warning(const std::string& log) { WriteLog(LogLevel::WARNING, log); }

void gds::utils::Logger::Error(const std::string& log) { WriteLog(LogLevel::ERROR, log); }
