#include "Snake.h"
#include "IRender.h"
using namespace GameManager;
using namespace DirectX;

namespace GameManager
{
	extern IRender g_CRender;
}

Snake::Snake(ID3D11Device *device)
:	_BaseGameObject(device),
	m_pSnakeVB(nullptr),
	m_pSnakeIB(nullptr),
	m_BodyLength(0)
{
	Create_VB_and_IB(&m_pSnakeVB, &m_pSnakeIB, m_HeadInfo, GEOSPHERE);
	Create_VB_and_IB(&m_pSnakeVB, &m_pSnakeIB, m_BodyInfo, GEOSPHERE);

	

	//XMMATRIX I = XMMatrixIdentity();
	//m_URL[0] = I;
	//m_SL[0] = MathHelper::Zero;

	m_SRV.clear();
	m_SRV.push_back(CreateSRVFromFile(L"Resoure/Textures/Snake_head.dds"));
	m_SRV.push_back(CreateSRVFromFile(L"Resoure/Textures/Snake_body.dds"));

}

Snake::~Snake()
{
}

void GameManager::Snake::Init()
{
	// Create head;
	//XMMATRIX I = XMMatrixIdentity();
	//XMStoreFloat4x4(&m_HeadWorld, I);
	//XMMATRIX scale = XMMatrixScaling(2.0f, 2.0f, 2.0f);
	XMMATRIX rotate = XMMatrixRotationY(0.5f*XM_PI);
	XMMATRIX offset = XMMatrixTranslation(0.0f,50.0f, 0.0f);
	XMStoreFloat4x4(&m_HeadWorld, XMMatrixMultiply(rotate, offset));

	m_HeadCoordSystem.Position = { 0.0f,50.0f,0.0f };
	m_HeadCoordSystem.Right = { 1.0f,0.0f,0.0f };
	m_HeadCoordSystem.Up = { 0.0f,1.0f,0.0f };
	m_HeadCoordSystem.Look = { 0.0f,0.0f,1.0f };

	m_HeadInfo.Material.Ambient = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	m_HeadInfo.Material.Diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	m_HeadInfo.Material.Specular = XMFLOAT4(0.8f, 0.8f, 0.8f, 16.0f);
	m_HeadInfo.Style = GEOSPHERE;
	m_HeadBounding = CreateImpactBox(m_HeadInfo);

	// Create body;
	m_BodyWorld.clear();
	m_BodyPos.clear();
	m_BodyCoordSystem.clear();
	m_ToLastBodyDistance.clear();
	//m_URL.clear();
	m_SL.clear();
	m_HeadTurnPos.clear();
	m_CoordSystemTemp.clear();
	m_HeadWorldTemp.clear();
	//m_URL.resize(1);
	//m_SL.resize(1);

	CreateNewBody(m_HeadCoordSystem.Position,
				  m_HeadInfo.BaseInfo.GeosphereInfo.Radius + m_BodyInfo.BaseInfo.GeosphereInfo.Radius,
				  m_HeadWorld,
				  m_HeadCoordSystem);
	
	m_BodyInfo.Material.Ambient = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	m_BodyInfo.Material.Diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	m_BodyInfo.Material.Specular = XMFLOAT4(0.8f, 0.8f, 0.8f, 16.0f);
	m_BodyInfo.Style = GEOSPHERE;
	m_BodyBounding = CreateImpactBox(m_BodyInfo);

	ResetData();
}

void GameManager::Snake::Update()
{
	// Update body.
	//m_BodyWorld.insert(m_BodyWorld.begin(), m_HeadWorld);
	//m_BodyPos.insert(m_BodyPos.begin(), m_HeadPrevPos);
	//if (m_BodyLength > 0)
	UpdateBody();

	// Update head.
	XMMATRIX prevWorld = XMLoadFloat4x4(&m_HeadWorld);
	UpdateHeadWorld(XMLoadFloat4x4(&m_HeadRotate_Up),
					XMLoadFloat4x4(&m_HeadRotate_Right),
					XMLoadFloat4x4(&m_HeadRotate_Look),
					m_HeadPrevPos,
					m_HeadCoordSystem.Position,
					prevWorld);
	ResetData();
}

void GameManager::Snake::Render()
{
	g_CRender.DrawSnake(m_HeadWorld, m_BodyWorld, m_HeadInfo, m_BodyInfo, m_pSnakeVB, m_pSnakeIB, m_SRV);
}

MathHelper::BOUNDING GameManager::Snake::GetImpactBox(SNAKE_TYPE type, int bodyNum)
{
	MathHelper::BOUNDING temp;
	switch (type)
	{
		case HEAD:
			temp = m_HeadBounding;
			temp.Sphere.CenterPos = m_HeadCoordSystem.Position;
			return temp;
		case BODY:
			temp = m_BodyBounding;
			temp.Sphere.CenterPos = m_BodyPos[bodyNum];
			return temp;
		default:
			return temp;
	}
}

XMFLOAT3 GameManager::Snake::GetHeadUp()
{
	return m_HeadCoordSystem.Up;
}

XMFLOAT3 GameManager::Snake::GetHeadRight()
{
	return m_HeadCoordSystem.Right;
}

XMFLOAT3 GameManager::Snake::GetHeadLook()
{
	return m_HeadCoordSystem.Look;
}

XMFLOAT3 GameManager::Snake::GetHeadPos()
{
	return m_HeadCoordSystem.Position;
}

void GameManager::Snake::Walk(float d)
{
	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR l = XMLoadFloat3(&m_HeadCoordSystem.Look);
	XMVECTOR p = XMLoadFloat3(&m_HeadCoordSystem.Position);
	m_HeadPrevPos = m_HeadCoordSystem.Position;
	XMStoreFloat3(&m_HeadCoordSystem.Position, XMVectorMultiplyAdd(s, l, p));
	//m_SL.insert(m_SL.begin(), s*l);
	///m_SL.insert(m_SL.begin(), s);
	m_Speed = d;
}

void GameManager::Snake::RotateUp(float angle)
{
	XMMATRIX R = XMMatrixRotationAxis(XMLoadFloat3(&m_HeadCoordSystem.Up), angle);
	XMStoreFloat4x4(&m_HeadRotate_Up, R);

	XMStoreFloat3(&m_HeadCoordSystem.Right, XMVector3TransformNormal(XMLoadFloat3(&m_HeadCoordSystem.Right), R));
	XMStoreFloat3(&m_HeadCoordSystem.Look, XMVector3TransformNormal(XMLoadFloat3(&m_HeadCoordSystem.Look), R));
}

void GameManager::Snake::RotateRight(float angle)
{
	XMMATRIX R = XMMatrixRotationAxis(XMLoadFloat3(&m_HeadCoordSystem.Right), angle);
	XMStoreFloat4x4(&m_HeadRotate_Right, R);

	XMStoreFloat3(&m_HeadCoordSystem.Up, XMVector3TransformNormal(XMLoadFloat3(&m_HeadCoordSystem.Up), R));
	XMStoreFloat3(&m_HeadCoordSystem.Look, XMVector3TransformNormal(XMLoadFloat3(&m_HeadCoordSystem.Look), R));
}

void GameManager::Snake::RotateLook(float angle)
{
	XMMATRIX R = XMMatrixRotationAxis(XMLoadFloat3(&m_HeadCoordSystem.Look), angle);
	XMStoreFloat4x4(&m_HeadRotate_Look, R);

	XMStoreFloat3(&m_HeadCoordSystem.Right, XMVector3TransformNormal(XMLoadFloat3(&m_HeadCoordSystem.Right), R));
	XMStoreFloat3(&m_HeadCoordSystem.Up, XMVector3TransformNormal(XMLoadFloat3(&m_HeadCoordSystem.Up), R));
}

void GameManager::Snake::IncreaseBodyLength()
{
	CreateNewBody(m_BodyPos[m_BodyLength],
				  2 * m_BodyInfo.BaseInfo.GeosphereInfo.Radius,
				  m_BodyWorld[m_BodyLength],
				  m_BodyCoordSystem[m_BodyLength]);
	m_BodyLength++;
}

void GameManager::Snake::ResetData()
{
	//XMMATRIX I = XMMatrixIdentity();
	XMStoreFloat4x4(&m_HeadRotate_Up, MathHelper::Identity);
	XMStoreFloat4x4(&m_HeadRotate_Right, MathHelper::Identity);
	XMStoreFloat4x4(&m_HeadRotate_Look, MathHelper::Identity);
	m_HeadPrevPos = m_HeadCoordSystem.Position;

	while (m_BodyWorld.size() > m_BodyLength + 1)
	{
		m_BodyWorld.pop_back();
	}
	while (m_BodyPos.size() > m_BodyLength + 1)
	{
		m_BodyPos.pop_back();
	}
	while (m_SL.size() > m_BodyLength + 1)
	{
		m_SL.pop_back();
	}

	/*if (m_TurnCount.size() > 0)
	{
		while (m_TurnCount.size() > 0 && m_TurnCount[0] > m_BodyLength)
		{
			m_TurnCount.erase(m_TurnCount.begin());
			m_HeadTurnPos.erase(m_HeadTurnPos.begin());
			m_URL.erase(m_URL.begin());

			m_CoordSystemTemp.erase(m_CoordSystemTemp.begin());
			m_HeadWorldTemp.erase(m_HeadWorldTemp.begin());
		}
	}*/
	while ((m_TurnCount.size() > 0 && m_TurnCount[m_TurnCount.size() - 1] > m_BodyLength))
	//while (m_TurnCount.size() > 0 && m_TurnCount[m_TurnCount.size() - 1] > m_BodyLength)
	{
		m_TurnCount.pop_back();
		m_HeadTurnPos.pop_back();
		//m_URL.pop_back();
		m_CoordSystemTemp.pop_back();
		m_HeadWorldTemp.pop_back();
	}
}

void GameManager::Snake::UpdateHeadWorld(XMMATRIX &rotateAboutUp, XMMATRIX &rotateAboutRight, XMMATRIX &rotateAboutLook, XMFLOAT3 & headPrevPos, XMFLOAT3 & headCurrPos, XMMATRIX & headPrevWorld)
{
	// currWorld = prevWorld * prevOffset^(-1) * U * R * L * currOffset(to currPos)
	XMMATRIX prevOffset = XMMatrixTranslation(headPrevPos.x, headPrevPos.y, headPrevPos.z);
	XMMATRIX currOffset = XMMatrixTranslation(headCurrPos.x, headCurrPos.y, headCurrPos.z);
	XMMATRIX url = rotateAboutUp*rotateAboutRight*rotateAboutLook;
	headPrevWorld *= (XMMatrixInverse(&XMMatrixDeterminant(prevOffset), prevOffset)*url*currOffset);
	XMStoreFloat4x4(&m_HeadWorld, headPrevWorld);

	if(!XMMatrixIsIdentity(url))
	{
		//m_URL.insert(m_URL.begin(), url);
		m_HeadTurnPos.insert(m_HeadTurnPos.begin(), m_HeadCoordSystem.Position);
		m_TurnCount.insert(m_TurnCount.begin(), 0);
		m_CoordSystemTemp.insert(m_CoordSystemTemp.begin(), m_HeadCoordSystem);
		m_HeadWorldTemp.insert(m_HeadWorldTemp.begin(), m_HeadWorld);
	}
	//m_URL.insert(m_URL.begin(), url);
}

void GameManager::Snake::CreateNewBody(XMFLOAT3 lastBodyPos, float distance, XMFLOAT4X4 lastBodyWorld, MathHelper::COORDSYSTEM lastSystem)
{
	XMVECTOR s = XMVectorReplicate(-distance);
	XMVECTOR l = XMLoadFloat3(&lastSystem.Look);
	XMVECTOR p = XMLoadFloat3(&lastBodyPos);
	XMFLOAT3 currPos;
	XMStoreFloat3(&currPos, XMVectorMultiplyAdd(s, l, p));
	m_BodyPos.push_back(currPos);

	XMMATRIX offset = XMMatrixTranslation(currPos.x - lastBodyPos.x,
										  currPos.y - lastBodyPos.y,
										  currPos.z - lastBodyPos.z);
	XMMATRIX lastWorld = XMLoadFloat4x4(&lastBodyWorld);
	XMFLOAT4X4 currWorld;
	XMStoreFloat4x4(&currWorld, lastWorld*offset);
	m_BodyWorld.push_back(currWorld);

	lastSystem.Position = currPos;
	m_BodyCoordSystem.push_back(lastSystem);
	m_ToLastBodyDistance.push_back(distance);
}

void GameManager::Snake::UpdateBody()
{
	XMVECTOR speed = XMVectorReplicate(m_Speed);
	std::vector<XMFLOAT3> prevPos = m_BodyPos;
	MathHelper::COORDSYSTEM lastSystem;
	XMFLOAT3 lastBodyPos;
	for (size_t i = 0; i < m_BodyLength + 1; ++i)
	{
		if (i == 0)
		{
			lastSystem = m_HeadCoordSystem;
			lastBodyPos = m_HeadCoordSystem.Position;
		}
		else
		{
			lastSystem = m_BodyCoordSystem[i - 1];
			lastBodyPos = m_BodyCoordSystem[i - 1].Position;
		}

		XMVECTOR s = XMVectorReplicate(-0.1*m_Speed);
		XMVECTOR l = XMLoadFloat3(&lastSystem.Look);
		XMVECTOR p = XMLoadFloat3(&lastBodyPos);
		XMFLOAT3 currPos;
		XMStoreFloat3(&currPos, XMVectorMultiplyAdd(s, l, p));

		XMFLOAT3 look = {
			currPos.x - m_BodyCoordSystem[i].Position.x,
			currPos.y - m_BodyCoordSystem[i].Position.y,
			currPos.z - m_BodyCoordSystem[i].Position.z
		};
		XMVECTOR L = XMLoadFloat3(&look);
		XMVECTOR P = XMLoadFloat3(&m_BodyPos[i]);
		//XMStoreFloat3(&m_BodyPos[i], XMVectorAdd(m_SL[i], p));
		XMStoreFloat3(&m_BodyPos[i], XMVectorMultiplyAdd(speed, L, P));

		XMMATRIX prevOffset = XMMatrixTranslation(prevPos[i].x, prevPos[i].y, prevPos[i].z);
		XMMATRIX currOffset = XMMatrixTranslation(m_BodyPos[i].x, m_BodyPos[i].y, m_BodyPos[i].z);
		XMMATRIX worldTemp = XMLoadFloat4x4(&m_BodyWorld[i]);
		worldTemp *= (XMMatrixInverse(&XMMatrixDeterminant(prevOffset), prevOffset) * currOffset);
		XMStoreFloat4x4(&m_BodyWorld[i], worldTemp);

		m_ToLastBodyDistance[i] -= m_Speed;
		m_BodyCoordSystem[i].Position = m_BodyPos[i];
		m_BodyCoordSystem[i].Look = look;
		/*if (m_ToLastBodyDistance[i] <= -1.0f)
		{
			m_ToLastBodyDistance[i] = 2 * m_BodyInfo.BaseInfo.GeosphereInfo.Radius;
			if (i != 0)
			{
				m_BodyCoordSystem[i] = m_BodyCoordSystem[i - 1];

			}
			else
			{
				m_BodyCoordSystem[i] = m_HeadCoordSystem;
			}
		}*/
	}

	

}
//
//void GameManager::Snake::UpdateBody()
//{
//	std::vector<XMFLOAT3> prevPos = m_BodyPos;
//
//	for (size_t i = 0; i < m_BodyLength + 1; ++i)
//	{
//		XMVECTOR l = XMLoadFloat3(&m_BodyCoordSystem[i].Look);
//		XMVECTOR p = XMLoadFloat3(&m_BodyPos[i]);
//		//XMStoreFloat3(&m_BodyPos[i], XMVectorAdd(m_SL[i], p));
//		XMStoreFloat3(&m_BodyPos[i], XMVectorMultiplyAdd(m_SL[i],l, p));
//	}
//
//	if (m_TurnCount.size() == 0)
//	{
//		for (size_t i = 0; i < m_BodyLength+1; ++i)
//		{
//			XMMATRIX prevOffset = XMMatrixTranslation(prevPos[i].x, prevPos[i].y, prevPos[i].z);
//			XMMATRIX currOffset = XMMatrixTranslation(m_BodyPos[i].x, m_BodyPos[i].y, m_BodyPos[i].z);
//			XMMATRIX worldTemp = XMLoadFloat4x4(&m_BodyWorld[i]);
//			worldTemp *= (XMMatrixInverse(&XMMatrixDeterminant(prevOffset), prevOffset) * currOffset);
//			XMStoreFloat4x4(&m_BodyWorld[i], worldTemp);
//
//			m_BodyCoordSystem[i].Position = m_BodyPos[i];
//		}
//	}
//	else
//	{
//		size_t s = m_TurnCount.size();
//		for (size_t i = 0; i < m_BodyLength + 1; i++)
//		{
//			size_t index = 0;
//			
//			MathHelper::findNumberInVector(m_TurnCount, i, s, index);
//			if (index != -1)
//			{
//				if (MathHelper::isAround(m_BodyPos[i], m_HeadTurnPos[index], 0.01f))
//				{
//					m_BodyPos[i] = m_HeadTurnPos[index];
//
//					m_BodyWorld[i] = m_HeadWorldTemp[index];
//					m_BodyCoordSystem[i] = m_CoordSystemTemp[index];
//
//					m_TurnCount[index]++;
//					continue;
//				}
//				
//			}
//
//			XMMATRIX prevOffset = XMMatrixTranslation(prevPos[i].x, prevPos[i].y, prevPos[i].z);
//			XMMATRIX currOffset = XMMatrixTranslation(m_BodyPos[i].x, m_BodyPos[i].y, m_BodyPos[i].z);
//			XMMATRIX worldTemp = XMLoadFloat4x4(&m_BodyWorld[i]);
//			worldTemp *= (XMMatrixInverse(&XMMatrixDeterminant(prevOffset), prevOffset) * currOffset);
//			XMStoreFloat4x4(&m_BodyWorld[i], worldTemp);
//
//			m_BodyCoordSystem[i].Position = m_BodyPos[i];
//		}
//
//	}
//
//}
