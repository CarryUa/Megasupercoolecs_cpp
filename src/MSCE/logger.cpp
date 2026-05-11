#include "logger.h"
#include <chrono>

using namespace msce;

std::size_t Logger::millis_at_start = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();

// std::ofstream Logger::log_file = ;

void Logger::append_to_log_file(const std::string_view &message) const noexcept
{
    if (!this->log_file.is_open())
    {
        std::cerr << "[ERROR]: [LOGGER]: Failed to create output file!" << std::endl;
        return;
    }

    std::size_t cur_t = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();

    log_file << "[" << (double)(cur_t - millis_at_start) / 1000 << "s] | " << message.data() << "\n";
}

msce::Logger::Logger(const std::string &owner_name)
{
    if (this->millis_at_start == 0)
        this->millis_at_start = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();

    this->owner_name = owner_name;
}
Logger::~Logger()
{
}
