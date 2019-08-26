#pragma once

namespace perlin_noise
{
	class OverflowControl
	{
	public:
		OverflowControl(const bool _overflow_control = true);
	protected:
		const bool overflow_control;
	};

	class Mode
	{
	public:
		virtual float blend(const float _first, 
							const float _second) = 0;
		
		virtual ~Mode();
	};

	class SumMode : public OverflowControl, public Mode
	{
	public:
		SumMode(const bool _overflow_control);

		float blend(const float _first,
					const float _second) override;
	};

	class SubMode : public OverflowControl, public Mode
	{
	public:
		SubMode(const bool _overflow_control);

		float blend(const float _first,
					const float _second) override;
	};
}
