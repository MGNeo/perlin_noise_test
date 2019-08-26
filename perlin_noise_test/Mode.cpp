#include "Mode.hpp"
#include <stdexcept>

using namespace std;
using namespace perlin_noise;

Mode::Mode(const bool _overflow_control = true,
	       const float _alpha = 1.f)
	:
	overflow_control{ _overflow_control },
	alpha{ _alpha }
{
	if (alpha < 0.f)
	{
		throw invalid_argument("Move::Mode(), _alpha < 0.f");
	}

	if (alpha > 1.f)
	{
		throw invalid_argument("Move::Mode(), _alpha > 1.f");
	}
}

Mode::~Mode()
{
}

SumMode::SumMode(const bool _overflow_control,
	             const float _alpha)
	:
	Mode{ _overflow_control, _alpha }
{
}

float SumMode::blend(const float _first,
				     const float _second)
{
	const float result = _first + alpha * _second;

	if (result > 1.f)
	{
		if (overflow_control == true)
		{
			return 1.f;
		}
	}

	return result;
}

SubMode::SubMode(const bool _overflow_control,
	             const float _alpha)
	:
	Mode{ _overflow_control, _alpha }
{
}

float SubMode::blend(const float _first,
					 const float _second)
{
	const float result = _first - (alpha * _second);

	if (result < -1.f)
	{
		if (overflow_control == true)
		{
			return -1.f;
		}
	}

	return result;
}


MulMode::MulMode(const bool _overflow_control,
	             const float _alpha)
	:
	Mode{ _overflow_control, _alpha }
{
}

float MulMode::blend(const float _first,
	                 const float _second)
{
	const float result = _first * (alpha * _second);

	if (result < -1.f)
	{
		if (overflow_control == true)
		{
			return -1.f;
		}
	}

	return result;
}

DevMode::DevMode(const bool _overflow_control,
	             const float _alpha)
	:
	Mode{ _overflow_control, _alpha }
{
}

float DevMode::blend(const float _first,
	                const float _second)
{
	const float d = (alpha * _second);

	float result;

	if (d == 0.f)
	{
		result = 0.f;
	}
	else {
		result = _first / d;
	}

	if (overflow_control == true)
	{
		if (result > 1.f)// Он ведь может быть и отрицательным...
		{
			return 1.f;
		}
		else {
			if (result < 0.f)
			{
				return 0.f;
			}

		}
	}

	return result;
}