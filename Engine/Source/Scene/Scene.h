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
		Camera* _camera;
		void _AddCamera(class Camera* cam);
		void _RemoveCamera(Camera* cam);
	public:
		Color GetBackground() {return _background;};
		void SetBackground(const Color &color) {_background = color;};
		Camera* GetCamera() { return _camera; };
		Scene();
		~Scene();
		bool IsActor(Actor *actor);
		bool AddActor(Actor *actor);
		bool RemoveActor(Actor *actor);
		void Clear();
		void Update();
		void Render();
	};
}