#pragma once
namespace Led
{
	namespace Math
	{
		inline float ToRadian(float angle)
		{
			return angle*(3.141592654f/180.0f);
		}
		inline float ToAngle(float rad)
		{
			return rad/(3.141592654f/180.0f);
		}
		inline float Lerp(float fMin, float fMax, float value)
		{
			return fMin + value*(fMax - fMin);
		}
		inline int Lerp(int fMin, int fMax, float value)
		{
			return fMin + (int)(value*(float)(fMax - fMin));
		}
		inline float Slerp(float p_1, float p, float p1, float p2, float f)
		{
			if(f <= 0.0f)
				return p;
			else if(f >= 1.0f)
				return p1;
			float a = (p - p1)*1.5f + (p2 - p_1)*0.5f;
			float b = p*-2.5f + p1*2.0f - p2*0.5f + p_1;
			float c = (p1 - p_1)*0.5f;
			return ((a*f + b)*f + c)*f + p;
		}
		inline int Clamp(int val, int minVal, int maxVal)
		{
			return max(minVal, min(val, maxVal));
		}

		inline float Clamp(float val, float minVal, float maxVal)
		{
			return max(minVal, min(val, maxVal));
		}
		inline int Random(int min, int max, bool reseed)
		{
			if(min == max)
				return min;
			if(reseed)
				srand(unsigned(std::time(0)));
			if(min > max)
				return int(rand()%(min - max + 1) + max);
			return int(rand()%(max - min + 1) + min);
		}

		inline float Random(float min = 0.0f, float max = 1.0f, bool reseed = false)
		{
			if(reseed)
				srand(unsigned(std::time(0)));
			return (max - min)*((float)rand()/(float)(0x7fff)) + min;
		}
	}
}