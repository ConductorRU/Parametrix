#pragma once
namespace Led
{
	struct Quaternion
	{
		float x, y, z, w;
		static const Quaternion IDENTITY;
		static const Quaternion ZERO;
		FORCEINLINE Quaternion()
		{
			x = 0.0f;
			y = 0.0f;
			z = 0.0f;
			w = 1.0f;
		}
		FORCEINLINE Quaternion(float fx, float fy, float fz, float fw)
		{
			x = fx;
			y = fy;
			z = fz;
			w = fw;
		}
		FORCEINLINE Quaternion(float rad, const float3& axis)
		{
			float fHalfAngle = 0.5f*rad;
			float fsin = sin(fHalfAngle);
			w = cos(fHalfAngle);
			x = fsin*axis.x;
			y = fsin*axis.y;
			z = fsin*axis.z;
		}
		FORCEINLINE Quaternion& operator= (const Quaternion& quat)
		{
			w = quat.w;
			x = quat.x;
			y = quat.y;
			z = quat.z;
			return *this;
		}
		FORCEINLINE bool operator== (const Quaternion& quat) const
		{
			return (quat.x == x) && (quat.y == y) && (quat.z == z) && (quat.w == w);
		}
		FORCEINLINE bool operator!= (const Quaternion& quat) const
		{
			return !operator==(quat);
		}
		FORCEINLINE float3 RotateVector(float3 v) const
		{	
			const float3 q(x, y, z);
			const float3 t = q.Cross(v)*2.0f;
			const float3 Result = v + (t*w) + q.Cross(t);
			return Result;
		}
		FORCEINLINE float3 GetAxisX() const
		{
			return RotateVector(float3(1.f, 0.f, 0.f));
		}
		FORCEINLINE float3 GetAxisY() const
		{
			return RotateVector(float3(0.f, 1.f, 0.f));
		}
		FORCEINLINE float3 GetAxisZ() const
		{
			return RotateVector(float3(0.f, 0.f, 1.f));
		}
		FORCEINLINE float3 GetForwardVector() const
		{
			return GetAxisX();
		}
		FORCEINLINE float3 GetRightVector() const
		{
			return GetAxisY();
		}
		FORCEINLINE float3 GetUpVector() const
		{
			return GetAxisZ();
		}
		FORCEINLINE Quaternion operator*(const Quaternion& q) const
		{
			Quaternion res;
			res.x = (q.w * x) + (q.x * w) + (q.y * z) - (q.z * y);
			res.y = (q.w * y) - (q.x * z) + (q.y * w) + (q.z * x);
			res.z = (q.w * z) + (q.x * y) - (q.y * x) + (q.z * w);
			res.w = (q.w * w) - (q.x * x) - (q.y * y) - (q.z * z);
			return res;
		}
		FORCEINLINE Quaternion Inverse() const
		{
			return Quaternion(-x, -y, -z, w);
		}
		FORCEINLINE float3 operator*(const float3& v) const
		{
			return RotateVector(v);
		}
	};
}