#pragma once
#include "../../Scene/Actor.h"

namespace Led
{
	class Character : public Actor
	{
	private:
		class Mesh* _mesh;
	public:
		Character();
		~Character();
		void Render(class Scene* scene);
	};
}