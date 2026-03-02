#include "timeSystem.h"

using namespace std::chrono;
using namespace msce;

double TimeSystem::get_delta_time() const
{
    auto delta_time = duration_cast<nanoseconds>(
        high_resolution_clock::now() - this->_last_frame_time);

    // convert to millis and then to seconds: / (1,000 * 1,000,000)
    return static_cast<double>(delta_time.count()) / 1000000000.0;
}

high_resolution_clock::time_point TimeSystem::get_last_frame_time() const
{
    return this->_last_frame_time;
}

high_resolution_clock::time_point TimeSystem::get_start_time() const
{
    return this->_start_time;
}

double TimeSystem::get_total_millis() const
{
    return get_total_nanos() / 1000000.0;
}

unsigned long TimeSystem::get_total_nanos() const
{
    return _nanos_since_start;
}

double TimeSystem::get_total_seconds() const
{
    return static_cast<double>(this->get_total_millis()) / 1000.0;
}

void TimeSystem::init()
{
    this->_start_time = high_resolution_clock::now();
    this->_last_frame_time = high_resolution_clock::now();
    this->_nanos_since_start = 0;
}

void TimeSystem::update(double delta_time)
{
    this->_last_frame_time = high_resolution_clock::now();
    this->_nanos_since_start = duration_cast<nanoseconds>(
                                   high_resolution_clock::now() - this->_start_time)
                                   .count();
}