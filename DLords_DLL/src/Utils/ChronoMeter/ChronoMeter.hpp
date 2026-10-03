#pragma once

#include <chrono>

class ChronoMeter
{
public:
    /**
     * Default constructor for the ChronoMeter class. Initializes the start time to the current time
     * using the high resolution clock. This effectively starts the timing measurement from the point
     * of object creation.
     */
    ChronoMeter()
    {
        start = std::chrono::high_resolution_clock::now();
    }

    /**
     * Constructs a ChronoMeter with an uninitialized start time. This constructor takes a nullptr_t
     * as a parameter to differentiate it from the default constructor. The start time is initialized
     * to a default, epoch value of the high resolution clock, indicating that the ChronoMeter has not
     * yet started.
     *
     * @param nullp A std::nullptr_t argument used to select this constructor, allowing the creation
     *              of a ChronoMeter object without automatically starting the timing.
     */
    ChronoMeter(std::nullptr_t)
    {
        start = std::chrono::time_point<std::chrono::high_resolution_clock>();
    }

    /**
     * Resets the ChronoMeter's start time to the current high resolution clock time.
     * This method is used to restart the timing measurements from the current point in time.
     */
    void reset();

    /**
     * Calculates the elapsed time in milliseconds since the ChronoMeter was last reset.
     * This method returns the time duration as a long long integer representing the
     * number of milliseconds that have elapsed.
     *
     * @return The elapsed time in milliseconds as a long long.
     */
    long long elapsed() const;

    /**
     * Checks if the ChronoMeter has been started or is in a reset state.
     * This method returns true if the ChronoMeter has never been started or has been
     * reset and not yet started again. It essentially checks if the start time is
     * at the epoch of the clock being used.
     *
     * @return A boolean indicating if the ChronoMeter is in a reset state (true) or has been started (false).
     */
    bool empty() const;

    /**
     * Converts the elapsed time into a human-readable string format.
     * This method provides a convenient way to display the elapsed time. If no time has elapsed
     * or the ChronoMeter is in a reset state, it returns "0.0ms". Otherwise, it formats the time
     * into milliseconds, seconds, or minutes depending on the duration, with appropriate units.
     *
     * @return A string representing the elapsed time with units.
     */
    std::string to_string() const;

    /**
     * Checks if a specified duration has elapsed since the ChronoMeter was last reset.
     * This template method allows for checking against various duration types (e.g., seconds, milliseconds).
     * It compares the elapsed time since the last reset with the specified duration to determine
     * if the specified duration has passed.
     *
     * @param duration The duration to compare against, specified in any std::chrono duration type (e.g., std::chrono::seconds, std::chrono::milliseconds).
     * @return A boolean indicating if the specified duration has elapsed (true) or not (false).
     *
     * Usage example:
     *   if (chronoMeter.has_elapsed(5s)) {
     *     // Code to execute if 5 seconds have elapsed since the last reset
     *   }
     */
    template<typename DurationType>
    bool has_elapsed(DurationType duration) const
    {
        auto now = std::chrono::high_resolution_clock::now();
        auto elapsed = now - start;
        return elapsed >= duration;
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
};