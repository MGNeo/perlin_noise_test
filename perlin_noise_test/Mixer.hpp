#pragma once

#include <SFML/Graphics.hpp>
#include <list>
#include "Sample.hpp"
#include "Mode.hpp"

namespace perlin_noise
{
	class Mixer
	{
	public:
		void push_back(std::unique_ptr<Sample> &_sample,
					   std::unique_ptr<Mode> &_mode);

		/*sf::Texture generateTexture(const size_t _width,
									const size_t _height);*/
	private:
		std::list< std::unique_ptr<Sample> > samples;
		std::list< std::unique_ptr<Mode> > modes;
	};
};