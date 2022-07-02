#pragma once

class Game
{
private:
	Engine* _engine;
	Camera* _cam;
	Mesh* _mesh;
public:
	Game();
	~Game();
	void Update();
};