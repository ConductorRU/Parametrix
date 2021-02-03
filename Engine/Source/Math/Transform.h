#pragma once
#include "Math.h"
#include "Vector.h"
#include "Quaternion.h"
namespace Led
{
	__declspec(align(16)) struct Matrix4
	{
		union
		{
			float m[4][4];
			float v[16];
			struct
			{
				float _00, _01, _02, _03;
				float _10, _11, _12, _13;
				float _20, _21, _22, _23;
				float _30, _31, _32, _33;
			};
		};
		Matrix4()
		{
			_00 = 1.0f; _01 = 0.0f; _02 = 0.0f; _03 = 0.0f;
			_10 = 0.0f; _11 = 1.0f; _12 = 0.0f; _13 = 0.0f;
			_20 = 0.0f; _21 = 0.0f; _22 = 1.0f; _23 = 0.0f;
			_30 = 0.0f; _31 = 0.0f; _32 = 0.0f; _33 = 1.0f;
		}
		Matrix4(float v00, float v01, float v02, float v03, float v10, float v11, float v12, float v13, float v20, float v21, float v22, float v23, float v30, float v31, float v32, float v33)
		{
			_00 = v00; _01 = v01; _02 = v02; _03 = v03;
			_10 = v10; _11 = v11; _12 = v12; _13 = v13;
			_20 = v20; _21 = v21; _22 = v22; _23 = v23;
			_30 = v30; _31 = v31; _32 = v32; _33 = v33;
		}
		Matrix4(const struct Matrix &matrix)
		{
			memcpy(m, &matrix, 48);
			m[3][0] = m[3][1] = m[3][2] = 0.0f;
			m[3][3] = 1.0f;
		}
		float* operator [] (uint row)
		{
			return m[row];
		}
		float& operator () (uint row, uint col)
		{
			return m[row][col];
		}
		float operator () (uint row, uint col) const
		{
			return m[row][col];
		}
		static Matrix4 Perspective(float fov, float aspect, float zNear, float zFar);
		static Matrix4 Orthographic(float width, float height, float zNear, float zFar);
		static Matrix4 Orthographic(float minX, float maxX, float minY, float maxY, float zNear, float zFar);
		static Matrix4 Projection2D(float width, float height);
		Matrix4 &Transpose()
		{
			Matrix4 mat;
			for(int i = 0; i != 4; ++i)
				for(int j = 0; j != 4; ++j)
					mat.m[i][j] = m[j][i];
			*this = mat;
			return *this;
		}
		Matrix4 operator * (const Matrix4 &m2) const
		{
			Matrix4 r;
			float x = m[0][0];
			float y = m[0][1];
			float z = m[0][2];
			float w = m[0][3];
			r.m[0][0] = m2.m[0][0]*x + m2.m[1][0]*y + m2.m[2][0]*z + m2.m[3][0]*w;
			r.m[0][1] = m2.m[0][1]*x + m2.m[1][1]*y + m2.m[2][1]*z + m2.m[3][1]*w;
			r.m[0][2] = m2.m[0][2]*x + m2.m[1][2]*y + m2.m[2][2]*z + m2.m[3][2]*w;
			r.m[0][3] = m2.m[0][3]*x + m2.m[1][3]*y + m2.m[2][3]*z + m2.m[3][3]*w;
			x = m[1][0];
			y = m[1][1];
			z = m[1][2];
			w = m[1][3];
			r.m[1][0] = m2.m[0][0]*x + m2.m[1][0]*y + m2.m[2][0]*z + m2.m[3][0]*w;
			r.m[1][1] = m2.m[0][1]*x + m2.m[1][1]*y + m2.m[2][1]*z + m2.m[3][1]*w;
			r.m[1][2] = m2.m[0][2]*x + m2.m[1][2]*y + m2.m[2][2]*z + m2.m[3][2]*w;
			r.m[1][3] = m2.m[0][3]*x + m2.m[1][3]*y + m2.m[2][3]*z + m2.m[3][3]*w;
			x = m[2][0];
			y = m[2][1];
			z = m[2][2];
			w = m[2][3];
			r.m[2][0] = m2.m[0][0]*x + m2.m[1][0]*y + m2.m[2][0]*z + m2.m[3][0]*w;
			r.m[2][1] = m2.m[0][1]*x + m2.m[1][1]*y + m2.m[2][1]*z + m2.m[3][1]*w;
			r.m[2][2] = m2.m[0][2]*x + m2.m[1][2]*y + m2.m[2][2]*z + m2.m[3][2]*w;
			r.m[2][3] = m2.m[0][3]*x + m2.m[1][3]*y + m2.m[2][3]*z + m2.m[3][3]*w;
			x = m[3][0];
			y = m[3][1];
			z = m[3][2];
			w = m[3][3];
			r.m[3][0] = m2.m[0][0]*x + m2.m[1][0]*y + m2.m[2][0]*z + m2.m[3][0]*w;
			r.m[3][1] = m2.m[0][1]*x + m2.m[1][1]*y + m2.m[2][1]*z + m2.m[3][1]*w;
			r.m[3][2] = m2.m[0][2]*x + m2.m[1][2]*y + m2.m[2][2]*z + m2.m[3][2]*w;
			r.m[3][3] = m2.m[0][3]*x + m2.m[1][3]*y + m2.m[2][3]*z + m2.m[3][3]*w;
			return r;
		}
		inline Matrix4 operator * (const Matrix &m2) const;
	};

	struct Matrix
	{
		float m[3][4];
		static const Matrix ZERO;
		static const Matrix IDENTITY;
		Matrix()
		{
			m[0][0] = 1.0f; m[1][0] = 0.0f; m[2][0] = 0.0f;
			m[0][1] = 0.0f; m[1][1] = 1.0f; m[2][1] = 0.0f;
			m[0][2] = 0.0f; m[1][2] = 0.0f; m[2][2] = 1.0f;
			m[0][3] = 0.0f; m[1][3] = 0.0f; m[2][3] = 0.0f;
		}
		Matrix(float m11, float m12, float m13, float m21, float m22, float m23, float m31, float m32, float m33, float m41, float m42, float m43)
		{
			m[0][0] = m11; m[1][0] = m12; m[2][0] = m13;
			m[0][1] = m21; m[1][1] = m22; m[2][1] = m23;
			m[0][2] = m31; m[1][2] = m32; m[2][2] = m33;
			m[0][3] = m41; m[1][3] = m42; m[2][3] = m43;
		}
		float* operator [] (uint row)
		{
			return m[row];
		}
		float& operator () (uint row, uint col)
		{
			return m[row][col];
		}
		float operator () (uint row, uint col) const
		{
			return m[row][col];
		}
		Matrix operator * (const Matrix &m2) const
		{
			Matrix r;
			r.m[0][0] = m[0][0] * m2.m[0][0] + m[1][0] * m2.m[0][1] + m[2][0] * m2.m[0][2];
			r.m[1][0] = m[0][0] * m2.m[1][0] + m[1][0] * m2.m[1][1] + m[2][0] * m2.m[1][2];
			r.m[2][0] = m[0][0] * m2.m[2][0] + m[1][0] * m2.m[2][1] + m[2][0] * m2.m[2][2];
			r.m[0][1] = m[0][1] * m2.m[0][0] + m[1][1] * m2.m[0][1] + m[2][1] * m2.m[0][2];
			r.m[1][1] = m[0][1] * m2.m[1][0] + m[1][1] * m2.m[1][1] + m[2][1] * m2.m[1][2];
			r.m[2][1] = m[0][1] * m2.m[2][0] + m[1][1] * m2.m[2][1] + m[2][1] * m2.m[2][2];
			r.m[0][2] = m[0][2] * m2.m[0][0] + m[1][2] * m2.m[0][1] + m[2][2] * m2.m[0][2];
			r.m[1][2] = m[0][2] * m2.m[1][0] + m[1][2] * m2.m[1][1] + m[2][2] * m2.m[1][2];
			r.m[2][2] = m[0][2] * m2.m[2][0] + m[1][2] * m2.m[2][1] + m[2][2] * m2.m[2][2];
			r.m[0][3] = m[0][3] * m2.m[0][0] + m[1][3] * m2.m[0][1] + m[2][3] * m2.m[0][2] + m2.m[0][3];
			r.m[1][3] = m[0][3] * m2.m[1][0] + m[1][3] * m2.m[1][1] + m[2][3] * m2.m[1][2] + m2.m[1][3];
			r.m[2][3] = m[0][3] * m2.m[2][0] + m[1][3] * m2.m[2][1] + m[2][3] * m2.m[2][2] + m2.m[2][3];
			return r;
		}
		Matrix operator * (const Matrix4 &m2) const
		{
			Matrix r;
			r.m[0][0] = m[0][0] * m2.m[0][0] + m[1][0] * m2.m[0][1] + m[2][0] * m2.m[0][2];
			r.m[1][0] = m[0][0] * m2.m[1][0] + m[1][0] * m2.m[1][1] + m[2][0] * m2.m[1][2];
			r.m[2][0] = m[0][0] * m2.m[2][0] + m[1][0] * m2.m[2][1] + m[2][0] * m2.m[2][2];
			r.m[0][1] = m[0][1] * m2.m[0][0] + m[1][1] * m2.m[0][1] + m[2][1] * m2.m[0][2];
			r.m[1][1] = m[0][1] * m2.m[1][0] + m[1][1] * m2.m[1][1] + m[2][1] * m2.m[1][2];
			r.m[2][1] = m[0][1] * m2.m[2][0] + m[1][1] * m2.m[2][1] + m[2][1] * m2.m[2][2];
			r.m[0][2] = m[0][2] * m2.m[0][0] + m[1][2] * m2.m[0][1] + m[2][2] * m2.m[0][2];
			r.m[1][2] = m[0][2] * m2.m[1][0] + m[1][2] * m2.m[1][1] + m[2][2] * m2.m[1][2];
			r.m[2][2] = m[0][2] * m2.m[2][0] + m[1][2] * m2.m[2][1] + m[2][2] * m2.m[2][2];
			r.m[0][3] = m[0][3] * m2.m[0][0] + m[1][3] * m2.m[0][1] + m[2][3] * m2.m[0][2] + m2.m[0][3];
			r.m[1][3] = m[0][3] * m2.m[1][0] + m[1][3] * m2.m[1][1] + m[2][3] * m2.m[1][2] + m2.m[1][3];
			r.m[2][3] = m[0][3] * m2.m[2][0] + m[1][3] * m2.m[2][1] + m[2][3] * m2.m[2][2] + m2.m[2][3];
			return r;
		}
		Matrix operator + (const Matrix &m2) const
		{
			Matrix r;
			r.m[0][0] = m[0][0] + m2.m[0][0];
			r.m[1][0] = m[1][0] + m2.m[1][0];
			r.m[2][0] = m[2][0] + m2.m[2][0];
			r.m[0][1] = m[0][1] + m2.m[0][1];
			r.m[1][1] = m[1][1] + m2.m[1][1];
			r.m[2][1] = m[2][1] + m2.m[2][1];
			r.m[0][2] = m[0][2] + m2.m[0][2];
			r.m[1][2] = m[1][2] + m2.m[1][2];
			r.m[2][2] = m[2][2] + m2.m[2][2];
			r.m[0][3] = m[0][3] + m2.m[0][3];
			r.m[1][3] = m[1][3] + m2.m[1][3];
			r.m[2][3] = m[2][3] + m2.m[2][3];
			return r;
		}
		Matrix operator - (const Matrix &m2) const
		{
			Matrix r;
			r.m[0][0] = m[0][0] - m2.m[0][0];
			r.m[1][0] = m[1][0] - m2.m[1][0];
			r.m[2][0] = m[2][0] - m2.m[2][0];
			r.m[0][1] = m[0][1] - m2.m[0][1];
			r.m[1][1] = m[1][1] - m2.m[1][1];
			r.m[2][1] = m[2][1] - m2.m[2][1];
			r.m[0][2] = m[0][2] - m2.m[0][2];
			r.m[1][2] = m[1][2] - m2.m[1][2];
			r.m[2][2] = m[2][2] - m2.m[2][2];
			r.m[0][3] = m[0][3] - m2.m[0][3];
			r.m[1][3] = m[1][3] - m2.m[1][3];
			r.m[2][3] = m[2][3] - m2.m[2][3];
			return r;
		}
		bool operator == (const Matrix& m2) const
		{
			if(m[0][0] != m2.m[0][0] || m[0][1] != m2.m[0][1] || m[0][2] != m2.m[0][2] || m[0][3] != m2.m[0][3] ||
				m[1][0] != m2.m[1][0] || m[1][1] != m2.m[1][1] || m[1][2] != m2.m[1][2] || m[1][3] != m2.m[1][3] ||
				m[2][0] != m2.m[2][0] || m[2][1] != m2.m[2][1] || m[2][2] != m2.m[2][2] || m[2][3] != m2.m[2][3])
				return false;
			return true;
		}
		bool operator != (const Matrix& m2) const
		{
			if(m[0][0] != m2.m[0][0] || m[0][1] != m2.m[0][1] || m[0][2] != m2.m[0][2] || m[0][3] != m2.m[0][3] ||
				m[1][0] != m2.m[1][0] || m[1][1] != m2.m[1][1] || m[1][2] != m2.m[1][2] || m[1][3] != m2.m[1][3] ||
				m[2][0] != m2.m[2][0] || m[2][1] != m2.m[2][1] || m[2][2] != m2.m[2][2] || m[2][3] != m2.m[2][3])
				return true;
			return false;
		}
		Matrix Inverse() const
		{
			float t00 = (m[2][2] * m[1][1] - m[2][1] * m[1][2]);
			float t10 = -(m[2][2] * m[1][0] - m[2][0] * m[1][2]);
			float t20 = (m[2][1] * m[1][0] - m[2][0] * m[1][1]);
			float invDet = 1.0f / (t00 * m[0][0] + t10 * m[0][1] + t20 * m[0][2]);
			float d00 = t00 * invDet;
			float d10 = t10 * invDet;
			float d20 = t20 * invDet;
			float d01 = -(m[2][2] * m[0][1] - m[2][1] * m[0][2]) * invDet;
			float d11 = (m[2][2] * m[0][0] - m[2][0] * m[0][2]) * invDet;
			float d21 = -(m[2][1] * m[0][0] - m[2][0] * m[0][1]) * invDet;
			float d02 = (m[1][2] * m[0][1] - m[1][1] * m[0][2]) * invDet;
			float d12 = -(m[1][2] * m[0][0] - m[1][0] * m[0][2]) * invDet;
			float d22 = (m[1][1] * m[0][0] - m[1][0] * m[0][1]) * invDet;
			float v0 = m[2][1] * m[1][0] - m[2][0] * m[1][1];
			float v1 = m[2][2] * m[1][0] - m[2][0] * m[1][2];
			float v2 = m[2][3] * m[1][0] - m[2][0] * m[1][3];
			float v3 = m[2][2] * m[1][1] - m[2][1] * m[1][2];
			float v4 = m[2][3] * m[1][1] - m[2][1] * m[1][3];
			float v5 = m[2][3] * m[1][2] - m[2][2] * m[1][3];
			float d03 = -(v5 * m[0][1] - v4 * m[0][2] + v3 * m[0][3]) * invDet;
			float d13 = (v5 * m[0][0] - v2 * m[0][2] + v1 * m[0][3]) * invDet;
			float d23 = -(v4 * m[0][0] - v2 * m[0][1] + v0 * m[0][3]) * invDet;
			return Matrix(d00, d10, d20, d01, d11, d21, d02, d12, d22, d03, d13, d23);
		}
		Matrix ToRotationMatrix() const
		{
			Matrix r = *this;
			r.m[0][3] = 0.0f;
			r.m[1][3] = 0.0f;
			r.m[2][3] = 0.0f;
			return r;
		}
		Matrix ToPositionMatrix() const
		{
			Matrix r = Matrix::IDENTITY;
			r.m[0][3] = m[0][3];
			r.m[1][3] = m[1][3];
			r.m[2][3] = m[2][3];
			return r;
		}
		void TransformScreen(float x, float y, float &destX, float &destY) const
		{
			destX = m[0][0]*x + m[0][1]*y + m[0][3] - 0.5f;
			destY = m[1][0]*x + m[1][1]*y + m[1][3] - 0.5f;
		}
	};
	inline Matrix4 Matrix4::operator * (const Matrix &m2) const
	{
		Matrix4 r;
		r.m[0][0] = m[0][0] * m2.m[0][0] + m[1][0] * m2.m[0][1] + m[2][0] * m2.m[0][2];
		r.m[1][0] = m[0][0] * m2.m[1][0] + m[1][0] * m2.m[1][1] + m[2][0] * m2.m[1][2];
		r.m[2][0] = m[0][0] * m2.m[2][0] + m[1][0] * m2.m[2][1] + m[2][0] * m2.m[2][2];
		r.m[3][0] = 0.0f;
		r.m[0][1] = m[0][1] * m2.m[0][0] + m[1][1] * m2.m[0][1] + m[2][1] * m2.m[0][2];
		r.m[1][1] = m[0][1] * m2.m[1][0] + m[1][1] * m2.m[1][1] + m[2][1] * m2.m[1][2];
		r.m[2][1] = m[0][1] * m2.m[2][0] + m[1][1] * m2.m[2][1] + m[2][1] * m2.m[2][2];
		r.m[3][1] = 0.0f;
		r.m[0][2] = m[0][2] * m2.m[0][0] + m[1][2] * m2.m[0][1] + m[2][2] * m2.m[0][2];
		r.m[1][2] = m[0][2] * m2.m[1][0] + m[1][2] * m2.m[1][1] + m[2][2] * m2.m[1][2];
		r.m[2][2] = m[0][2] * m2.m[2][0] + m[1][2] * m2.m[2][1] + m[2][2] * m2.m[2][2];
		r.m[3][2] = 0.0f;
		r.m[0][3] = m[0][3] * m2.m[0][0] + m[1][3] * m2.m[0][1] + m[2][3] * m2.m[0][2] + m2.m[0][3];
		r.m[1][3] = m[0][3] * m2.m[1][0] + m[1][3] * m2.m[1][1] + m[2][3] * m2.m[1][2] + m2.m[1][3];
		r.m[2][3] = m[0][3] * m2.m[2][0] + m[1][3] * m2.m[2][1] + m[2][3] * m2.m[2][2] + m2.m[2][3];
		r.m[3][3] = 1.0f;
		return r;
	}

	class Transform
	{
	protected:
		float3 _position;
		Quaternion _rotation;
		float3 _scale;
	public:
		Transform()
		{
			_scale = float3(1.0f, 1.0f, 1.0f);
		}
		Matrix ToMatrix()
		{
			Matrix out;
			out[0][3] = _position.x;
			out[1][3] = _position.y;
			out[2][3] = _position.z;

			const float x2 = _rotation.x + _rotation.x;
			const float y2 = _rotation.y + _rotation.y;
			const float z2 = _rotation.z + _rotation.z;
			{
				const float xx2 = _rotation.x*x2;
				const float yy2 = _rotation.y*y2;
				const float zz2 = _rotation.z*z2;

				out[0][0] = (1.0f - (yy2 + zz2))*_scale.x;
				out[1][1] = (1.0f - (xx2 + zz2))*_scale.y;
				out[2][2] = (1.0f - (xx2 + yy2))*_scale.z;
			}
			{
				const float yz2 = _rotation.y*z2;
				const float wx2 = _rotation.w*x2;

				out[1][2] = (yz2 - wx2)*_scale.z;
				out[2][1] = (yz2 + wx2)*_scale.y;
			}
			{
				const float xy2 = _rotation.x*y2;
				const float wz2 = _rotation.w*z2;

				out[0][1] = (xy2 - wz2)*_scale.y;
				out[1][0] = (xy2 + wz2)*_scale.x;
			}
			{
				const float xz2 = _rotation.x*z2;
				const float wy2 = _rotation.w*y2;

				out[0][2] = (xz2 + wy2) * _scale.z;
				out[2][0] = (xz2 - wy2) * _scale.x;
			}
			return out;
		}
		Matrix4 ToMatrix4()
		{
			Matrix4 out;
			out[3][0] = _position.x;
			out[3][1] = _position.y;
			out[3][2] = _position.z;
			out[0][3] = 0.0f;
			out[1][3] = 0.0f;
			out[2][3] = 0.0f;
			out[3][3] = 1.0f;
			const float x2 = _rotation.x + _rotation.x;
			const float y2 = _rotation.y + _rotation.y;
			const float z2 = _rotation.z + _rotation.z;
			{
				const float xx2 = _rotation.x*x2;
				const float yy2 = _rotation.y*y2;
				const float zz2 = _rotation.z*z2;

				out[0][0] = (1.0f - (yy2 + zz2))*_scale.x;
				out[1][1] = (1.0f - (xx2 + zz2))*_scale.y;
				out[2][2] = (1.0f - (xx2 + yy2))*_scale.z;
			}
			{
				const float yz2 = _rotation.y*z2;
				const float wx2 = _rotation.w*x2;

				out[2][1] = (yz2 - wx2)*_scale.z;
				out[1][2] = (yz2 + wx2)*_scale.y;
			}
			{
				const float xy2 = _rotation.x*y2;
				const float wz2 = _rotation.w*z2;

				out[1][0] = (xy2 - wz2)*_scale.y;
				out[0][1] = (xy2 + wz2)*_scale.x;
			}
			{
				const float xz2 = _rotation.x*z2;
				const float wy2 = _rotation.w*y2;

				out[2][0] = (xz2 + wy2) * _scale.z;
				out[0][2] = (xz2 - wy2) * _scale.x;
			}
			return out;
		}
		__inline void SetPosition(const float3& position)
		{
			_position = position;
		}
		__inline void Move(const float3& position, bool isRelative = false)
		{
			if(isRelative)
				_position += _rotation.Inverse()*position;
			else
				_position += position;
		}
		__inline void SetRotation(const Quaternion& rotation)
		{
			_rotation = rotation;
		}
		__inline void Rotate(const Quaternion& rotation, bool isRelative = false)
		{
			if(isRelative)
			{
				_rotation = rotation*_rotation;
				//_rotation = _rotation*rotation.Inverse();
			}
			else
				_rotation = _rotation*rotation;
		}
		__inline void SetScale(const float3& scale)
		{
			_scale = scale;
		}
		__inline float3 GetPosition() const
		{
			return _position;
		}
		__inline Quaternion GetQuaternion() const
		{
			return _rotation;
		}
		__inline float3 GetScale() const
		{
			return _scale;
		}
		FORCEINLINE bool IsScaleNegative() const
		{
			return (_scale.x < 0.f || _scale.y < 0.f || _scale.z < 0.f);
		}
		FORCEINLINE Transform operator * (const Transform &t)
		{
			Transform res;
			res._rotation = t._rotation*_rotation;
			res._scale = _scale*t._scale;
			res._position = t._rotation*(t._scale*_position) + t._position;
			return res;
		}
	};
}