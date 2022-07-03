#pragma once

namespace Led
{
	class Camera: public Actor
	{
	private:
		Matrix4 _proj;
		Matrix4 _view;
		float _aspect;
		float _fov;
		float _near;
		float _far;
		float _orthoScale;
		bool _isOrtho;
	public:
		float GetNear() { return _near; };
		float GetFar() { return _far; };
		void SetRange(float nearRange, float farRange) { _near = nearRange; _far = farRange; };
		void SetFov(float angle) { _fov = angle; };
		Camera();
		~Camera();
		void AfterAdd(Scene* scene);
		bool BeforeRemove(Scene* scene);
		Matrix4 GetView() const;
		Matrix4 GetProjection() const;
		void Update();
	};
	typedef shared_ptr<Camera> SCamera;
}