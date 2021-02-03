#include "stdafx.h"
#include "Shape.h"
#include "Canvas.h"
namespace Led
{
	Canvas::Canvas(InputLayout *input, Material *material)
	{
		_input = input;
		_material = material;
	}
	Canvas::~Canvas()
	{
		
	}
	ShapeRectangle* Canvas::CreateRectangle(float width, float height)
	{
		unique_ptr<ShapeRectangle> shape = make_unique<ShapeRectangle>(_input, _material, width, height);
		ShapeRectangle* res = shape.get();
		_shapes.insert(move(shape));
		return res;
	}
	ShapePath *Canvas::CreatePath(const string &desc)
	{
		unique_ptr<ShapePath> shape = make_unique<ShapePath>(_input, _material, desc);
		ShapePath* res = shape.get();
		_shapes.insert(move(shape));
		return res;
	}

	void Canvas::BeforeRender()
	{
		for(const unique_ptr<Shape>& shape : _shapes)
		{
			((Actor2D*)shape.get())->Update();
			((Actor2D*)shape.get())->BeforeRender();
		}
	}

	void Canvas::Render(Scene *scene)
	{
		for(const unique_ptr<Shape> &shape : _shapes)
			((Actor2D*)shape.get())->Render(scene);
	}
}