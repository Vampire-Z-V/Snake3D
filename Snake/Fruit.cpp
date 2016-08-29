#include "Fruit.h"
using namespace GameManager;


Fruit::Fruit(ID3D11Device *device)
	:_BaseGameObject(device)
{
}


Fruit::~Fruit()
{
}

void GameManager::Fruit::Update()
{
}

void GameManager::Fruit::Render()
{
}

MathHelper::BOUNDING GameManager::Fruit::GetImpactBox(BUFFER_TYPE type)
{
	return MathHelper::BOUNDING();
}

const std::vector<XMFLOAT4X4>* GameManager::Fruit::GetWorld(BUFFER_TYPE type)
{
	return nullptr;
}
