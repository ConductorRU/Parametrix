#include "stdafx.h"
#include "../Render/Header.h"
#include "../Material/Material.h"
#include "Actor.h"
#include "Mesh.h"
#include "Camera.h"
#include "Scene.h"


namespace Led
{
	Mesh::Mesh(InputLayout* il): RenderMesh(il)
	{
		
	}
	Mesh::~Mesh()
	{
	
	}
	void Mesh::BeforeRender()
	{
		UpdateBuffer();
	}
	void Mesh::Render(Scene* scene)
	{
		Vector lDir = Vector(1.0f, -1.0f, 3.0f);
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
			Vector pos = f->GetPosition();
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

	Polygons ParaBox::Generate(RenderMesh *mesh)
	{
		Vector xyz = (center - size)*0.5f;
		Vector XYZ = (center + size)*0.5f;
		xyz.y = center.y;
		XYZ.y = center.y + size.y;
		Polygons res;
		Vector xyZ = Vector(xyz.x, xyz.y, XYZ.z);
		Vector xYz = Vector(xyz.x, XYZ.y, xyz.z);
		Vector xYZ = Vector(xyz.x, XYZ.y, XYZ.z);

		Vector Xyz = Vector(XYZ.x, xyz.y, xyz.z);
		Vector XyZ = Vector(XYZ.x, xyz.y, XYZ.z);
		Vector XYz = Vector(XYZ.x, XYZ.y, xyz.z);

		Poly poly = mesh->AddQuad(xyz, xYz, XYz, Xyz);
		poly.SetNormal(Vector(0.0f, 0.0f, -1.0f));
		poly.SetUV({Vector2(0.0f, 0.0f), Vector2(1.0f, 0.0f), Vector2(1.0f, 1.0f), Vector2(0.0f, 1.0f)});
		res.list.push_back(poly);

		poly = mesh->AddQuad(XyZ, XYZ, xYZ, xyZ);
		poly.SetNormal(Vector(0.0f, 0.0f, 1.0f));
		poly.SetUV({Vector2(0.0f, 0.0f), Vector2(1.0f, 0.0f), Vector2(1.0f, 1.0f), Vector2(0.0f, 1.0f)});
		res.list.push_back(poly);

		poly = mesh->AddQuad(xyz, xyZ, xYZ, xYz);
		poly.SetNormal(Vector(-1.0f, 0.0f, 0.0f));
		poly.SetUV({Vector2(0.0f, 0.0f), Vector2(1.0f, 0.0f), Vector2(1.0f, 1.0f), Vector2(0.0f, 1.0f)});
		res.list.push_back(poly);

		poly = mesh->AddQuad(XYz, XYZ, XyZ, Xyz);
		poly.SetNormal(Vector(1.0f, 0.0f, 0.0f));
		poly.SetUV({Vector2(0.0f, 0.0f), Vector2(1.0f, 0.0f), Vector2(1.0f, 1.0f), Vector2(0.0f, 1.0f)});
		res.list.push_back(poly);

		poly = mesh->AddQuad(xyz, Xyz, XyZ, xyZ);
		poly.SetNormal(Vector(0.0f, -1.0f, 0.0f));
		poly.SetUV({Vector2(0.0f, 0.0f), Vector2(1.0f, 0.0f), Vector2(1.0f, 1.0f), Vector2(0.0f, 1.0f)});
		res.list.push_back(poly);

		poly = mesh->AddQuad(xYZ, XYZ, XYz, xYz);
		poly.SetNormal(Vector(0.0f, 1.0f, 0.0f));
		poly.SetUV({Vector2(0.0f, 0.0f), Vector2(1.0f, 0.0f), Vector2(1.0f, 1.0f), Vector2(0.0f, 1.0f)});
		res.list.push_back(poly);
		return res;
	}
	Polygons ParaPlane::Generate(RenderMesh *mesh)
	{
		Vector minBox = (center - Vector(size.x, 0.0f, size.y))*0.5f;
		Vector maxBox = (center + Vector(size.x, 0.0f, size.y))*0.5f;
		Polygons res;
		Poly poly = mesh->AddQuad(Vector(minBox.x, minBox.y, minBox.z), Vector(minBox.x, minBox.y, maxBox.z), Vector(maxBox.x, minBox.y, maxBox.z), Vector(maxBox.x, minBox.y, minBox.z));
		poly.SetUV({Vector2(0.0f, 0.0f), Vector2(1.0f, 0.0f), Vector2(1.0f, 1.0f), Vector2(0.0f, 1.0f)});
		poly.SetNormal(Vector(0.0f, 1.0f, 0.0f));
		res.list.push_back(poly);
		return res;
	}
	Polygons ParaSphere::Generate(RenderMesh *mesh)
	{
		float delta = (PI/sides)*2.0f;
		uint hSegments = sides/2 + sides%2;
		float deltaH = (radius*2.0f)/(float)hSegments;

		float a = 0.0f;
		Polygons res;
		Poly poly;
		Vector d0, d1, t0, t1;
		Vector c0 = center - Vector(0.0f, radius, 0.0f);
		Vector c1 = center + Vector(0.0f, radius, 0.0f);
		float x0, y0, x1, y1;
		float hPrev;
		float hCur;
		for(uint i = 0; i <= sides; ++i)
		{
			x1 = radius*cos(a);
			y1 = radius*sin(a);
			if(i > 0)
			{
				for(uint h = 0; h <= hSegments; ++h)
				{
					/*hCur = center.y + radius + (float)h*deltaH;
					if(h == 1)
					{
						poly = mesh->AddTriangle(c1, t0, c0);
						res.list.push_back(poly);
					}
					d0 = center + Vector(x0, 0.0f, y0);
					d1 = center + Vector(x0, height, y0);
					t0 = center + Vector(x1, 0.0f, y1);
					t1 = center + Vector(x1, height, y1);
					Poly poly = mesh->AddQuad(d0, d1, t1, t0);
					poly.SetNormal(-Vector::Normal(d0, d1, t1));
					res.list.push_back(poly);

					poly = mesh->AddTriangle(d0, t0, c0);
					poly.SetNormal(Vector(0.0f, -1.0f, 0.0f));
					res.list.push_back(poly);
					poly = mesh->AddTriangle(t1, d1, c1);
					poly.SetNormal(Vector(0.0f, 1.0f, 0.0f));
					res.list.push_back(poly);

					hPrev = hCur;*/
				}
			}
			x0 = x1;
			y0 = y1;
			a += delta;
		}
		return res;
	}
	Polygons ParaCylinder::Generate(RenderMesh *mesh)
	{
		float delta = (PI/sides)*2.0f;
		float a = 0.0f;
		Polygons res;
		Vector d0, d1, t0, t1, c0, c1;
		float x0, y0, x1, y1;
		c0 = center + Vector(0.0f, 0.0f, 0.0f);
		c1 = center + Vector(0.0f, height, 0.0f);
		for(uint i = 0; i <= sides; ++i)
		{
			x1 = radius*cos(a);
			y1 = radius*sin(a);
			if(i > 0)
			{
				d0 = center + Vector(x0, 0.0f, y0);
				d1 = center + Vector(x0, height, y0);
				t0 = center + Vector(x1, 0.0f, y1);
				t1 = center + Vector(x1, height, y1);
				Poly poly = mesh->AddQuad(d0, d1, t1, t0);
				poly.SetNormal(-Vector::Normal(d0, d1, t1));
				res.list.push_back(poly);

				poly = mesh->AddTriangle(d0, t0, c0);
				poly.SetNormal(Vector(0.0f, -1.0f, 0.0f));
				res.list.push_back(poly);
				poly = mesh->AddTriangle(t1, d1, c1);
				poly.SetNormal(Vector(0.0f, 1.0f, 0.0f));
				res.list.push_back(poly);
			}
			x0 = x1;
			y0 = y1;
			a += delta;
		}
		return res;
	}
	Polygons ParaCone::Generate(RenderMesh* mesh)
	{
		float delta = (PI/sides)*2.0f;
		float a = 0.0f;
		Polygons res;
		Vector d0, d1, t0, t1, c0, c1;
		float x0, y0, x1, y1;
		c0 = center + Vector(0.0f, 0.0f, 0.0f);
		c1 = center + Vector(0.0f, height, 0.0f);
		for(uint i = 0; i <= sides; ++i)
		{
			x1 = radius*cos(a);
			y1 = radius*sin(a);
			if(i > 0)
			{
				d0 = center + Vector(x0, 0.0f, y0);
				t0 = center + Vector(x1, 0.0f, y1);

				Poly poly = mesh->AddTriangle(d0, t0, c0);
				poly.SetNormal(Vector(0.0f, -1.0f, 0.0f));
				res.list.push_back(poly);

				poly = mesh->AddTriangle(t0, d0, c1);
				if(smooth)
				{
					poly.SetNormal(-Vector::Normal(t0, d0, c1));
				}
				else
					poly.SetNormal(-Vector::Normal(t0, d0, c1));
				res.list.push_back(poly);
			}
			x0 = x1;
			y0 = y1;
			a += delta;
		}
		return res;
	}
}