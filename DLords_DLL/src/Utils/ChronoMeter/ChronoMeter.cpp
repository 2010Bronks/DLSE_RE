#include "precompiled.hpp"

void ChronoMeter::reset()
{
    start = std::chrono::high_resolution_clock::now();
}

long long ChronoMeter::elapsed() const
{
    auto now = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - start);
    return duration.count();
}

bool ChronoMeter::empty() const
{
    return start.time_since_epoch().count() == 0;
}

std::string ChronoMeter::to_string() const
{
    if (empty())
        return "0.0ms";

    auto duration_ms = elapsed();
    std::stringstream ss;

    if (duration_ms < 1000)
    {
        ss << duration_ms << "ms";
    }
    else if (duration_ms < 60000)
    {
        ss << std::fixed << std::setprecision(1) << duration_ms / 1000.0 << "s";
    }
    else
    {
        ss << std::fixed << std::setprecision(1) << duration_ms / 60000.0 << "min";
    }

    return ss.str();
}