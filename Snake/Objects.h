#pragma once
#include "Fruit.h"
#include "Wall.h"
#include "Snake.h"

namespace GameManager
{
	struct Objects
	{
		Fruit	fruit;
		Wall	wall;
		Snake	snake;
		
		Objects(ID3D11Device *device)
			: wall(device),
			fruit(device),
			snake(device)
		{

		}
	};
}