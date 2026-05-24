#ifndef _MSCE_TIME_SYSTEM_H_
#define _MSCE_TIME_SYSTEM_H_
#include <chrono>
#include <MSCE/ECS/system.h>
namespace msce
{
    class TimeSystem : public System
    {
    private:
        /// @brief Nanoseconds since start of program.
        unsigned long nanos_since_start_ = 0;
        /// @brief Nanoseconds since start of program.
        double millis_since_start_ = 0;
        /// @brief Nanoseconds since start of program.
        double seconds_since_start_ = 0;

        /// @brief Amount of seconds that have passed since last frame.
        double delta_seconds_ = 0;

        /// @brief Consistently sets all time units from nanoseconds.
        void set_all_units_from_nanos(unsigned long nanos);

    public:
        void init() override;
        void update(double delta_seconds) override;

        /// @brief Returns SECONDS that have passed since last frame. (Delta T)
        double get_delta_seconds() const;

        /// @brief Returns SECONDS that have passed since program start. (runtime)
        double get_total_seconds() const;

        /// @brief Returns MILLISECONDS that have passed since program start. (runtime)
        double get_total_millis() const;

        /// @brief Returns NANOSECONDS that have passed since program start. (runtime)
        unsigned long get_total_nanos() const;
    };
}
MSCE_REGISTER_SYSTEM(msce::TimeSystem, TimeSystem)

#endif