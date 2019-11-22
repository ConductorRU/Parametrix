#pragma once
namespace Led
{

	class Canvas: public Actor
	{
	private:
		vector<Actor2d*> _items;
	public:
		Canvas();
		~Canvas();
		void AddItem(Actor2d* item);
		void Render();
	};
}