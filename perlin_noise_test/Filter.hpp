#pragma once

class Source;

class Filter
{
public:
	virtual void apply(Source &_source) const = 0;
	virtual ~Filter();
};

class PosterizationFilter : public Filter
{
public:
	PosterizationFilter(const size_t _levels = 10U);

	void apply(Source &_source) const override;
private:
	size_t levels;
};

// Пороговый фильтр.

// Просветляющий фильтр.

// Затемняющий фильтр.
