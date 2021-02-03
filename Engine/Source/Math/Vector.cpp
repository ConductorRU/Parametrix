#include "stdafx.h"
#include "Vector.h"
namespace Led
{
	const float3 float3::ONE_X = float3(1.0f, 0.0f, 0.0f);
	const float3 float3::ONE_Y = float3(0.0f, 1.0f, 0.0f);
	const float3 float3::ONE_Z = float3(0.0f, 0.0f, 1.0f);
	const float3 float3::MINUS_X = float3(-1.0f, 0.0f, 0.0f);
	const float3 float3::MINUS_Y = float3(0.0f, -1.0f, 0.0f);
	const float3 float3::MINUS_Z = float3(0.0f, 0.0f, -1.0f);
	const float3 float3::ONE = float3(1.0f, 1.0f, 1.0f);
	const float3 float3::ZERO = float3(0.0f, 0.0f, 0.0f);

	const float2 float2::ONE = float2(1.0f, 1.0f);
	const float2 float2::ZERO = float2(0.0f, 0.0f);

	void float2::QuadLerp(const float2& a, const float2& b, const float2& p, const float f)
	{
		float invF = 1.0f - f;
		if(f <= 0.0f)
			*this = a;
		else if(f >= 1.0f)
			*this = b;
		*this = a*(invF*invF) + p*(2.0f*f*invF) + b*(f*f);
	}

	void float2::CubicLerp(const float2& p_1, const float2& p, const float2& p1, const float2& p2, const float f)
	{
		if(f <= 0.0f)
			*this = p;
		else if(f >= 1.0f)
			*this = p1;
		float2 a = (p - p1)*1.5f + (p2 - p_1)*0.5f;
		float2 b = p*-2.5f + p1*2.0f - p2*0.5f + p_1;
		float2 c = (p1 - p_1)*0.5f;
		float2 d = p;
		*this = ((a*f + b)*f + c)*f + d;
	}

	float2 float2::TransformCoord(const float2 &v)
	{
		float2 res;
		res.x = v.x*x + v.y*y;
		res.y = v.x*y + v.y*x;
		return res;
	}

	Rect::Rect()
	{
		minX = 0;
		minY = 0;
		maxX = 0;
		maxY = 0;
	}

	Rect::Rect(int minx, int miny, int maxx, int maxy)
	{
		minX = minx;
		minY = miny;
		maxX = maxx;
		maxY = maxy;
	}

	RECT Rect::ToRect()
	{
		RECT r;
		r.left = minX;
		r.top = minY;
		r.right = maxX;
		r.bottom = maxY;
		return r;
	}

	bool Rect::Pick(int x, int y)
	{
		if(minX <= x && x < maxX && minY <= y && y < maxY)
			return true;
		return false;
	}

	Box2D::Box2D()
	{
	
	}

	Box2D::Box2D(float minX, float minY, float maxX, float maxY)
	{
		_min.x = minX;
		_min.y = minY;
		_max.x = maxX;
		_max.y = maxY;
	}

	float2 Box2D::GetSize()
	{
		return _max - _min;
	}

	void Box2D::SetSize(float sizeX, float sizeY)
	{
		_max.x = _min.x + sizeX;
		_max.y = _min.y + sizeY;
	}
}