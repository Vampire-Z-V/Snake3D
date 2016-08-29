#include "Wall.h"
#include "IRender.h"
using namespace GameManager;

namespace GameManager
{
	extern IRender g_CRender;
}

Wall::Wall(ID3D11Device *device)
:	_BaseGameObject(device)
{
	Create_VB_and_IB(&m_pWallVB, &m_pWallIB, m_GridInfo, GRID);
	Create_VB_and_IB(&m_pWallVB, &m_pWallIB, m_BoxInfo, BOX);

	// Create around walls;
	m_GridWorld.resize(6);
	XMMATRIX I = XMMatrixIdentity();
	XMStoreFloat4x4(&m_GridWorld[0], I);

	XMMATRIX rotateAboutX_Front = XMMatrixRotationX(-0.5f*MathHelper::Pi);
	XMMATRIX offset_Front = XMMatrixTranslation(0.0f, 50.0f, +50.0f);
	XMStoreFloat4x4(&m_GridWorld[1], XMMatrixMultiply(rotateAboutX_Front, offset_Front));

	XMMATRIX rotateAboutX_Back = XMMatrixRotationX(+0.5f*MathHelper::Pi);
	XMMATRIX offset_Back = XMMatrixTranslation(0.0f, 50.0f, -50.0f);
	XMStoreFloat4x4(&m_GridWorld[2], XMMatrixMultiply(rotateAboutX_Back, offset_Back));

	XMMATRIX rotateAboutZ_Left = XMMatrixRotationZ(-0.5f*MathHelper::Pi);
	XMMATRIX offset_Left = XMMatrixTranslation(-50.0f, 50.0f, 0.0f);
	XMStoreFloat4x4(&m_GridWorld[3], XMMatrixMultiply(rotateAboutZ_Left, offset_Left));

	XMMATRIX rotateAboutZ_Right = XMMatrixRotationZ(+0.5f*MathHelper::Pi);
	XMMATRIX offset_Right = XMMatrixTranslation(+50.0f, 50.0f, 0.0f);
	XMStoreFloat4x4(&m_GridWorld[4], XMMatrixMultiply(rotateAboutZ_Right, offset_Right));

	XMMATRIX offset_Up = XMMatrixTranslation(0.0f, 100.0f, 0.0f);
	XMStoreFloat4x4(&m_GridWorld[5], offset_Up);

	m_GridInfo.Material.Ambient= XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	m_GridInfo.Material.Diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	m_GridInfo.Material.Specular = XMFLOAT4(0.8f, 0.8f, 0.8f, 16.0f);
	m_GridInfo.Style = GRID;
	m_GridBounding = CreateImpactBox(m_GridInfo);

	// Create box walls;
	m_BoxWorld.clear();
	vector<XMFLOAT3>* pBoxPosTemp = const_cast<vector<XMFLOAT3>*>(m_BoxPos.GetFromMap());
	XMFLOAT4X4 boxWorldTemp;
	XMMATRIX scale = XMMatrixScaling(2.0f, 2.0f, 2.0f);
	for (size_t i = 0; i < pBoxPosTemp->size(); ++i)
	{
		XMMATRIX offset = XMMatrixTranslation(m_BoxInfo.BaseInfo.BoxInfo.Length / 2 * (*pBoxPosTemp)[i].x,
											  m_BoxInfo.BaseInfo.BoxInfo.Height / 2 * (*pBoxPosTemp)[i].y,
											  m_BoxInfo.BaseInfo.BoxInfo.Width / 2 * (*pBoxPosTemp)[i].z);
		XMStoreFloat4x4(&boxWorldTemp, XMMatrixMultiply(offset, scale));
		m_BoxWorld.push_back(boxWorldTemp);
	}
	m_BoxInfo.Material.Ambient = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	m_BoxInfo.Material.Diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	m_BoxInfo.Material.Specular = XMFLOAT4(0.8f, 0.8f, 0.8f, 16.0f);
	m_BoxInfo.Style = BOX;
	m_BoxBounding = CreateImpactBox(m_BoxInfo);

	m_SRV.clear();
	m_SRV.push_back(CreateSRVFromFile(L"Resoure/Textures/Wall_0.dds"));
	m_SRV.push_back(CreateSRVFromFile(L"Resoure/Textures/Wall_1.dds"));
	m_SRV.push_back(CreateSRVFromFile(L"Resoure/Textures/Wall_2.dds"));
	m_SRV.push_back(CreateSRVFromFile(L"Resoure/Textures/Wall_3.dds"));
	m_SRV.push_back(CreateSRVFromFile(L"Resoure/Textures/Wall_4.dds"));
	m_SRV.push_back(CreateSRVFromFile(L"Resoure/Textures/Wall_5.dds"));

}



Wall::~Wall()
{
	ReleaseCOM(m_pWallVB);
	ReleaseCOM(m_pWallIB);
}

void GameManager::Wall::Update()
{
}

void Wall::Render()
{
	g_CRender.DrawWall(m_GridWorld, m_BoxWorld, m_GridInfo, m_BoxInfo, m_pWallVB, m_pWallIB, m_SRV);
}

MathHelper::BOUNDING Wall::GetImpactBox(BUFFER_TYPE type)
{
	switch (type)
	{
		case GRID:
			return m_GridBounding;
		case BOX:
			return m_BoxBounding;
		default:
			return MathHelper::BOUNDING();
	}

}

const std::vector<XMFLOAT4X4>* Wall::GetWorld(BUFFER_TYPE type)
{
	switch (type)
	{
		case GRID:
			return &m_GridWorld;
		case BOX:
			return &m_BoxWorld;
		default:
			return nullptr;
	}
}
