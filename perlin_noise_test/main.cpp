#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;
using namespace std;

const float PI = 3.141592653589793238463f;
const float GRAD_TO_RAD_K = 180U / PI;

const size_t WINDOW_SIZE = 800U;

const size_t POINT_COUNT = 11;
const size_t CELL_SIZE = WINDOW_SIZE / (POINT_COUNT - 1);
const float sqrt_2 = sqrt(2.f);

class PerlinNoise
{
public:
	PerlinNoise()
	{
		Vector2f points[POINT_COUNT][POINT_COUNT];
		// Create the Perlin grid;
		for (size_t point_x = 0; point_x < POINT_COUNT; ++point_x)
		{
			for (size_t point_y = 0; point_y < POINT_COUNT; ++point_y)
			{
				const float angle_grad = rand() % 360U;
				const float angle_rad = angle_grad * GRAD_TO_RAD_K;
				const float v = 1;// (rand() % 100) / 100.f;
				points[point_x][point_y].x = v * cos(angle_rad);
				points[point_x][point_y].y = v * sin(angle_rad);
			}
		}
Vector2f range{ 0.f, 0.f };
		// Rasterization.
		Image image;
		image.create(WINDOW_SIZE, WINDOW_SIZE, Color::Black);
		for (size_t point_x = 0; point_x < POINT_COUNT - 1U; ++point_x)
		{
			for (size_t point_y = 0; point_y < POINT_COUNT - 1U; ++point_y)
			{
				const Vector2f &top_left_gradient     = points[point_x    ][point_y    ];
				const Vector2f &top_right_gradient    = points[point_x + 1][point_y    ];
				const Vector2f &bottom_left_gradient  = points[point_x    ][point_y + 1];
				const Vector2f &bottom_right_gradient = points[point_x + 1][point_y + 1];

				for (size_t pixel_x = 0; pixel_x < CELL_SIZE; ++pixel_x)
				{
					for (size_t pixel_y = 0; pixel_y < CELL_SIZE; ++pixel_y)
					{
						const float local_x = (pixel_x + 0.5f) / (float)CELL_SIZE;
						const float local_y = (pixel_y + 0.5f) / (float)CELL_SIZE;

						Vector2f vector_to_top_left     = { local_x    , local_y     };
						Vector2f vector_to_top_right    = { local_x - 1, local_y     };
						Vector2f vector_to_bottom_left  = { local_x    , local_y - 1 };
						Vector2f vector_to_bottom_right = { local_x - 1, local_y - 1 };

						auto local_normalize = [](Vector2f &_v)
						{
							_v.x /= sqrt_2;
							_v.y /= sqrt_2;

							//_v.x *= 2;
							//_v.y *= 2;
						};

						local_normalize(vector_to_top_left);
						local_normalize(vector_to_top_right);
						local_normalize(vector_to_bottom_left);
						local_normalize(vector_to_bottom_right);

						const float top_left_value     = top_left_gradient.x     * vector_to_top_left.x     + top_left_gradient.y     * vector_to_top_left.y    ;
						const float top_right_value    = top_right_gradient.x    * vector_to_top_right.x    + top_right_gradient.y    * vector_to_top_right.y   ;
						const float bottom_left_value  = bottom_left_gradient.x  * vector_to_bottom_left.x  + bottom_left_gradient.y  * vector_to_bottom_left.y ;
						const float bottom_right_value = bottom_right_gradient.x * vector_to_bottom_right.x + bottom_right_gradient.y * vector_to_bottom_right.y;
			

						auto interpolator = [](const float _t)->float
						{
							return _t *_t *_t *(_t * (_t * 6 - 15) + 10);
						};

						const float top_value    = top_left_value    + interpolator(local_x) * (top_right_value    - top_left_value   );
						const float bottom_value = bottom_left_value + interpolator(local_x) * (bottom_right_value - bottom_left_value);

						const float value = top_value + interpolator(local_y) * (bottom_value - top_value);

			 if (value < range.x) range.x = value;
			 if (value > range.y) range.y = value;
			 
			 // Почему max fabs value < max fabs top_left_value???? В два раза!

						Uint8 brightness = 128 + value * 127;

						const size_t x = point_x * CELL_SIZE + pixel_x;
						const size_t y = point_y * CELL_SIZE + pixel_y;

						image.setPixel(x, y, Color(brightness, brightness, brightness, brightness));
					}
				}
			}
		}

		cout << range.x << " " << range.y << endl;

		texture.loadFromImage(image);
		sprite.setTexture(texture, true);
	}

	void draw(RenderWindow &_render_window)
	{
		_render_window.draw(sprite);
	}

private:
	Texture texture;
	Sprite sprite;
};

int main(int argc, char **argv)
{
	//srand(time(NULL));

	RenderWindow render_window{ VideoMode(WINDOW_SIZE, WINDOW_SIZE), "Perlin Noise Test", Style::Close };

	PerlinNoise perlin_noise;

	while (render_window.isOpen() == true)
	{
		Event event;
		while (render_window.pollEvent(event) == true)
		{
			if (event.type == Event::Closed)
			{
				render_window.close();
			}
		}

		render_window.clear();
		perlin_noise.draw(render_window);
		render_window.display();
	}

	return 0;
}