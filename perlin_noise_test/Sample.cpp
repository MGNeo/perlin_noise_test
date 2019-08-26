#include "Sample.hpp"

#include <stdexcept>
#include <random>

using namespace perlin_noise;
using namespace std;
using namespace sf;

Sample::Sample(const size_t _width,
			   const size_t _height)
{
	resizeGrid(_width, _height);
}

void Sample::resizeGrid(const size_t _width,
						const size_t _height)
{
	if (_width <= 1)
	{
		throw invalid_argument("Sample::resizeGrid(), _width <= 1");
	}

	if (_height <= 1)
	{
		throw invalid_argument("Sample::resizeGrid(), _height <= 1");
	}

	// Exception safety.
	unique_ptr<unique_ptr<Vector2f[]>[]> new_points;
	new_points = make_unique<unique_ptr<Vector2f[]>[]>(_width);
	for (size_t w = 0; w < _width; ++w)
	{
		new_points[w] = make_unique<Vector2f[]>(_height);
	}
	
	swap(points, new_points);
	width = _width;
	height = _height;

	resetGrid();
}

void Sample::resetGrid()
{
	default_random_engine random_engine(time(NULL) + clock());

	uniform_real_distribution<float> angle_rad_distribution(0.f, 2.f);
	uniform_real_distribution<float> value_distribution(0.f, 1.f);

	for (size_t w = 0; w < width; ++w)
	{
		for (size_t h = 0; h < height; ++h)
		{
			const float angle_grad = angle_rad_distribution(random_engine);
			const float value = value_distribution(random_engine);

			points[w][h].x = value * cos(angle_grad);
			points[w][h].x = value * sin(angle_grad);
		}
	}
}

const Vector2f &Sample::getPoint(const size_t _x,
								 const size_t _y) const
{
	if (_x >= width)
	{
		throw invalid_argument("const Vector2f &Sample::getPoint(), _x >= width");
	}

	if (_y >= height)
	{
		throw invalid_argument("const Vector2f &Sample::getPoint(), _y >= height");
	}

	return points[_x][_y];
}

size_t Sample::getWidth() const
{
	return width;
}

size_t Sample::getHeight() const
{
	return height;
}