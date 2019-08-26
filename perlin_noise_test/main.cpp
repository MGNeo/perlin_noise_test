#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>

#include "Mixer.hpp"

using namespace sf;
using namespace std;
using namespace perlin_noise;

const size_t WINDOW_SIZE = 800U;

int main(int argc, char **argv)
{
	RenderWindow render_window{ VideoMode(WINDOW_SIZE, WINDOW_SIZE), "Perlin Noise Test", Style::Close };

	Mixer mixer;

	unique_ptr<Sample> sample_1 = make_unique<Sample>(6, 6);
	unique_ptr<Mode> mode_1 = make_unique<SumMode>(true, 1.f);
	mixer.push_back(sample_1, mode_1);

	unique_ptr<Sample> sample_2 = make_unique<Sample>(12, 12);
	unique_ptr<Mode> mode_2 = make_unique<SubMode>(false, 0.9f);
	mixer.push_back(sample_2, mode_2);

	unique_ptr<Sample> sample_3 = make_unique<Sample>(30, 30);
	unique_ptr<Mode> mode_3 = make_unique<SubMode>(false, 0.1f);
	mixer.push_back(sample_3, mode_3);

	// Нужна более гибкая настройка наложения и сдвигов шума по яркости.
	// Возможно, лучше, чтобы PerlinNoise генерировал сырье в виде float[][] со значениями [0; 1].
	// И потом смешивание производить в ручную по слоям сырья?

	Texture texture = mixer.generateTexture(WINDOW_SIZE, WINDOW_SIZE);
	Sprite sprite;
	sprite.setTexture(texture, true);

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
		render_window.draw(sprite);
		render_window.display();
	}

	return 0;
}