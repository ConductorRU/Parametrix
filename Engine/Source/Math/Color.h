#pragma once
#include "Math.h"

namespace Led
{
	struct Color
	{
		float r, g, b, a;
		Color() { r = g = b = a = 1.0f; };
		Color(float red, float green, float blue, float alpha = 1.0f)
		{
			r = red;
			g = green;
			b = blue;
			a = alpha;
		}
		Color(int red, int green, int blue, int alpha = 255)
		{
			r = (float)red/255.0f;
			g = (float)green/255.0f;
			b = (float)blue/255.0f;
			a = (float)alpha/255.0f;
		}
		static Color Near(const Color& color, const Color& range)
		{
			Color col;
			col.r = Math::Clamp(color.r + Math::Random(-range.r, range.r), 0.0f, 1.0);
			col.g = Math::Clamp(color.g + Math::Random(-range.g, range.g), 0.0f, 1.0);
			col.b = Math::Clamp(color.b + Math::Random(-range.b, range.b), 0.0f, 1.0);
			col.a = Math::Clamp(color.a + Math::Random(-range.a, range.a), 0.0f, 1.0);
			return col;
		}
		static Color Lerp(const Color& start, const Color& end, float delta)
		{
			Color res;
			res.r = Math::Lerp(start.r, end.r, delta);
			res.g = Math::Lerp(start.g, end.g, delta);
			res.b = Math::Lerp(start.b, end.b, delta);
			res.a = Math::Lerp(start.a, end.a, delta);
			return res;
		}
	};
}