#include "stdafx.h"
#include "Math/Header.h"
#include "Render/Render.h"
#include "Scene/Actor.h"
#include "Scene/Camera.h"
#include "Scene/Scene.h"

namespace Led
{
	Camera::Camera()
	{
		_fov = 45.0f;
		_orthoScale = 16.0f;
		_aspect = 1.0f;
		_near = 0.1f;
		_far = 100.0f;
		_isOrtho = false;
		_proj = Matrix4::Perspective(Math::ToRadian(_fov), _aspect, _near, _far);
	}
	Camera::~Camera()
	{
	
	}
	void Camera::AfterAdd(Scene* scene)
	{
		scene->_AddCamera(this);
	}
	bool Camera::BeforeRemove(Scene* scene)
	{
		scene->_RemoveCamera(this);
		return true;
	}
	Matrix4 Camera::GetView() const
	{
		return _view;
	}
	Matrix4 Camera::GetProjection() const
	{
		return _proj;
	}
	void Camera::Update()
	{
		float aspect = _aspect*Render::Get()->GetAspect();
		_proj = Matrix4::Perspective(Math::ToRadian(_fov), aspect, _near, _far);
		//_proj = Matrix4::Orthographic(aspect*_orthoScale, (1.0f/aspect)*_orthoScale, _near, _far);
		_view = GetWorldTransform().ToMatrix4();
		float3 pos = GetWorldTransform().GetPosition();
		_view[3][0] = -float3(_view[0][0], _view[1][0], _view[2][0]).Dot(pos);
		_view[3][1] = -float3(_view[0][1], _view[1][1], _view[2][1]).Dot(pos);
		_view[3][2] = -float3(_view[0][2], _view[1][2], _view[2][2]).Dot(pos);
	}
}