#pragma once

class Human: public Actor
{
private:
	Mesh* _mesh;
public:
	Mesh* GetMesh() { return _mesh; };
	Human();
	~Human();
	void Init(InputLayout *ia, Material* mat);
	void BeforeRender();
	void Render(Scene* scene);
};

class Plant: public Actor
{
private:
	Mesh* _mesh;
public:
	Mesh* GetMesh() { return _mesh; };
	Plant();
	~Plant();
	void Init(InputLayout *ia, Material* mat);
	void BeforeRender();
	void Render(Scene* scene);
};

class Game
{
private:
	Engine* _engine;
	Camera* _cam;
public:
	Game();
	~Game();
	Human* CreateHuman();
	void Update();
};