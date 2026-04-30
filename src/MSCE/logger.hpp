#ifndef MSCE_LOGGER_H_
#define MSCE_LOGGER_H_
#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <format>
#include <chrono>

namespace msce
{
    class Logger
    {
    private:
        std::string owner_name;
        static std::size_t millis_at_start;
        static std::ofstream log_file;

        void append_to_log_file(const std::string_view &message = "") const noexcept;

    public:
        Logger(const std::string &owner_name);
        ~Logger();

        template <typename... Args>
        void log_info(const std::string &message, Args... args) const;
        template <typename... Args>
        void log_warning(const std::string &message, Args... args) const;
        template <typename... Args>
        void log_error(const std::string &message, Args... args) const;
        template <typename... Args>
        void log_debug(const std::string &message, Args... args) const;
    };

    inline std::size_t Logger::millis_at_start = 0;
    inline std::ofstream Logger::log_file = std::ofstream(std::filesystem::temp_directory_path() / "msce_log.txt");

    inline void Logger::append_to_log_file(const std::string_view &message) const noexcept
    {
        if (!this->log_file.is_open())
        {
            std::cerr << "[ERROR]: [LOGGER]: Failed to create output file!" << std::endl;
            return;
        }

        std::size_t cur_t = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();

        log_file << "[" << cur_t - this->millis_at_start << "ms] | " << message.data() << "\n";
    }

    inline msce::Logger::Logger(const std::string &owner_name)
    {
        if (this->millis_at_start == 0)
            this->millis_at_start = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();

        this->owner_name = owner_name;
    }
    inline Logger::~Logger()
    {
        this->log_file.close();
    }

    template <typename... Args>
    inline void Logger::log_info(const std::string &message, Args... args) const
    {
        const std::string formatted_msg = std::vformat(message, std::make_format_args(args...));
        std::cout << "[INFO]: [" << this->owner_name << "]: " << formatted_msg << std::endl;
        append_to_log_file(std::string_view("[INFO]: [" + owner_name + "]: " + formatted_msg));
    }

    template <typename... Args>
    inline void Logger::log_warning(const std::string &message, Args... args) const
    {
        const std::string formatted_msg = std::vformat(message, std::make_format_args(args...));
        std::cout << "[WARNING]: [" << this->owner_name << "]: " << formatted_msg << std::endl;
        append_to_log_file(std::string_view("[WARNING]: [" + owner_name + "]: " + formatted_msg));
    }
    template <typename... Args>
    inline void Logger::log_error(const std::string &message, Args... args) const
    {
        const std::string formatted_msg = std::vformat(message, std::make_format_args(args...));
        std::cout << "[ERROR]: [" << this->owner_name << "]: " << formatted_msg << std::endl;
        append_to_log_file(std::string_view("[ERROR]: [" + owner_name + "]: " + formatted_msg));
    }
    template <typename... Args>
    inline void Logger::log_debug(const std::string &message, Args... args) const
    {
        const std::string formatted_msg = std::vformat(message, std::make_format_args(args...));
#ifdef DEBUG
        append_to_log_file(std::string_view("[DEBUG]: [" + owner_name + "]: " + formatted_msg));
        std::cout << "[DEBUG]: [" << this->owner_name << "]: " << formatted_msg << std::endl;
        return;
#endif
        append_to_log_file(std::string_view("[DEBUG]: [" + owner_name + "]: " + formatted_msg + " | (NOPRINT)"));
    }
}

#endif