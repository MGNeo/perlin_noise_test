#include "Mode.hpp"

using namespace perlin_noise;

Mode::~Mode()
{
}

OverflowControl::OverflowControl(const bool _overflow_control)
	:
	overflow_control{ _overflow_control }
{
}

SumMode::SumMode(const bool _overflow_control)
	:
	OverflowControl{ _overflow_control }
{
}

float SumMode::blend(const float _first,
				     const float _second)
{
	const float result = _first + _second;

	if (result > 1.f)
	{
		if (overflow_control == true)
		{
			return 1.f;
		}
	}

	return result;
}

SubMode::SubMode(const bool _overflow_control)
	:
	OverflowControl{ _overflow_control }
{
}

float SubMode::blend(const float _first,
					 const float _second)
{
	const float result = _first - _second;

	if (result < -1.f)
	{
		if (overflow_control == true)
		{
			return -1.f;
		}
	}

	return result;
}