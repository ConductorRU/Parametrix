#include "Source/Parametrix.h"
#include "Game.h"

Human::Human()
{

}
Human::~Human()
{
	if(_mesh)
		delete _mesh;
}

void Human::Init(InputLayout *ia, Material* mat)
{
	_mesh = new Mesh(ia);
	ParaCone cyl;
	cyl.center = Vector(8.0f, 0.0f, 4.0f);
	cyl.height = 1.0f;
	cyl.radius = 0.5f;
	cyl.sides = 4;
	Polygons polys = cyl.Generate(_mesh);
	polys.SetColor(Color::Near(Color(0.0f, 0.8f, 0.2f), Color(0.0f, 0.4f, 0.2f)));
	_mesh->SetMaterial(mat);
	AddChild(_mesh);
}

void Human::BeforeRender()
{
	if(_mesh)
		_mesh->BeforeRender();
}

void Human::Render(Scene* scene)
{
	if(_mesh)
		_mesh->Render(scene);
}

Plant::Plant()
{

}
Plant::~Plant()
{
	if(_mesh)
		delete _mesh;
}
void Plant::Init(InputLayout *ia, Material* mat)
{
	_mesh = new Mesh(ia);
	ParaCone cyl;
	cyl.center = Vector(8.0f, 0.0f, 4.0f);
	cyl.height = 1.0f;
	cyl.radius = 0.5f;
	cyl.sides = 4;
	Polygons polys = cyl.Generate(_mesh);
	polys.SetColor(Color::Near(Color(0.0f, 0.8f, 0.2f), Color(0.0f, 0.4f, 0.2f)));
	_mesh->SetMaterial(mat);
	AddChild(_mesh);
}
void Plant::BeforeRender()
{
	if(_mesh)
		_mesh->BeforeRender();
}
void Plant::Render(Scene* scene)
{
	if(_mesh)
		_mesh->Render(scene);
}

Game::Game()
{
	_engine = new Engine;
	_engine->Initialize(800, 600, false);
	Scene* scene = _engine->GetScene();
	Manager* man = _engine->GetManager();

	InputLayout *ia = man->CreateInputLayout();
	ia->Add("POSITION", DXGI_FORMAT_R32G32B32_FLOAT, InputLayout::POSITION);
	ia->Add("COLOR", DXGI_FORMAT_R32G32B32A32_FLOAT, InputLayout::COLOR);
	ia->Add("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT, InputLayout::NORMAL);
	ia->Add("UV", DXGI_FORMAT_R32G32_FLOAT, InputLayout::TEXCOORD0);
	Material* mat = man->CreateMaterial();
	VertexShader *vs = man->CreateVertexShader();
	vs->CompileFile(L"VS.hlsl", "main");
	PixelShader *ps = man->CreatePixelShader();
	ps->CompileFile(L"PS.hlsl", "main");

	mat->SetVertexShader(vs);
	mat->SetPixelShader(ps);
	mat->SetInputLayout(ia);
	mat->Prepare();

	for(int i = 0; i < 10; ++i)
	{
		Human* h = new Human();
		h->Init(ia, mat);
		h->GetLocalTransform().SetPosition(Vector((float)i, 0.0f, 0.0f));
		scene->AddActor(h);
	}

	for(int i = 0; i < 10; ++i)
	{
		for(int j = 0; j < 10; ++j)
		{
			Plant* h = new Plant();
			h->Init(ia, mat);
			h->GetLocalTransform().SetPosition(Vector((float)i, 0.0f, (float)j));
			scene->AddActor(h);
		}
	}


	Mesh* mesh = new Mesh(ia);
	ParaBox box;
	box.size = Vector(2.0f, 2.0f, 2.0f);
	Polygons polys = box.Generate(mesh);
	polys.SetColor(Color(0.0f, 0.0f, 1.0f));

	Mesh* meshPlane = new Mesh(ia);
	ParaPlane plane;
	plane.size = Vector2(2000.0f, 2000.0f);
	polys = plane.Generate(meshPlane);
	polys.SetColor(Color(0.0f, 0.8f, 0.2f));

	Mesh* meshCyl = new Mesh(ia);
	ParaCylinder cyl;
	cyl.center = Vector(4.0f, 0.0f, 4.0f);
	cyl.height = 1.0f;
	cyl.radius = 0.5f;;
	cyl.sides = 12;
	polys = cyl.Generate(meshCyl);
	polys.SetColor(Color(0.0f, 0.8f, 0.2f));

		//mesh->GetTransform().SetPosition(Vector(Random(-10.0f, 10.0f), Random(-10.0f, 10.0f), Random(-10.0f, 10.0f)));
	mesh->SetMaterial(mat);
	meshPlane->SetMaterial(mat);
	meshCyl->SetMaterial(mat);
	mesh->AddChild(meshCyl);
	scene->AddActor(mesh);
	scene->AddActor(meshPlane);
	scene->AddActor(meshCyl);

	_cam = new Camera;
	scene->AddActor(_cam);

		
	_cam->GetLocalTransform().SetPosition(Vector(0.0f, 10.0f, -40.0f));
	_cam->SetRange(0.01f, 10000.0f);
	Point2 cur = _engine->GetInput()->GetCursorPos();
	_engine->GetInput()->SetMode(true);
	_engine->GetInput()->ClipToWindow();

}
Game::~Game()
{
	delete _engine;
}
void Game::Update()
{
	float t = 0.0f;
	float spf = 0.0f;
	Color col1 = Color(Math::Random(), Math::Random(), Math::Random(), 1.0f);
	Color col2 = Color(Math::Random(), Math::Random(), Math::Random(), 1.0f);
	Vector offset1 = Vector(1.0f, 1.0f, 1.0f);
	Vector offset2 = Vector(Math::Random(), Math::Random(), 1.0f);
	while(_engine->Update())
	{
		t += _engine->GetTime()->spf;
		spf = _engine->GetTime()->spf;

		if(_engine->GetInput()->IsKeyPressed(KEY_W))
			_cam->GetLocalTransform().Move(Vector(0.0f, 0.0f, 100.0f*spf), true);
		if(_engine->GetInput()->IsKeyPressed(KEY_S))
			_cam->GetLocalTransform().Move(Vector(0.0f, 0.0f, -100.0f*spf), true);
		if(_engine->GetInput()->IsKeyPressed(KEY_A))
			_cam->GetLocalTransform().Move(Vector(-100.0f*spf, 0.0f, 0.0f), true);
		if(_engine->GetInput()->IsKeyPressed(KEY_D))
			_cam->GetLocalTransform().Move(Vector(100.0f*spf, 0.0f, 0.0f), true);
		Point2 curDelta = _engine->GetInput()->GetCursorRel();
		if(curDelta.x != 0 || curDelta.y != 0)
		{
			Vector2 rPos;
			rPos.x = curDelta.x*spf*2.0f;
			rPos.y = curDelta.y*spf*2.0f;
			Quaternion q = _cam->GetLocalTransform().GetQuaternion();
			_cam->GetLocalTransform().Rotate(Quaternion(rPos.x, Vector(0.0f, -1.0f, 0.0f)), true);
			_cam->GetLocalTransform().Rotate(Quaternion(rPos.y, Vector(-1.0f, 0.0f, 0.0f)));
		}
		_cam->Update();
		if(_engine->GetInput()->IsKeyPressed(KEY_ESCAPE))
			_engine->Exit();
		Time *time = _engine->GetTime();
		static int sec = time->secCount;
		if(sec == time->secCount)
		{
			Debug::Log(to_string(time->fps) + " " + to_string(time->spf));
			++sec;
		}
		_engine->Render();
	}
}
Human* Game::CreateHuman()
{
	return nullptr;
}