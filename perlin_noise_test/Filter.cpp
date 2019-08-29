#include "Filter.hpp"
#include "Source.hpp"

#include <stdexcept>

using namespace std;

Filter::~Filter()
{
}

PosterizationFilter::PosterizationFilter(const size_t _levels)
	:
	levels{ _levels }
{
	if (levels <= 1U)
	{
		throw invalid_argument("PosterizationFilter::PosterizationFilter(), levels <= 1U");
	}
}

void PosterizationFilter::apply(Source &_source) const
{
	const size_t source_width = _source.getWidth();
	const size_t source_height = _source.getHeight();

	// Find the max and the min values.
	float min_value = MAX_SOURCE_VALUE;
	float max_value = MIN_SOURCE_VALUE;
	for (size_t x = 0; x < source_width; ++x)
	{
		for (size_t y = 0; y < source_height; ++y)
		{
			const float value = _source.getPoint(x, y);
			if (value < min_value) min_value = value;
			if (value > max_value) max_value = value;
		}
	}

	// Apply the filter.
	const float range = max_value - min_value;

	const float step_value = range / levels;
	const float half_step_value = step_value / 2.f;

	for (size_t x = 0; x < source_width; ++x)
	{
		for (size_t y = 0; y < source_height; ++y)
		{
			const float value = _source.getPoint(x, y);
			
			const float inter_value = value - min_value;

			const float new_value = min_value + step_value * floor(inter_value / step_value);

			_source.setPoint(x, y, new_value);
		}
	}
}