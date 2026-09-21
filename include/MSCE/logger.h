#ifndef MSCE_LOGGER_H_
#define MSCE_LOGGER_H_
#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <format>

namespace msce
{
    /**
     * @brief Standart logger. Use it!
     * @note It creates a logfile at [your OS temp dir]/msce_log.txt and writes all logs into it.
     */
    class Logger
    {
    private:
        /// @brief The name of owner of this instance. Despite scary name, it's just what gets printed inside [].
        std::string owner_name_;
        /// @brief The log file ofstream instance.
        inline static std::ofstream log_file = std::ofstream(std::filesystem::temp_directory_path() / "msce_log.txt");

        /**
         * @brief Appends the message into logfile.
         * @note Note that it adds log time at the beginning.
         */
        void append_to_log_file(const std::string_view &message) const noexcept;

    public:
        /// @brief The goto constructor.
        /// @param owner_name What gets printed in [].
        Logger(const std::string &owner_name);

        /// @brief Deleted copy constructor.
        Logger(const Logger &other) = default;
        /// @brief Why would I document deconstructor!
        ~Logger();

        /**
         * @brief Logs formatted message as [INFO].
         * @param message Pre-formatted message.
         * @param args Args that get passed into std::format.
         */
        template <typename... Args>
        void log_info(const std::string &message, Args... args) const;
        /**
         * @brief Logs formatted message as [WARNING].
         * @param message Pre-formatted message.
         * @param args Args that get passed into std::format.
         */
        template <typename... Args>
        void log_warning(const std::string &message, Args... args) const;
        /**
         * @brief Logs formatted message as [ERROR].
         * @param message Pre-formatted message.
         * @param args Args that get passed into std::format.
         */
        template <typename... Args>
        void log_error(const std::string &message, Args... args) const;
        template <typename... Args>
        /**
         * @brief Logs formatted message as [DEBUG].
         * @param message Pre-formatted message.
         * @param args Args that get passed into std::format.
         * @note Note that debug messages are printed into console only in DEBUG build type. They still get written into the logfile tho.
         */
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