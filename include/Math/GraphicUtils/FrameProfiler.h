#pragma once
#include <Math/Math.h>
#include <chrono>

namespace MathLib
{
	namespace GraphicUtils
	{
		class FrameProfiler
		{
		public:
			class Timer 
			{
			public:
				Timer(const char* info)
				{
					m_info = info;
					m_start = std::chrono::steady_clock::now();
				};
				~Timer()
				{
					auto end = std::chrono::steady_clock::now();
					auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - m_start).count();
					printf("%s cost time: %d ms\n", m_info.c_str(), duration);
				}
			private:
				std::string m_info;
				std::chrono::time_point<std::chrono::steady_clock> m_start;
			};
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

			std::unique_ptr<Timer> GetTimer(const char* info)
			{
				return std::make_unique<Timer>(info);
			}
		private:
			std::chrono::time_point<std::chrono::steady_clock> m_start;
			std::chrono::time_point<std::chrono::steady_clock> m_end;
		};
	}
}

#define PROFILE_FRAME(info) MathLib::GraphicUtils::FrameProfiler::Timer timer(info);