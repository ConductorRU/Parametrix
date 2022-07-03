#pragma once
namespace Led
{
	class Canvas
	{
	private:
		set<unique_ptr<class Shape>> _shapes;
		class InputLayout *_input;
		class Material *_material;
	public:
		InputLayout *GetInputLayout() {return _input; };
		Canvas(InputLayout *input, Material *material);
		~Canvas();
		class ShapeRectangle* CreateRectangle(float width = 100.0f, float height = 100.0f);
		class ShapePath *CreatePath(const string &desc);
		void BeforeRender();
		void Render(class Scene *scene);
	};
}