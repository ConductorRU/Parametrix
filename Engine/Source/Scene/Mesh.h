#pragma once
namespace Led
{
	class Mesh: public Actor, public RenderMesh
	{
	private:
		class Material *_material;
	public:
		Mesh(InputLayout* il);
		~Mesh();
		void SetMaterial(Material *material) { _material = material; RenderMesh::SetMaterial((RenderMaterial*)_material); }
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
		Vector center;
		Vector size;
		Polygons Generate(RenderMesh *mesh);
	};
	struct ParaPlane
	{
		Vector center;
		Vector2 size;
		Polygons Generate(RenderMesh *mesh);
	};
	struct ParaSphere
	{
		Vector center;
		float radius;
		uint sides;
		Polygons Generate(RenderMesh *mesh);
	};
	struct ParaCylinder
	{
		Vector center;
		float radius;
		float height;
		uint sides;
		Polygons Generate(RenderMesh *mesh);
	};
	struct ParaCone
	{
		Vector center;
		float radius = 0.5f;
		float height = 1.0f;
		uint sides = 16;
		bool smooth = true;
		Polygons Generate(RenderMesh *mesh);
	};
}