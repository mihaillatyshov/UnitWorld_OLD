#pragma once

#include <chrono>
#include "Debug/ConsoleLog.h"

class Timer
{
public:
	Timer() 
	{
		m_StartTimepoint = std::chrono::high_resolution_clock::now();
	}

	~Timer()
	{
		auto endTimepoint = std::chrono::high_resolution_clock::now();

		auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
		auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

		auto duration = end - start;
		double ms = duration * 0.001;
		m_DeltaTime = ms * 0.001;
		m_IntermediateTime += ms;
		m_IntermediateFPS++;
		if (m_IntermediateTime > 1000.0)
		{
			m_FPS = m_IntermediateFPS;
			LOGI("FPS: ", m_FPS);
			LOGD();
			m_IntermediateTime = 0.0;
			m_IntermediateFPS = 0;
		}
	}

	static void Reset()
	{
		m_DeltaTime = 0.0;
		m_FPS = 0;
	}

	static inline double GetDeltaTime() { return m_DeltaTime; }
	static inline int GetFPS() { return m_FPS; }
protected:
	static inline std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
	// ms
	static inline double m_DeltaTime = 0.0;
	static inline double m_IntermediateTime = 0.0;
	static inline int m_IntermediateFPS = 0;
	static inline int m_FPS = 0;

};



class TestTimer
{
public:
	TestTimer() 
	{
		m_StartTimepoint = std::chrono::high_resolution_clock::now();
	}

	~TestTimer()
	{
		auto endTimepoint = std::chrono::high_resolution_clock::now();

		auto start = std::chrono::time_point_cast<std::chrono::nanoseconds>(m_StartTimepoint).time_since_epoch().count();
		auto end = std::chrono::time_point_cast<std::chrono::nanoseconds>(endTimepoint).time_since_epoch().count();

		auto duration = end - start;
		double ms = duration * 0.000001;
		printf("%f sec\n", ms);
	}
protected:
	std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;

};


class TimerStartEnd
{
public:

	
	void Start()
	{
		m_StartTimepoint = std::chrono::high_resolution_clock::now();
	}

	void End()
	{
		auto endTimepoint = std::chrono::high_resolution_clock::now();

		auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
		auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

		auto duration = end - start;
		double ms = duration * 0.001;
		printf("%f\n", ms);
	}
protected:
	std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;

};