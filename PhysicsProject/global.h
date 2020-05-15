#pragma once

namespace global {
	constexpr double screen_width = 1400;
	constexpr double screen_height = 1000;
	constexpr double pi = 3.14159265359;

	constexpr double to_radians(double angle)
	{
		return angle * pi / 180;
	}
};