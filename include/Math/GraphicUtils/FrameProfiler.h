#pragma once
#include <Math/Math.h>
#include <chrono>
#if _DEBUG
#define USE_FRAME_PROFILER
#endif

#ifdef USE_FRAME_PROFILER
namespace MathLib
{
	namespace GraphicUtils
	{
		class FrameProfiler
		{
		public:
		public:
			void Start()
			{
				m_start = std::chrono::steady_clock::now();
			};
			void End()
			{
				m_end = std::chrono::steady_clock::now();
			};
			HReal GetFrameTime()
			{
				return std::chrono::duration_cast<std::chrono::milliseconds>(m_end - m_start).count();
			};

			HReal GetFrameRate()
			{
				return 1000.0f / GetFrameTime();
			};

		private:
			std::chrono::time_point<std::chrono::steady_clock> m_start;
			std::chrono::time_point<std::chrono::steady_clock> m_end;
		};

		static FrameProfiler g_FrameProfiler;
		class FrameProfilerTimer
		{
		public:
			FrameProfilerTimer(const char *info)
			{
				m_info = info;
				m_start = std::chrono::steady_clock::now();
			};
			~FrameProfilerTimer()
			{
				auto end = std::chrono::steady_clock::now();
				auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - m_start).count();
				printf("%s cost time: %d ms\n", m_info.c_str(), duration);
			}

		private:
			std::string m_info;
			std::chrono::time_point<std::chrono::steady_clock> m_start;
		};
	}
}

#define PROFILE_TIMER(info) MathLib::GraphicUtils::FrameProfilerTimer profilerTimer(info)
#define PROFILE_FRAME_START() MathLib::GraphicUtils::g_FrameProfiler.Start()
#define PROFILE_FRAME_END() MathLib::GraphicUtils::g_FrameProfiler.End()
#define PROFILE_FRAME_RATE() MathLib::GraphicUtils::g_FrameProfiler.GetFrameRate()
#define PROFILE_FRAME_TIME() MathLib::GraphicUtils::g_FrameProfiler.GetFrameTime()
#else
#define PROFILE_TIMER(info)
#define PROFILE_FRAME_START()
#define PROFILE_FRAME_END()
#define PROFILE_FRAME_RATE()
#define PROFILE_FRAME_TIME()
#endif