#include "Parametrix.h"
#include "Game.h"

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
	//Texture* tex = new Texture();
	//tex->Create(16, 16);


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

	_mesh = new Mesh(ia);
	ParaBox box;
	box.size = float3(2.0f, 2.0f, 2.0f);
	Polygons polys = box.Generate(_mesh);
	polys.SetColor(Color(1.0f, 1.0f, 1.0f));


		//mesh->GetTransform().SetPosition(float3(Random(-10.0f, 10.0f), Random(-10.0f, 10.0f), Random(-10.0f, 10.0f)));
	_mesh->SetMaterial(mat);
	scene->AddActor(_mesh);

	_cam = new Camera;
	scene->AddActor(_cam);

		
	_cam->GetLocalTransform().SetPosition(float3(0.0f, 1.0f, -5.0f));
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
		_mesh->GetLocalTransform().Rotate(Quaternion(1.0f * time->spf, float3::ONE_Y));
		if(sec == time->secCount)
		{
			Debug::Log(to_string(time->fps) + " " + to_string(time->spf));
			++sec;
		}
		_engine->Render();
	}
}
