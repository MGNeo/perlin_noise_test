#pragma once

class Source;

class Mixer
{
public:
	virtual	void mix(const Source &_first_source,
					 const Source &_second_source,
					 Source &_result_source) const = 0;

	virtual ~Mixer();
};

class SimpleMixer : public Mixer
{
public:
	SimpleMixer(const float _k = 0.5f);

	void mix(const Source &_first_source,
			 const Source &_second_source,
			 Source &_result_source) const override;
private:
	const float k;
};
