#include "Source/Parametrix.h"
#include "Game.h"

float HumanGenome::GetHeight()
{
	float h = 1.8f;
	if(gender == GENDER::FEMALE)
		h = 1.7f;
	if(age < 18)
		h = max(0.1f, h*((float)age/18.0f));
	return h;
}

Human::Human()
{
	_mesh = nullptr;
}
Human::~Human()
{
	if(_mesh)
		delete _mesh;
}

void Human::Init(InputLayout *ia, Material* mat, HumanGenome *genome)
{
	_genome = genome;
	_mesh = new Mesh(ia);
	ParaCylinder cyl;
	cyl.height = _genome->GetHeight() - 0.5f;
	if(_genome->gender == GENDER::FEMALE)
		cyl.SetRadius(0.5f, 0.2f);
	else
		cyl.SetRadius(0.2f, 0.5f);
	cyl.sides = 6;

	ParaSphere head;
	head.radius = 0.25f;
	head.sides = 16;

	ParaSphere eye;
	eye.radius = 0.05f;
	eye.sides = 8;

	Color col = Color::Near(Color(0.0f, 0.0f, 1.0f), Color(0.2f, 0.2f, 1.0f));
	Polygons polys = cyl.Generate(_mesh);
	polys.SetColor(col);

	polys = head.Generate(_mesh, float3(0.0f, cyl.height + 0.25f, 0.0f));
	polys.SetColor(col);

	polys = eye.Generate(_mesh, float3(0.1f, cyl.height + 0.25f, -0.2f));
	polys.SetColor(Color(1.0f, 1.0f, 1.0f));
	polys = eye.Generate(_mesh, float3(-0.1f, cyl.height + 0.25f, -0.2f));
	polys.SetColor(Color(1.0f, 1.0f, 1.0f));

	_mesh->SetMaterial(mat);
	AddChild(_mesh);
}

void Human::Update()
{
	//GetLocalTransform().Rotate(Quaternion(Engine::Get()->GetTime()->spf, float3::ONE_Y), true);
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
	_mesh = nullptr;
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
	_engine->Initialize(1280, 800, false);
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

	InputLayout* iaGUI = man->CreateInputLayout();
	iaGUI->Add("POSITION", DXGI_FORMAT_R32G32_FLOAT, InputLayout::POSITION);
	iaGUI->Add("COLOR", DXGI_FORMAT_R32G32B32A32_FLOAT, InputLayout::COLOR);
	iaGUI->Add("UV", DXGI_FORMAT_R32G32_FLOAT, InputLayout::TEXCOORD0);
	Material* matGUI = man->CreateMaterial();
	vs = man->CreateVertexShader();
	vs->CompileFile(L"guiVS.hlsl", "main");
	ps = man->CreatePixelShader();
	ps->CompileFile(L"guiPS.hlsl", "main");

	matGUI->SetVertexShader(vs);
	matGUI->SetPixelShader(ps);
	matGUI->SetInputLayout(iaGUI);
	matGUI->SetDepthEnable(false);
	matGUI->SetDepthFunc(D3D12_COMPARISON_FUNC_NEVER);
	matGUI->SetDepthWriteMask(false);
	matGUI->SetCullMode(D3D12_CULL_MODE_NONE);
	matGUI->SetBlendEnable(true);
	matGUI->SetBlend(D3D12_BLEND_SRC_ALPHA, D3D12_BLEND_INV_SRC_ALPHA);
	matGUI->SetBlendAlpha(D3D12_BLEND_ONE, D3D12_BLEND_ONE);
	matGUI->Prepare();

	Canvas* canvas = new Canvas(iaGUI, matGUI);
	ShapeRectangle* rec = canvas->CreateRectangle(100.0f, 100.0f);
	rec->onClick += [rec, this](unsigned int i)
	{
		Debug::Log("Event!");
	};
	scene->AddCanvas(canvas);

	HumanGenome* genM = new HumanGenome;
	genM->age = 20;
	genM->gender = GENDER::MALE;
	HumanGenome* genF = new HumanGenome;
	genF->age = 20;
	genF->gender = GENDER::FEMALE;

	Human* h = new Human();
	h->Init(ia, mat, genM);
	h->GetLocalTransform().SetPosition(float3(2.0f, 0.0f, 0.0f));
	scene->AddActor(h);

	h = new Human();
	h->Init(ia, mat, genF);
	h->GetLocalTransform().SetPosition(float3(4.0f, 0.0f, 0.0f));
	scene->AddActor(h);


	for(int i = 0; i < 10; ++i)
	{
		for(int j = 0; j < 10; ++j)
		{
			Plant* h = new Plant();
			h->Init(ia, mat);
			h->GetLocalTransform().SetPosition(float3((float)i, 0.0f, (float)j));
			scene->AddActor(h);
		}
	}


	Mesh* mesh = new Mesh(ia);
	ParaBox box;
	box.size = float3(2.0f, 2.0f, 2.0f);
	Polygons polys = box.Generate(mesh);
	polys.SetColor(Color(0.0f, 0.0f, 1.0f));

	Mesh* meshPlane = new Mesh(ia);
	ParaPlane plane;
	plane.size = float2(2000.0f, 2000.0f);
	polys = plane.Generate(meshPlane);
	polys.SetColor(Color(0.0f, 0.8f, 0.2f));

	Mesh* meshCyl = new Mesh(ia);
	ParaCylinder cyl;
	cyl.height = 1.0f;
	cyl.SetRadius(0.5f, 0.1f);
	cyl.sides = 12;
	polys = cyl.Generate(meshCyl);
	polys.SetColor(Color(1.0f, 0.2f, 0.2f));
	meshCyl->GetLocalTransform().SetPosition(float3(4.0f, 0.0f, 4.0f));

	Mesh* meshSphere = new Mesh(ia);
	ParaSphere sphere;
	sphere.sides = 48;
	polys = sphere.Generate(meshSphere);
	polys.SetColor(Color(1.0f, 0.0f, 0.0f));
	meshCyl->GetLocalTransform().SetPosition(float3(6.0f, 0.0f, 6.0f));

		//mesh->GetTransform().SetPosition(float3(Random(-10.0f, 10.0f), Random(-10.0f, 10.0f), Random(-10.0f, 10.0f)));
	mesh->SetMaterial(mat);
	meshPlane->SetMaterial(mat);
	meshCyl->SetMaterial(mat);
	meshSphere->SetMaterial(mat);
	mesh->AddChild(meshCyl);
	scene->AddActor(mesh);
	scene->AddActor(meshPlane);
	scene->AddActor(meshCyl);
	scene->AddActor(meshSphere);

	_cam = new Camera;
	scene->AddActor(_cam);

		
	_cam->GetLocalTransform().SetPosition(float3(0.0f, 10.0f, -40.0f));
	_cam->SetRange(0.01f, 10000.0f);
	_engine->ShowCursor(true);
	Point2 cur = _engine->GetInput()->GetCursorPos();
	//_engine->GetInput()->SetMode(false);
	//_engine->GetInput()->ClipToWindow();

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
	float3 offset1 = float3(1.0f, 1.0f, 1.0f);
	float3 offset2 = float3(Math::Random(), Math::Random(), 1.0f);
	while(_engine->Update())
	{
		t += _engine->GetTime()->spf;
		spf = _engine->GetTime()->spf;

		if(_engine->GetInput()->IsKeyPressed(KEY_W))
			_cam->GetLocalTransform().Move(float3(0.0f, 0.0f, 30.0f*spf), true);
		if(_engine->GetInput()->IsKeyPressed(KEY_S))
			_cam->GetLocalTransform().Move(float3(0.0f, 0.0f, -30.0f*spf), true);
		if(_engine->GetInput()->IsKeyPressed(KEY_A))
			_cam->GetLocalTransform().Move(float3(-30.0f*spf, 0.0f, 0.0f), true);
		if(_engine->GetInput()->IsKeyPressed(KEY_D))
			_cam->GetLocalTransform().Move(float3(30.0f*spf, 0.0f, 0.0f), true);
		if(_engine->GetInput()->IsKeyPressed(KEY_Q))
			_cam->GetLocalTransform().Rotate(Quaternion(spf*0.5f, float3(1.0f, 0.0f, 0.0f)), true);
		if(_engine->GetInput()->IsKeyPressed(KEY_E))
			_cam->GetLocalTransform().Rotate(Quaternion(-spf*0.5f, float3(1.0f, 0.0f, 0.0f)), true);
		Point2 curDelta = Point2(0, 0);// _engine->GetInput()->GetCursorRel();
		if(curDelta.x != 0 || curDelta.y != 0)
		{
			float2 rPos;
			rPos.x = curDelta.x*spf*2.0f;
			rPos.y = curDelta.y*spf*2.0f;
			Quaternion q = _cam->GetLocalTransform().GetQuaternion();
			_cam->GetLocalTransform().Rotate(Quaternion(rPos.x, float3(0.0f, -1.0f, 0.0f)), true);
			_cam->GetLocalTransform().Rotate(Quaternion(rPos.y, float3(-1.0f, 0.0f, 0.0f)));
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