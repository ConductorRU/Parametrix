#pragma once

enum class GENDER
{
	MALE = 0,
	FEMALE = 1
};

struct HumanGenome
{
	GENDER gender;
	int age;
	float GetHeight();
};

class Human: public Actor
{
private:
	Mesh* _mesh;
	bool _gender;
	HumanGenome *_genome;
public:
	Mesh* GetMesh() { return _mesh; };
	Human();
	~Human();
	void Init(InputLayout *ia, Material* mat, HumanGenome *genome);
	void Update();
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