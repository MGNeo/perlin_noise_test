#include "Mixer.hpp"

#include <stdexcept>

using namespace perlin_noise;
using namespace std;

void Mixer::push_back(unique_ptr<Sample> &_sample,
				      unique_ptr<Mode> &_mode)
{
	if (_sample == nullptr)
	{
		throw invalid_argument("Mixer::add(), _sample == nullptr");
	}

	if (_mode == nullptr)
	{
		throw invalid_argument("Mixer::add(), _mode == nullptr");
	}

	samples.push_back(move(_sample));
	modes.push_back(move(_mode));

}