#pragma once



namespace Led
{
	class Scene
	{
		friend class Render;
		friend class Camera;
	private:
		Color _background;
		set<class Actor*> _actors;
		set<class Camera*> _cameras;
		set<class Canvas*> _canvas;
		Camera* _camera;
		void _AddCamera(class Camera* cam);
		void _RemoveCamera(Camera* cam);
	public:
		Color GetBackground() {return _background;};
		void SetBackground(const Color &color) {_background = color;};
		Camera* GetCamera() { return _camera; };
		Scene();
		~Scene();
		bool AddCanvas(Canvas *canvas);
		bool IsActor(Actor *actor);
		bool IsCanvas(Canvas *canvas);
		bool AddActor(Actor *actor);
		bool RemoveActor(Actor *actor);
		void Clear();
		void Update();
		void BeforeRender();
		void Render();
	};
}