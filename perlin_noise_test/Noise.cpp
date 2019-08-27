#include "Noise.hpp"
#include "Source.hpp"

#include <stdexcept>
#include <random>
#include <time.h>

using namespace std;

const float PI = 3.141592f;
const float PI_2 = PI * 2.f;
const float SQRT_2 = sqrt(2.f);

Noise::~Noise()
{
}

PerlinNoise::PerlinNoise(const size_t _width,
						 const size_t _height)
{
	resize(_width, _height);
}

// Resize (and reset).
void PerlinNoise::resize(const size_t _width,
						 const size_t _height)
{
	if (_width <= 2U)
	{
		throw invalid_argument("PerlinNoise::resize(), _width <= 2U");
	}

	if (_height <= 2U)
	{
		throw invalid_argument("PerlinNoise::resize(), _height <= 2U");
	}

	default_random_engine dre{ static_cast<unsigned int>(time(NULL)) };
	uniform_real_distribution<float> angle_distribution{ 0.f, PI_2 };

	// Exception safety.
	unique_ptr<unique_ptr<Vector2f[]>[]> new_points;
	new_points = make_unique<unique_ptr<Vector2f[]>[]>(_width);
	for (size_t x = 0; x < _width; ++x)
	{
		new_points[x] = make_unique<Vector2f[]>(_height);
		for (size_t y = 0; y < _height; ++y)
		{
			const float angle_rad = angle_distribution(dre);
			new_points[x][y].x = cos(angle_rad);
			new_points[x][y].y = sin(angle_rad);
		}
	}

	points.swap(new_points);
	width = _width;
	height = _height;
}

void PerlinNoise::reset()
{
	default_random_engine dre{ static_cast<unsigned int>(time(NULL)) };
	uniform_real_distribution<float> angle_distribution{ 0.f, PI_2 };

	for (size_t x = 0; x < width; ++x)
	{
		for (size_t y = 0; y < height; ++y)
		{
			const float angle_rad = angle_distribution(dre);
			points[x][y].x = cos(angle_rad);
			points[x][y].y = sin(angle_rad);
		}
	}
}

void PerlinNoise::generate(Source &_source) const
{
	const size_t cw = width  - 1U;
	const size_t pw = _source.getWidth();

	if (pw <= cw)
	{
		throw invalid_argument("PerlinNoise::generate(), pw <= cw");
	}

	const size_t ch = height - 1U;
	const size_t ph = _source.getHeight();

	if (ph <= ch)
	{
		throw invalid_argument("PerlinNoise::generate(), ph <= ch");
	}

	const size_t pcw = static_cast<size_t>(static_cast<float>(pw) / cw/* + 0.5f*/);
	const size_t pch = static_cast<size_t>(static_cast<float>(ph) / ch/* + 0.5f*/);
	const size_t pcd = static_cast<size_t>(sqrt(pow(pcw, 2) + pow(pch, 2)));

	for (size_t cell_x = 0; cell_x < cw; ++cell_x)
	{
		for (size_t cell_y = 0; cell_y < ch; ++cell_y)
		{
			const Vector2f &top_left_cell     = points[cell_x    ][cell_y    ];
			const Vector2f &top_right_cell    = points[cell_x + 1][cell_y    ];
			const Vector2f &bottom_left_cell  = points[cell_x    ][cell_y + 1];
			const Vector2f &bottom_right_cell = points[cell_x + 1][cell_y + 1];

			for (size_t point_x = 0; point_x < pcw; ++point_x)
			{
				const float normed_point_x = (point_x + 0.5f) / pcw;
				for (size_t point_y = 0; point_y < pch; ++point_y)
				{
					const float normed_point_y = (point_y + 0.5f) / pch;

					Vector2f vector_to_top_left    { normed_point_x      , normed_point_y       };
					Vector2f vector_to_top_right   { normed_point_x - 1.f, normed_point_y       };
					Vector2f vector_to_bottom_left { normed_point_x      , normed_point_y - 1.f };
					Vector2f vector_to_bottom_right{ normed_point_x - 1.f, normed_point_y - 1.f };

					auto normalize_by_d = [](Vector2f &_v)
					{
						_v.x /= SQRT_2;
						_v.y /= SQRT_2;

						/*
						_v.x *= 2;
						_v.y *= 2;
						*/
						
					};

					normalize_by_d(vector_to_top_left);
					normalize_by_d(vector_to_top_right);
					normalize_by_d(vector_to_bottom_left);
					normalize_by_d(vector_to_bottom_right);

					// Dot product.
					const float top_left_value     = top_left_cell.x     * vector_to_top_left.x     + top_left_cell.y     * vector_to_top_left.y    ;
					const float top_right_value    = top_right_cell.x    * vector_to_top_right.x    + top_right_cell.y    * vector_to_top_right.y   ;
					const float bottom_left_value  = bottom_left_cell.x  * vector_to_bottom_left.x  + bottom_left_cell.y  * vector_to_bottom_left.y ;
					const float bottom_right_value = bottom_right_cell.x * vector_to_bottom_right.x + bottom_right_cell.y * vector_to_bottom_right.y;

					auto curve_fixer = [](const float _value)
					{
						return _value * _value * _value * (_value * (_value * 6 - 15) + 10);
					};

					const float top_value    = top_left_value    + curve_fixer(normed_point_x) * (top_right_value - top_left_value      );
					const float bottom_value = bottom_left_value + curve_fixer(normed_point_x) * (bottom_right_value - bottom_left_value);
					float value              = top_value         + curve_fixer(normed_point_y) * (bottom_value - top_value              );

					value = (1.f + value) / 2.f;

					const float source_x = cell_x * pcw + point_x;
					const float source_y = cell_y * pch + point_y;

					_source.setPoint(source_x, source_y, value);
				}
			}

		}
	}
}