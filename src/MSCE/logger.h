#ifndef MSCE_LOGGER_H_
#define MSCE_LOGGER_H_
#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <format>

namespace msce
{
    class Logger
    {
    private:
        std::string owner_name_;
        inline static std::ofstream log_file = std::ofstream(std::filesystem::temp_directory_path() / "msce_log.txt");

        void append_to_log_file(const std::string_view &message) const noexcept;

    public:
        Logger(const std::string &owner_name);
        Logger(const Logger &other) = default;
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

    template <typename... Args>
    inline void Logger::log_info(const std::string &message, Args... args) const
    {
        const std::string formatted_msg = std::vformat(message, std::make_format_args(args...));
        std::cout << "[INFO]: [" << this->owner_name_ << "]: " << formatted_msg << std::endl;
        append_to_log_file(std::string_view("[INFO]: [" + owner_name_ + "]: " + formatted_msg));
    }

    template <typename... Args>
    inline void Logger::log_warning(const std::string &message, Args... args) const
    {
        const std::string formatted_msg = std::vformat(message, std::make_format_args(args...));
        std::cout << "[WARNING]: [" << this->owner_name_ << "]: " << formatted_msg << std::endl;
        append_to_log_file(std::string_view("[WARNING]: [" + owner_name_ + "]: " + formatted_msg));
    }
    template <typename... Args>
    inline void Logger::log_error(const std::string &message, Args... args) const
    {
        const std::string formatted_msg = std::vformat(message, std::make_format_args(args...));
        std::cout << "[ERROR]: [" << this->owner_name_ << "]: " << formatted_msg << std::endl;
        append_to_log_file(std::string_view("[ERROR]: [" + owner_name_ + "]: " + formatted_msg));
    }
    template <typename... Args>
    inline void Logger::log_debug(const std::string &message, Args... args) const
    {
        const std::string formatted_msg = std::vformat(message, std::make_format_args(args...));
        append_to_log_file(std::string_view("[DEBUG]: [" + owner_name_ + "]: " + formatted_msg));
#ifdef DEBUG
        std::cout << "[DEBUG]: [" << this->owner_name_ << "]: " << formatted_msg << std::endl;
        return;
#endif
    }

}

#endif