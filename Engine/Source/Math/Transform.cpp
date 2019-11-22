#include "stdafx.h"
#include "Transform.h"
namespace Led
{
	const Matrix Matrix::ZERO(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	const Matrix Matrix::IDENTITY(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	const Quaternion Quaternion::ZERO = Quaternion(0.0f, 0.0f, 0.0f, 0.0f);
	const Quaternion Quaternion::IDENTITY = Quaternion(0.0f, 0.0f, 0.0f, 1.0f);

	Matrix4 Matrix4::Perspective(float fov, float aspect, float zNear, float zFar)
	{
		Matrix4 mat;
#ifdef OPENGL
		float y = 1.0f / tan(fov*0.5f);
		float x = y / aspect;
		float zF = zFar / (zNear - zFar);
		float zN = (zNear*zFar) / (zNear - zFar);
		mat.m[0][0] = y;
		mat.m[0][1] = 0.0f;
		mat.m[0][2] = 0.0f;
		mat.m[0][3] = 0.0f;
		mat.m[1][0] = 0.0f;
		mat.m[1][1] = x;
		mat.m[1][2] = 0.0f;
		mat.m[1][3] = 0.0f;
		mat.m[2][0] = 0.0f;
		mat.m[2][1] = 0.0f;
		mat.m[2][2] = zF;
		mat.m[2][3] = -1.0f;
		mat.m[3][0] = 0.0f;
		mat.m[3][1] = 0.0f;
		mat.m[3][2] = zN;
		mat.m[3][3] = 0.0f;
#else
		float h = 1.0f / tan(fov*0.5f);
		float w = h / aspect;
		float a = zFar / (zFar - zNear);
		float b = -(zNear*zFar) / (zFar - zNear);
		mat.m[0][0] = w;
		mat.m[0][1] = 0.0f;
		mat.m[0][2] = 0.0f;
		mat.m[0][3] = 0.0f;
		mat.m[1][0] = 0.0f;
		mat.m[1][1] = h;
		mat.m[1][2] = 0.0f;
		mat.m[1][3] = 0.0f;
		mat.m[2][0] = 0.0f;
		mat.m[2][1] = 0.0f;
		mat.m[2][2] = a;
		mat.m[2][3] = 1.0f;
		mat.m[3][0] = 0.0f;
		mat.m[3][1] = 0.0f;
		mat.m[3][2] = -a*zNear;
		mat.m[3][3] = 0.0f;
#endif
		return mat;
	}

	Matrix4 Matrix4::Orthographic(float width, float height, float zNear, float zFar)
	{
		Matrix4 mat;
		mat.m[0][0] = 2.0f / width;
		mat.m[0][1] = 0.0f;
		mat.m[0][2] = 0.0f;
		mat.m[0][3] = 0.0f;
		mat.m[1][0] = 0.0f;
		mat.m[1][1] = 2.0f / height;
		mat.m[1][2] = 0.0f;
		mat.m[1][3] = 0.0f;
		mat.m[2][0] = 0.0f;
		mat.m[2][1] = 0.0f;
		mat.m[2][2] = 1.0f / (zFar - zNear);
		mat.m[2][3] = 0.0f;
		mat.m[3][0] = 0.0f;
		mat.m[3][1] = 0.0f;
		mat.m[3][2] =- mat.m[2][2]*zNear;
		mat.m[3][3] = 1.0f;
		return mat;
	}

	Matrix4 Matrix4::Orthographic(float minX, float maxX, float minY, float maxY, float zNear, float zFar)
	{
		Matrix4 mat;
		mat.m[0][0] = 2.0f / (maxX - minX);
		mat.m[0][1] = 0.0f;
		mat.m[0][2] = 0.0f;
		mat.m[0][3] = 0.0f;
		mat.m[1][0] = 0.0f;
		mat.m[1][1] = 2.0f / (maxY - minY);
		mat.m[1][2] = 0.0f;
		mat.m[1][3] = 0.0f;
		mat.m[2][0] = 0.0f;
		mat.m[2][1] = 0.0f;
		mat.m[2][2] = 1.0f / (zFar - zNear);
		mat.m[2][3] = 0.0f;
		mat.m[3][0] = (minX + maxX) / (minX - maxX);
		mat.m[3][1] = (maxY + minY) / (minY - maxY);
		mat.m[3][2] = zNear / (zNear - zFar);
		mat.m[3][3] = 1.0f;
		return mat;
	}

	Matrix4 Matrix4::Projection2D(float width, float height)
	{
		Matrix4 mat;
		mat.m[0][0] = 2.0f / width;
		mat.m[0][1] = 0.0f;
		mat.m[0][2] = 0.0f;
		mat.m[0][3] = 0.0f;
		mat.m[1][0] = 0.0f;
		mat.m[1][1] = -2.0f / height;
		mat.m[1][2] = 0.0f;
		mat.m[1][3] = 0.0f;
		mat.m[2][0] = 0.0f;
		mat.m[2][1] = 0.0f;
		mat.m[2][2] = 0.0f;
		mat.m[2][3] = 0.0f;
		mat.m[3][0] = -1.0f;
		mat.m[3][1] = 1.0f;
		mat.m[3][2] = 1.0f;
		mat.m[3][3] = 1.0f;
		return mat;
	}
}