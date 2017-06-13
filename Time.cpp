#include "stdafx.h"

#include "Time.h"


std::chrono::duration<double> Time::m_deltaTime;
std::chrono::time_point<std::chrono::system_clock> Time::m_prevTime;
std::chrono::time_point<std::chrono::system_clock> Time::m_currTime;

void Time::StartTime()
{
	m_currTime = std::chrono::system_clock::now();
}

float Time::GetDeltaTime()
{
	return m_deltaTime.count();
}

void Time::Update()
{
	m_prevTime = m_currTime;
	m_currTime = std::chrono::system_clock::now();
	m_deltaTime = m_currTime - m_prevTime;
}