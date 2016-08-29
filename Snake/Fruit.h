#pragma once
#include "_BaseGameObject.h"
namespace GameManager
{
	class Fruit :
		public _BaseGameObject
	{
	public:
		Fruit(ID3D11Device *device);
		~Fruit();

		void Update();
		void Render();
		MathHelper::BOUNDING GetImpactBox(BUFFER_TYPE type);
		const std::vector<XMFLOAT4X4>* GetWorld(BUFFER_TYPE type);
	};
}
