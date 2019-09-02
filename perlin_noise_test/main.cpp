#include <SFML/Graphics.hpp>
#include <iostream>// DEBUG
#include <memory>

#include "Source.hpp"
#include "Noise.hpp"
#include "Filter.hpp"
#include "Mixer.hpp"

using namespace sf;
using namespace std;

const size_t WINDOW_SIZE = 512U;

// TODO: Fix all type conversions.

int main(int argc, char **argv)
{
	RenderWindow render_window{ VideoMode(WINDOW_SIZE, WINDOW_SIZE), "Perlin Noise Test", Style::Close };

	Source source_1{ WINDOW_SIZE, WINDOW_SIZE };
	Source source_2{ WINDOW_SIZE, WINDOW_SIZE };
	Source source_3{ WINDOW_SIZE, WINDOW_SIZE };
	Source source_4{ WINDOW_SIZE, WINDOW_SIZE };
	Source source_5{ WINDOW_SIZE, WINDOW_SIZE };

	PerlinNoise perlin_noise_1 { 4U, 4U, false };
	PerlinNoise perlin_noise_2 { 16U, 16U, false };
	PerlinNoise perlin_noise_3 { 64U, 64U, false };
	
	CustomPosterizationFilter custom_posterization_filter{ 0.1f, 0.2f, 0.6f, 0.85f, 1.f};

	Image image;
	image.create(source_5.getWidth(), source_5.getHeight(), Color::Black);

	Texture texture;

	Sprite sprite;
	
	while (render_window.isOpen() == true)
	{
		Event event;
		while (render_window.pollEvent(event) == true)
		{
			if (event.type == Event::Closed)
			{
				render_window.close();
			}
			if (event.type == Event::KeyPressed)
			{
				perlin_noise_1.reset();
				perlin_noise_1.generate(source_1);

				perlin_noise_2.reset();
				perlin_noise_2.generate(source_2);

				perlin_noise_3.reset();
				perlin_noise_3.generate(source_3);

				SimpleMixer simple_mixer_1{ 0.3f };
				SimpleMixer simple_mixer_2{ 0.90f };

				simple_mixer_1.mix(source_1, source_2, source_4);
				simple_mixer_2.mix(source_3, source_4, source_5);
				
				RangeCorrectionFilter range_correction_filter;

				range_correction_filter.apply(source_5);
				custom_posterization_filter.apply(source_5);

				for (size_t x = 0; x < image.getSize().x; ++x)
				{
					for (size_t y = 0; y < image.getSize().y; ++y)
					{
						const Uint8 brightness = 255 * source_5.getValue(x, y);
						const Color color{ brightness, brightness, brightness, 255U };
						image.setPixel(x, y, color);
					}
				}
				texture.loadFromImage(image);
				sprite.setTexture(texture, true);
			}
		}

		render_window.clear();
		render_window.draw(sprite);
		render_window.display();
	}

	return 0;
}