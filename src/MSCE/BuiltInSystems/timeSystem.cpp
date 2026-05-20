#include "timeSystem.h"

#define TIME_POINT_NOW() high_resolution_clock::now()

using namespace std::chrono;
using namespace msce;

namespace
{
    const high_resolution_clock::time_point PROGRAM_START_TIME_POINT = TIME_POINT_NOW();
}

void msce::TimeSystem::set_all_units_from_nanos(unsigned long nanos)
{
    this->nanos_since_start_ = nanos;
    this->millis_since_start_ = static_cast<double>(nanos) / 1.0E6;
    this->seconds_since_start_ = static_cast<double>(nanos) / 1.0E9;
}

void msce::TimeSystem::init()
{
    this->set_all_units_from_nanos(duration_cast<nanoseconds>(TIME_POINT_NOW() - PROGRAM_START_TIME_POINT).count());
}

void msce::TimeSystem::update(double delta_time)
{
    auto prev_total_sec = this->seconds_since_start_;

    this->set_all_units_from_nanos(duration_cast<nanoseconds>(TIME_POINT_NOW() - PROGRAM_START_TIME_POINT).count());
    this->delta_seconds_ = this->seconds_since_start_ - prev_total_sec;
}

double msce::TimeSystem::get_delta_seconds() const
{
    return this->delta_seconds_;
}

double msce::TimeSystem::get_total_seconds() const
{
    return this->seconds_since_start_;
}

double msce::TimeSystem::get_total_millis() const
{
    return this->millis_since_start_;
}

unsigned long msce::TimeSystem::get_total_nanos() const
{
    return this->nanos_since_start_;
}

#undef TIME_POINT_NOW