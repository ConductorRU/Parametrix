#include "stdafx.h"
#include "Scene/Mesh.h"
#include "Material/Material.h"
#include "Scene/Camera.h"
#include "Scene/Scene.h"


namespace Led
{
	void Poly::SetNormal(const float3& nor)
	{
		for(Vertex* v: list)
			v->SetNormal(nor);
	}

	void Poly::SetColor(const Color& color)
	{
		for(Vertex* v: list)
			v->SetColor(color);
	}

	void Poly::SetUV(const initializer_list<float2> uv)
	{
		uint size = min((uint)list.size(), (uint)uv.size());
		uint n = 0;
		for(float2 coord : uv)
    {
			if(n >= size)
				break;
			list[n]->SetUV(coord);
			++n;
    }
	}

	void Polygons::SetNormal(const float3& nor)
	{
		for(Poly& v: list)
			v.SetNormal(nor);
	}

	void Polygons::SetColor(const Color& color)
	{
		for(Poly& v: list)
			v.SetColor(color);
	}

	Mesh::Mesh(InputLayout* il): RenderMesh(il)
	{
		
	}
	Mesh::~Mesh()
	{
	
	}

	Poly Mesh::AddTriangle(const float3 &pos0, const float3 &pos1, const float3 &pos2)
	{
		uint size = (uint)_vertex.size();
		Poly list;
		Vertex* v = CreateVertex();
		v->SetPosition(pos0);
		list.list.push_back(v);
		v = CreateVertex();
		v->SetPosition(pos1);
		list.list.push_back(v);
		v = CreateVertex();
		v->SetPosition(pos2);
		list.list.push_back(v);
		_index.push_back(size);
		_index.push_back(size + 1);
		_index.push_back(size + 2);
		return list;
	}

	Poly Mesh::AddQuad(const float3 &pos0, const float3 &pos1, const float3 &pos2, const float3 &pos3)
	{
		uint size = (uint)_vertex.size();
		Poly list;
		Vertex* v = CreateVertex();
		v->SetPosition(pos0);
		list.list.push_back(v);
		v = CreateVertex();
		v->SetPosition(pos1);
		list.list.push_back(v);
		v = CreateVertex();
		v->SetPosition(pos2);
		list.list.push_back(v);
		v = CreateVertex();
		v->SetPosition(pos3);
		list.list.push_back(v);
		_index.push_back(size);
		_index.push_back(size + 1);
		_index.push_back(size + 2);
		_index.push_back(size + 2);
		_index.push_back(size + 3);
		_index.push_back(size);
		return list;
	}

	void Mesh::BeforeRender()
	{
		UpdateBuffer();
	}
	void Mesh::Render(Scene* scene)
	{
		float3 lDir = float3(1.0f, -1.0f, 3.0f);
		lDir.Normalize();
		Camera* cam = scene->GetCamera();
		if(cam)
		{
			SetConst(SHADER_PS, "ConstantBufferX", "camPos", cam->GetLocalTransform().GetPosition());
			SetConst(SHADER_PS, "ConstantBufferX", "lightDir", lDir);
			Matrix4 mWorld = GetWorldTransform().ToMatrix4();
			Matrix4 mView = cam->GetView();
			Matrix4 mProj = cam->GetProjection();
			Matrix4 wv = mWorld*mView*mProj;
			SetConst(SHADER_VS, "ConstantBuffer", "wvpMat", wv);
			SetConst(SHADER_VS, "ConstantBuffer", "worldMat", mWorld);

			if(_material)
				_material->Render();
			RenderMesh::Render(_material);
		}
	}

	void ParaOp::Bridge(RenderMesh *mesh, Poly from, Poly to)
	{
		for(Vertex *f: from.list)
		{
			float3 pos = f->GetPosition();
			float dist = -1.0f;
			Vertex* s = nullptr;
			for(Vertex *t: to.list)
			{
				float d = t->GetPosition().GetSquaredDistance(pos);
				if(dist < -0.5f || dist > d)
				{
					dist = d;
					s = t;
				}
			}
		}
	}

	Polygons ParaBox::Generate(Mesh *mesh, const float3 &center)
	{
		float3 xyz = (center - size)*0.5f;
		float3 XYZ = (center + size)*0.5f;
		xyz.y = center.y;
		XYZ.y = center.y + size.y;
		Polygons res;
		float3 xyZ = float3(xyz.x, xyz.y, XYZ.z);
		float3 xYz = float3(xyz.x, XYZ.y, xyz.z);
		float3 xYZ = float3(xyz.x, XYZ.y, XYZ.z);

		float3 Xyz = float3(XYZ.x, xyz.y, xyz.z);
		float3 XyZ = float3(XYZ.x, xyz.y, XYZ.z);
		float3 XYz = float3(XYZ.x, XYZ.y, xyz.z);

		Poly poly = mesh->AddQuad(xyz, xYz, XYz, Xyz);
		poly.SetNormal(float3(0.0f, 0.0f, -1.0f));
		poly.SetUV({float2(0.0f, 0.0f), float2(1.0f, 0.0f), float2(1.0f, 1.0f), float2(0.0f, 1.0f)});
		res.list.push_back(poly);

		poly = mesh->AddQuad(XyZ, XYZ, xYZ, xyZ);
		poly.SetNormal(float3(0.0f, 0.0f, 1.0f));
		poly.SetUV({float2(0.0f, 0.0f), float2(1.0f, 0.0f), float2(1.0f, 1.0f), float2(0.0f, 1.0f)});
		res.list.push_back(poly);

		poly = mesh->AddQuad(xyz, xyZ, xYZ, xYz);
		poly.SetNormal(float3(-1.0f, 0.0f, 0.0f));
		poly.SetUV({float2(0.0f, 0.0f), float2(1.0f, 0.0f), float2(1.0f, 1.0f), float2(0.0f, 1.0f)});
		res.list.push_back(poly);

		poly = mesh->AddQuad(XYz, XYZ, XyZ, Xyz);
		poly.SetNormal(float3(1.0f, 0.0f, 0.0f));
		poly.SetUV({float2(0.0f, 0.0f), float2(1.0f, 0.0f), float2(1.0f, 1.0f), float2(0.0f, 1.0f)});
		res.list.push_back(poly);

		poly = mesh->AddQuad(xyz, Xyz, XyZ, xyZ);
		poly.SetNormal(float3(0.0f, -1.0f, 0.0f));
		poly.SetUV({float2(0.0f, 0.0f), float2(1.0f, 0.0f), float2(1.0f, 1.0f), float2(0.0f, 1.0f)});
		res.list.push_back(poly);

		poly = mesh->AddQuad(xYZ, XYZ, XYz, xYz);
		poly.SetNormal(float3(0.0f, 1.0f, 0.0f));
		poly.SetUV({float2(0.0f, 0.0f), float2(1.0f, 0.0f), float2(1.0f, 1.0f), float2(0.0f, 1.0f)});
		res.list.push_back(poly);
		return res;
	}
	Polygons ParaPlane::Generate(Mesh *mesh, const float3 &center)
	{
		float3 minBox = (center - float3(size.x, 0.0f, size.y))*0.5f;
		float3 maxBox = (center + float3(size.x, 0.0f, size.y))*0.5f;
		Polygons res;
		Poly poly = mesh->AddQuad(float3(minBox.x, minBox.y, minBox.z), float3(minBox.x, minBox.y, maxBox.z), float3(maxBox.x, minBox.y, maxBox.z), float3(maxBox.x, minBox.y, minBox.z));
		poly.SetUV({float2(0.0f, 0.0f), float2(1.0f, 0.0f), float2(1.0f, 1.0f), float2(0.0f, 1.0f)});
		poly.SetNormal(float3(0.0f, 1.0f, 0.0f));
		res.list.push_back(poly);
		return res;
	}
	Polygons ParaSphere::Generate(Mesh *mesh, const float3 &center)
	{
		float delta = (PI/sides)*2.0f;
		uint hSegments = sides/2 + sides%2;
		float deltaH = (radius*2.0f)/(float)hSegments;

		float a = 0.0f;
		Polygons res;
		Poly poly;
		float3 c0 = center - float3(0.0f, radius, 0.0f);
		float3 c1 = center + float3(0.0f, radius, 0.0f);
		float sectorStep = 2.0f*PI/sides;
		float stackStep = PI/hSegments;
		float stackAngle, sectorAngle;
		float3 y0, y1;
		vector<float3> s;
		vector<float3> t;
		s.resize(sides + 1);
		t.resize(sides + 1);
		for(uint i = 0; i <= hSegments; ++i)
		{
			stackAngle = PI / 2 - i*stackStep;
			float xy = radius*cosf(stackAngle);
			y0.y = center.y + radius*sinf(stackAngle);
			for(uint j = 0; j <= sides; ++j)
			{
				sectorAngle = j*sectorStep;
				y0.x = center.x + xy*cosf(sectorAngle);
				y0.z = center.z + xy*sinf(sectorAngle);
				if(i > 0 && j > 0)
				{
					Poly poly = mesh->AddQuad(t[j - 1], t[j], y0, s[j - 1]);
					poly.SetNormal(-float3::Normal(t[j], y0, s[j - 1]));
					res.list.push_back(poly);
				}
				s[j] = y0;
				y1 = y0;
			}
			for(uint j = 0; j <= sides; ++j)
				t[j] = s[j];
		}
		return res;
	}

	void ParaCylinder::SetRadius(float radius)
	{
		_topRadius = _bottomRadius = radius;
	}

	void ParaCylinder::SetRadius(float bottomRadius, float topRadius)
	{
		_topRadius = topRadius;
		_bottomRadius = bottomRadius;
	}

	Polygons ParaCylinder::Generate(Mesh *mesh, const float3 &center)
	{
		float delta = (PI/sides)*2.0f;
		float a = 0.0f;
		Polygons res;
		float3 d0, d1, t0, t1, c0, c1;
		float x0, y0, x1, y1, tx0, ty0, tx1, ty1;
		c0 = center + float3(0.0f, 0.0f, 0.0f);
		c1 = center + float3(0.0f, height, 0.0f);
		for(uint i = 0; i <= sides; ++i)
		{
			x1 = _bottomRadius*cos(a);
			y1 = _bottomRadius*sin(a);
			tx1 = _topRadius*cos(a);
			ty1 = _topRadius*sin(a);
			if(i > 0)
			{
				d0 = center + float3(x0, 0.0f, y0);
				d1 = center + float3(tx0, height, ty0);
				t0 = center + float3(x1, 0.0f, y1);
				t1 = center + float3(tx1, height, ty1);
				Poly poly = mesh->AddQuad(d0, d1, t1, t0);
				poly.SetNormal(-float3::Normal(d0, d1, t1));
				res.list.push_back(poly);

				poly = mesh->AddTriangle(d0, t0, c0);
				poly.SetNormal(float3(0.0f, -1.0f, 0.0f));
				res.list.push_back(poly);
				poly = mesh->AddTriangle(t1, d1, c1);
				poly.SetNormal(float3(0.0f, 1.0f, 0.0f));
				res.list.push_back(poly);
			}
			x0 = x1;
			y0 = y1;
			tx0 = tx1;
			ty0 = ty1;
			a += delta;
		}
		return res;
	}

	Polygons ParaCone::Generate(Mesh* mesh, const float3 &center)
	{
		float delta = (PI/sides)*2.0f;
		float a = 0.0f;
		Polygons res;
		float3 d0, d1, t0, t1, c0, c1;
		float x0, y0, x1, y1;
		c0 = center + float3(0.0f, 0.0f, 0.0f);
		c1 = center + float3(0.0f, height, 0.0f);
		for(uint i = 0; i <= sides; ++i)
		{
			x1 = radius*cos(a);
			y1 = radius*sin(a);
			if(i > 0)
			{
				d0 = center + float3(x0, 0.0f, y0);
				t0 = center + float3(x1, 0.0f, y1);

				Poly poly = mesh->AddTriangle(d0, t0, c0);
				poly.SetNormal(float3(0.0f, -1.0f, 0.0f));
				res.list.push_back(poly);

				poly = mesh->AddTriangle(t0, d0, c1);
				if(smooth)
				{
					poly.SetNormal(-float3::Normal(t0, d0, c1));
				}
				else
					poly.SetNormal(-float3::Normal(t0, d0, c1));
				res.list.push_back(poly);
			}
			x0 = x1;
			y0 = y1;
			a += delta;
		}
		return res;
	}
}