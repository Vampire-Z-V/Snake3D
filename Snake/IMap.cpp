#include "IMap.h"
using namespace GameManager;


IMap::IMap()
{
	m_BoxWallPos.clear();
	m_BoxWallPos.push_back({ 1.0f,1.0f,1.0f });
	m_BoxWallPos.push_back({ 10.0f,10.0f,10.0f });
}


IMap::~IMap()
{
}

const std::vector<DirectX::XMFLOAT3>* IMap::GetFromMap()
{
	return &m_BoxWallPos;
}
