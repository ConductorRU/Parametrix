#pragma once
#include "Math/Header.h"
#include "Core/Event.h"
#include "../Render/Header.h"
#include "Actor2D.h"
namespace Led
{
	class InputLayout;
	class Scene;

	class EventListener2D
	{
	private:
		bool z_isHover;
	public:
		Event<unsigned int> onClick;
	};

	class Shape: public Actor2D, protected RenderMesh
	{
	protected:
		class Material *_mat;
		bool _isUpdate;
		int _sort;
		Color _color;
		Color _borderColor;
	public:
		Shape(InputLayout *io, Material* mat);
		~Shape();
	};

	class ShapeRectangle: public Shape
	{
	private:
		float _borderRadius[4];
	public:
		Event<unsigned int> onClick;
		ShapeRectangle(InputLayout *io, Material *mat, float width = 100.0f, float height = 100.0f);
		~ShapeRectangle();

		void Bake();
		void BeforeRender() override;
		void Render(Scene* scene) override;
	};

	struct ShapeVector
	{
		ShapeVector *prev = nullptr;
		ShapeVector *next = nullptr;
		float2 pos;
	};

	class ShapePath: public Shape
	{
	private:
		float2 _cursor;
		vector<ShapeVector*> _points;
	public:
		ShapePath(InputLayout *io, Material *mat, const string &desc);
		~ShapePath();
		void SetCursor(const float2 &pos);
		ShapeVector *AddLine(const float2 &nextPos);
		void Close();
		void Bake();
		void Render(Scene *scene);
	};
}
/*


*/