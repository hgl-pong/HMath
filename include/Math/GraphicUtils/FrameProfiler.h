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
	}
}