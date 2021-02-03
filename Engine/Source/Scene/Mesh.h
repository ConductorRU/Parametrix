#pragma once
#include "../Render/Header.h"
#include "Actor.h"
namespace Led
{
	struct Poly
	{
		vector<Vertex*> list;
		void SetNormal(const float3& nor);
		void SetColor(const Color& color);
		void SetUV(const initializer_list<float2> uv);
	};

	struct Polygons
	{
		vector<Poly> list;
		void SetNormal(const float3& nor);
		void SetColor(const Color& color);
	};

	class Mesh: public Actor, public RenderMesh
	{
	private:
		class Material *_material;
	public:
		Mesh(InputLayout* il);
		~Mesh();
		Poly AddTriangle(const float3 &pos0, const float3 &pos1, const float3 &pos2);
		Poly AddQuad(const float3 &pos0, const float3 &pos1, const float3 &pos2, const float3 &pos3);
		void SetMaterial(Material *material)
		{
			_material = material;
			RenderMesh::SetMaterial((RenderMaterial*)_material);
		}
		Material* GetMaterial() { return _material; }
		void BeforeRender() override;
		void Render(class Scene* scene) override;
	};
	namespace ParaOp
	{
		void Bridge(RenderMesh* mesh, Poly from, Poly to);
	};
	struct ParaBox
	{
		float3 scale = float3::ONE;
		float3 size;
		Polygons Generate(Mesh *mesh, const float3 &center = float3::ZERO);
	};
	struct ParaPlane
	{
		float2 scale = float2::ONE;
		float2 size;
		Polygons Generate(Mesh *mesh, const float3 &center = float3::ZERO);
	};
	struct ParaSphere
	{
		float3 scale = float3::ONE;
		float radius = 0.5f;
		uint sides = 32;
		Polygons Generate(Mesh *mesh, const float3 &center = float3::ZERO);
	};
	struct ParaCylinder
	{
	private:
		float _topRadius = 0.5f;
		float _bottomRadius = 0.5f;
	public:
		float3 scale = float3::ONE;
		float height = 1.0f;
		uint sides = 16;
		void SetRadius(float radius);
		void SetRadius(float bottomRadius, float topRadius);
		Polygons Generate(Mesh *mesh, const float3 &center = float3::ZERO);
	};
	struct ParaCone
	{
		float3 scale = float3::ONE;
		float radius = 0.5f;
		float height = 1.0f;
		uint sides = 16;
		bool smooth = true;
		Polygons Generate(Mesh *mesh, const float3 &center = float3::ZERO);
	};
}