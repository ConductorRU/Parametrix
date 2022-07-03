#include "stdafx.h"
#include "Math/Transform.h"
#include "Math/Color.h"
#include "Render/DirectX12/RenderMesh.h"
#include "Material/Header.h"
#include "Render/Render.h"
#include "Scene/Manager.h"
#include "Scene/Header.h"
#include "Core/Header.h"
#include "2D/Shape.h"

namespace Led
{
	Shape::Shape(InputLayout *io, Material* mat): RenderMesh(io)
	{
		_mat = mat;
		_isUpdate = true;
		RenderMesh::SetMaterial((RenderMaterial*)_mat);
	}

	Shape::~Shape()
	{
	
	}

	ShapeRectangle::ShapeRectangle(InputLayout *io, Material *mat, float width, float height): Shape(io, mat)
	{
		SetSize(width, height);
	}
	ShapeRectangle::~ShapeRectangle()
	{
	
	}
	void ShapeRectangle::Bake()
	{
		InputLayout *layout = GetInputLayout();
		if(!layout || !layout->GetSize())
			return;
		UINT lSize = (UINT)layout->GetSize();
		UINT vSize = 4;
		UINT pos = 0;
		Vertex* v = CreateVertex();
		v->SetPosition2D(0.0f, 0.0f);
		v->SetColor(_color);
		v->SetUV(0.0f, 0.0f);

		v = CreateVertex();
		v->SetPosition2D(_size.x, 0.0f);
		v->SetColor(_color);
		v->SetUV(1.0f, 0.0f);

		v = CreateVertex();
		v->SetPosition2D(0.0f, _size.y);
		v->SetColor(_color);
		v->SetUV(0.0f, 1.0f);

		v = CreateVertex();
		v->SetPosition2D(_size.x, _size.y);
		v->SetColor(_color);
		v->SetUV(1.0f, 1.0f);

		SetTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	}

	void ShapeRectangle::BeforeRender()
	{
		if(_isUpdate)
		{
			Bake();
			_isUpdate = false;
		}
		UpdateBuffer();
		if(Engine::Get()->GetInput()->IsKeyHit(KEY_SPACE))
			onClick(1);
	}

	void ShapeRectangle::Render(Scene *scene)
	{
		if(_mat)
		{
			Camera *cam = scene->GetCamera();
			Matrix4 view = cam->GetView();
			Matrix4 proj = cam->GetProjection();
			Matrix4 wrld = GetWorldTransform().ToMatrix4();
			Matrix4 wvp = (wrld*view)*proj;
			float3 camPos = cam->GetWorldTransform().GetPosition();
			Vector4 screen;
			screen.x = 1.0f/(float)Render::Get()->GetWidth();
			screen.y = 1.0f/(float)Render::Get()->GetHeight();
			//view = view.Transpose();
			//proj = proj.Transpose();
			//wrld = wrld.Transpose();
			//wvp = wvp.Transpose();
			Color col = Color(1.0f, 1.0f, 1.0f);
			SetConst(SHADER_VS, "ConstantBuffer", "screen", screen);
			SetConst(SHADER_VS, "ConstantBuffer", "rect", wrld);
			SetConst(SHADER_VS, "ConstantBuffer", "color", col);
			if(_mat)
				_mat->Render();
			RenderMesh::Render(_mat);
		}
	}

	ShapePath::ShapePath(InputLayout *io, Material *mat, const string &desc): Shape(io, mat)
	{
		///Set(desc);
	}

	ShapePath::~ShapePath()
	{
		
	}

	void ShapePath::SetCursor(const float2 &pos)
	{
		_cursor = pos;
	}

	ShapeVector *ShapePath::AddLine(const float2 &nextPos)
	{
		ShapeVector *next = new ShapeVector;
		next->pos = nextPos;
		if(_points.size())
		{
			_points[_points.size() - 1u]->next = next;
			next->prev = _points[_points.size() - 1u];
		}
		else
		{
			ShapeVector *start = new ShapeVector;
			start->pos = _cursor;
			start->next = next;
			_points.push_back(start);
			next->prev = start;
		}
		_points.push_back(next);
		return next;
	}

	void ShapePath::Close()
	{
		if(_points.size() > 1u)
		{
			_points[0]->prev = _points[_points.size() - 1u];
			_points[_points.size() - 1u]->next = _points[0];
		}
	}

	void ShapePath::Bake()
	{
		
	}

	void ShapePath::Render(Scene *scene)
	{
		
	}
}