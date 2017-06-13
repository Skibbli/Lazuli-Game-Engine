#ifndef TIME_H
#define TIME_H

#include "stdafx.h"

#include <chrono>

/// Static class used for getting the delta time between frames
class Time
{
public:
	/// Starts the clock
	static void StartTime();

	/// Returns the time since the last frame
	static float GetDeltaTime();
	/// Updates the delta time value
	static void Update();

private:
	/// The time since the last frame
	static std::chrono::duration<double> m_deltaTime;
	static std::chrono::time_point<std::chrono::system_clock> m_prevTime;
	static std::chrono::time_point<std::chrono::system_clock> m_currTime;
};

#endif