#pragma once
#include "Vector.h"
#include "Transform.h"

namespace Led
{
	class float2x2
	{
	public:
		union
		{
			float m[2][2];
			float v[4];
			struct
			{
				float A, B;
				float C, D;
			};
			struct
			{
				float _00, _01;
				float _10, _11;
			};
		};
		float2x2()
		{
			A = 1.0f; B = 0.0f;
			C = 0.0f; D = 1.0f;
		}

		float2x2(float m00, float m01, float m10, float m11)
		{
			A = m00; B = m01;
			C = m10; D = m11;
		}

		float2x2 operator*(const float2x2& m) const
		{
			return float2x2
			(
				A*m.A + B*m.C, A*m.B + B*m.D,
				C*m.A + D*m.C, C*m.B + D*m.D
			);
		}

		float2 TransformVector(const float2& v) const
		{
			return float2(v.x*A + v.y*C, v.x*B + v.y*D);
		}

		float Determinant() const
		{
			return (A*D - B*C);
		}

		float InverseDeterminant() const
		{
			float Det = Determinant();
			return 1.0f/Det;
		}

		float2x2 Inverse() const
		{
			float InvDet = InverseDeterminant();
			return float2x2(D*InvDet, -B*InvDet, -C*InvDet, A*InvDet);
		}
	};

	class Transform2D
	{
	private:
		float2 _position;
		float2x2 _matrix;
	public:
		const float2x2& GetMatrix() const { return _matrix; }
		const float2& GetTranslation() const { return _position; }
		void SetTranslation(const float2& pos) { _position = pos; }

		Transform2D(const float2& pos = float2(0.0f, 0.0f))
		{
			_position = pos;
		}

		Transform2D(const float2x2& transform, const float2& pos = float2(0.f,0.f))
		: _matrix(transform), _position(pos)
		{
		}

		Transform2D Inverse() const
		{
			float2x2 iMat = _matrix.Inverse();
			float2 iPos = iMat.TransformVector(-_position);
			return Transform2D(iMat, iPos);
		}
		Matrix4 ToMatrix4() const
		{
			return Matrix4(
				_matrix.A, _matrix.B, 0.0f, 0.0f,
				_matrix.C, _matrix.D, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				_position.x, _position.y, 0.0f, 1.0f
			);
		}

		FORCEINLINE Transform2D operator * (const Transform2D &t)
		{
			///TODO - написал наугад
			Transform2D res;
			res._matrix = t._matrix*_matrix;
			res._position = _position + t._position;
			return res;
		}

	};
}