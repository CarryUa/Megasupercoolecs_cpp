#ifndef _MSCE_TIME_SYSTEM_H_
#define _MSCE_TIME_SYSTEM_H_
#include <chrono>
#include <MSCE/ECS/system.h>
namespace msce
{
    class TimeSystem : public System
    {
    private:
        /// @brief chrono::time_point of program start.
        std::chrono::high_resolution_clock::time_point _start_time;
        /// @brief chrono::time_point of last frame.
        std::chrono::high_resolution_clock::time_point _last_frame_time;
        /// @brief Nanoseconds since start of the program.
        unsigned long _nanos_since_start;

    public:
        void init() override;
        void update(double delta_time) override;

        double get_delta_time() const;
        double get_total_seconds() const;
        double get_total_millis() const;
        unsigned long get_total_nanos() const;

        std::chrono::high_resolution_clock::time_point get_start_time() const;
        std::chrono::high_resolution_clock::time_point get_last_frame_time() const;
    };
}

#endif