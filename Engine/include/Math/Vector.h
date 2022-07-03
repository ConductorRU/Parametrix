#pragma once
#include "Math.h"

namespace Led
{
	struct int3
	{
		int x;
		int y;
		int z;
	};
	struct float3
	{
		float x, y, z;
		float3() { x = y = z = 0.0f; };
		float3(float value) { x = y = z = value; };
		float3(float px, float py, float pz)
		{
			x = px;
			y = py;
			z = pz;
		}
		float GetDistance(const float3 &v) const
		{
			return sqrt((x - v.x)*(x - v.x) + (y - v.y)*(y - v.y) + (z - v.z)*(z - v.z));
		}
		float GetSquaredDistance(const float3 &v) const
		{
			return (x - v.x)*(x - v.x) + (y - v.y)*(y - v.y) + (z - v.z)*(z - v.z);
		}
		float3 operator + (const float3& p) const
		{
			return float3(x + p.x, y + p.y, z + p.z);
		}
		float3& operator += (const float3& p)
		{
			x += p.x;
			y += p.y;
			z += p.z;
			return *this;
		}
		float3& operator -= (const float3& p)
		{
			x -= p.x;
			y -= p.y;
			z -= p.z;
			return *this;
		}
		bool operator== (const float3& p) const
		{
			return (p.x == x) && (p.y == y) && (p.z == z);
		}
		bool operator!= (const float3& p) const
		{
			return (p.x != x) || (p.y != y) || (p.z != z);
		}
		float3 operator - (const float3 pVector) const
		{
			return float3(x - pVector.x, y - pVector.y, z - pVector.z);
		}
		float3 operator - () const
		{
			return float3(-x, -y, -z);
		}
		float3 operator / (const float f) const
		{
			return float3(x / f, y / f, z / f);
		}
		float3 operator * (const float f) const
		{
			return float3(x*f, y*f, z*f);
		}
		float3 operator * (const float3 &pVector) const
		{
			return float3(x*pVector.x, y*pVector.y, z*pVector.z);
		}
		float3& operator *= (const float3& p)
		{
			x *= p.x;
			y *= p.y;
			z *= p.z;
			return *this;
		}
		float3& operator /= (const float3& p)
		{
			x /= p.x;
			y /= p.y;
			z /= p.z;
			return *this;
		}
		float3 operator / (const float3 &pVector) const
		{
			return float3(x / pVector.x, y / pVector.y, z / pVector.z);
		}
		bool operator < (const float3& vector) const
		{
			if(x < vector.x) return true;
			if(x == vector.x && y < vector.y) return true;
			if(x == vector.x && y == vector.y && z < vector.z) return true;
			return false;
		}
		bool operator > (const float3& vector) const
		{
			if(x > vector.x) return true;
			if(x == vector.x && y > vector.y) return true;
			if(x == vector.x && y == vector.y && z > vector.z) return true;
			return false;
		}
		float Dot(const float3& vec) const
		{
			return x*vec.x + y*vec.y + z*vec.z;
		}
		float3 Cross(const float3& p) const
		{
			return float3(y*p.z - z*p.y, z*p.x - x*p.z, x*p.y - y*p.x);
		}
		static float3 Lerp(const float3& start, const float3& end, float delta)
		{
			float3 res;
			res.x = Math::Lerp(start.x, end.x, delta);
			res.y = Math::Lerp(start.y, end.y, delta);
			res.z = Math::Lerp(start.z, end.z, delta);
			return res;
		}
		inline float Normalize()
		{
			float fLength = sqrt(x*x + y*y + z*z);
			if ( fLength > 1e-08 )
			{
				float fInvLength = 1.0f/fLength;
				x *= fInvLength;
				y *= fInvLength;
				z *= fInvLength;
			}
			return fLength;
		}
		inline static float3 Normal(float3 v0, float3 v1, float3 v2)
		{
			float3 vVector1 = v2 - v0;
			float3 vVector2 = v1 - v0;
			float3 vNormal = vVector1.Cross(vVector2);
			vNormal.Normalize();
			return vNormal;
		}
		static const float3 ONE_X;
		static const float3 ONE_Y;
		static const float3 ONE_Z;
		static const float3 MINUS_X;
		static const float3 MINUS_Y;
		static const float3 MINUS_Z;
		static const float3 ONE;
		static const float3 ZERO;
	};

	struct float2
	{
	public:
		float x;
		float y;
		float2()
		{
			x = y = 0.0f;
		}
		float2(float pX, float pY)
		{
			x = pX;
			y = pY;
		}
		float2 operator + (const float2& p) const
		{
			return float2(x + p.x, y + p.y);
		}
		float2& operator += (const float2& p)
		{
			x += p.x;
			y += p.y;
			return *this;
		}
		bool operator== (const float2& p) const
		{
			return (p.x == x) && (p.y == y);
		}
		bool operator!= (const float2& p) const
		{
			return (p.x != x) || (p.y != y);
		}
		float2 operator - (const float2 pVector) const
		{
			return float2(x - pVector.x, y - pVector.y);
		}
		float2 operator - () const
		{
			return float2(-x, -y);
		}
		float2 operator / (const float f)
		{
			return float2(x / f, y / f);
		}
		float2 operator * (const float f) const
		{
			return float2(x*f, y*f);
		}
		float2 operator * (const float2 &pVector)
		{
			return float2(x*pVector.x, y*pVector.y);
		}
		float2 operator / (const float2 &pVector)
		{
			return float2(x / pVector.x, y / pVector.y);
		}
		float Length() const
		{
			return sqrt(x*x + y*y);
		}
		float2 &Abs()
		{
			x = abs(x);
			y = abs(y);
			return *this;
		}
		float Normalize()
		{
			float fLength = sqrt(x*x + y*y);
			if(fLength > 1e-08)
			{
				float fInvLength = 1.0f / fLength;
				x *= fInvLength;
				y *= fInvLength;
			}
			return fLength;
		}
		float Dot(const float2& vec) const
		{
			return x*vec.x + y*vec.y;
		}
		float2 Normal() const
		{
			float2 n = float2(-y, x);
			n.Normalize();
			return n;
		}
		float GetDistance(const float2 &destVector) const
		{
			return sqrt((x - destVector.x)*(x - destVector.x) + (y - destVector.y)*(y - destVector.y));
		}
		float GetSquaredDistance(const float2 &destVector) const
		{
			return (x - destVector.x)*(x - destVector.x) + (y - destVector.y)*(y - destVector.y);
		}
		void QuadLerp(const float2& a, const float2& b, const float2& p, const float f);
		void CubicLerp(const float2& p_1, const float2& p, const float2& p1, const float2& p2, const float f);
		float2 TransformCoord(const float2 &v);
		static const float2 ONE;
		static const float2 ZERO;
	};

	struct Vector4
	{
		union
		{
			struct
			{
				float x, y, z, w;
			};
			struct
			{
				float left, top, right, bottom;
			};
			struct
			{
				float2 pos, size;
			};
		};
		Vector4()
		{
			x = y = z = w = 0.0f;
		}
		Vector4(float pX, float pY, float pZ, float pW)
		{
			x = pX;
			y = pY;
			z = pZ;
			w = pW;
		}
	};

	class Point
	{
	public:
		int x;
		int y;
		int z;
		Point() { x = y = z = 0; }
		Point(int x, int y, int z)
		{
			this->x = x;
			this->y = y;
			this->z = z;
		}
		const bool operator== (const Point &s)
		{
			if(x == s.x && y == s.y && z == s.z)
				return true;
			return false;
		}
		const bool operator!= (const Point &s)
		{
			if(x != s.x || y != s.y || z != s.z)
				return true;
			return false;
		}
		inline Point operator + (const  Point &pVector) const
		{
			Point p(x + pVector.x, y + pVector.y, z + pVector.z);
			return p;
		}
		inline Point operator - (const  Point &pVector) const
		{
			Point p(x - pVector.x, y - pVector.y, z - pVector.z);
			return p;
		}
		inline Point& operator += ( const Point& p )
		{
			x += p.x;
			y += p.y;
			z += p.z;
			return *this;
		}
		inline Point& operator -= ( const Point& p )
		{
			x -= p.x;
			y -= p.y;
			z -= p.z;
			return *this;
		}
		inline Point operator * (const Point &pVector)
		{
			return Point(x*pVector.x, y*pVector.y, z*pVector.z);
		}
		inline Point operator / (const Point &pVector)
		{
			return Point(x/pVector.x, y/pVector.y, z/pVector.z);
		}
		inline Point operator / (const float &f)
		{
			return Point((int)((float)x/f), (int)((float)y/f), (int)((float)z/f));
		}
		inline Point operator * (const float &f)
		{
			return Point((int)((float)x*f), (int)((float)y*f), (int)((float)z*f));
		}
		inline Point operator / (const int &i)
		{
			return Point(x/i, y/i, z/i);
		}
		inline Point operator * (const int &i)
		{
			return Point(x*i, y*i, z*i);
		}
		inline bool operator < ( const Point& point ) const
		{
			if (x < point.x) return true;
			if (x == point.x && y < point.y) return true;
			if (x == point.x && y == point.y && z < point.z) return true;
			return false;
		}
		inline bool operator > ( const Point& point ) const
		{
			if (x > point.x) return true;
			if (x == point.x && y > point.y) return true;
			if (x == point.x && y == point.y && z > point.z) return true;
			return false;
		}
	};

	struct Point2
	{
		int x, y;
		Point2()
		{
			x = y = 0;
		}
		Point2(int pX, int pY)
		{
			x = pX;
			y = pY;
		}
		~Point2()
		{

		}
		Point2 operator + (const  Point2 &pVector)
		{
			return Point2(x + pVector.x, y + pVector.y);
		}
		Point2 operator - (const  Point2 &pVector)
		{
			return Point2(x - pVector.x, y - pVector.y);
		}
		Point2 operator * (const Point2 &pVector)
		{
			return Point2(x*pVector.x, y*pVector.y);
		}
		Point2 operator / (const Point2 &pVector)
		{
			return Point2(x / pVector.x, y / pVector.y);
		}
		Point2 operator / (float f)
		{
			return Point2((int)((float)x / f), (int)((float)y / f));
		}
		Point2 operator * (float f)
		{
			return Point2((int)((float)x*f), (int)((float)y*f));
		}
		Point2 operator / (int i)
		{
			return Point2(x / i, y / i);
		}
		Point2 operator * (int i)
		{
			return Point2(x*i, y*i);
		}
		bool operator == (const Point2& point) const
		{
			return (x == point.x) && (y == point.y);
		}
		bool operator!= (const Point2 &s) const
		{
			if(x != s.x || y != s.y)
				return true;
			return false;
		}
		bool operator < (const Point2& point) const
		{
			if(x < point.x) return true;
			if(x == point.x && y < point.y) return true;
			return false;
		}
		bool operator > (const Point2& point) const
		{
			if(x > point.x) return true;
			if(x == point.x && y > point.y) return true;
			return false;
		}
		float GetDistance(const Point2 &destVector) const
		{
			return sqrt((float)((x - destVector.x)*(x - destVector.x) + (y - destVector.y)*(y - destVector.y)));
		}
		void Lerp(const Point2& p1, const Point2& p2, const float f)
		{
			if(f < 0.0f)
				*this = p1;
			else if(f > 1.0f)
				*this = p2;
			x = p1.x + int(f*float(p2.x - p1.x) + 0.5f);
			y = p1.y + int(f*float(p2.y - p1.y) + 0.5f);
		}
	};

	class Rect
	{
	public:
		int minX, minY, maxX, maxY;
		Rect();
		Rect(int minx, int miny, int maxx, int maxy);
		RECT ToRect();
		bool Pick(int x, int y);
	};

	class Box2D
	{
	private:
		float2 _min;
		float2 _max;
	public:
		float2 GetMin() { return _min; };
		float2 GetMax() { return _max; };
		Box2D();
		Box2D(float minX, float minY, float maxX, float maxY);
		float2 GetSize();
		void SetSize(float sizeX, float sizeY);
	};
}