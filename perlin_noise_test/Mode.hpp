#pragma once

namespace perlin_noise
{
	class Mode
	{
	public:
		Mode(const bool _overflow_control,
			const float _alpha);

		virtual float blend(const float _first, 
							const float _second) = 0;
		
		virtual ~Mode();

	protected:
		const bool overflow_control;
		const float alpha;
	};

	class SumMode : public Mode
	{
	public:
		SumMode(const bool _overflow_control,
				const float _alpha);

		float blend(const float _first,
					const float _second) override;
	};

	class SubMode : public Mode
	{
	public:
		SubMode(const bool _overflow_control,
			    const float _alpha);

		float blend(const float _first,
					const float _second) override;
	};

	class MulMode : public Mode
	{
	public:
		MulMode(const bool _overflow_control,
			    const float _alpha);

		float blend(const float _first,
			        const float _second) override;
	};

}
