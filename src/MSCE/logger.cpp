#include "logger.h"
#include <chrono>

using namespace msce;
using namespace std::chrono;

namespace
{
    const auto start_time_point = high_resolution_clock::now();
}

// std::ofstream Logger::log_file = ;

void Logger::append_to_log_file(const std::string_view &message) const noexcept
{
    if (!this->log_file.is_open())
    {
        std::cerr << "[ERROR]: [LOGGER]: Failed to create output file!" << std::endl;
        return;
    }

    auto cur_time_point = high_resolution_clock::now();

    log_file << "[" << static_cast<double>(duration_cast<milliseconds>(cur_time_point - start_time_point).count()) / 1000 << "s] | "
             << message << std::endl;
}

msce::Logger::Logger(const std::string &owner_name)
{
    this->owner_name = owner_name;
}
Logger::~Logger()
{
}
