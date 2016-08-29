#include "ICamera.h"
using namespace GameManager;

ICamera::ICamera()
:	m_Position(0.0f, 0.0f, 0.0f),
	m_Right(1.0f, 0.0f, 0.0f),
	m_Up(0.0f, 1.0f, 0.0f),
	m_Look(0.0f, 0.0f, 1.0f),
	m_X(1.0f, 0.0f, 0.0f),
	m_Y(0.0f, 1.0f, 0.0f),
	m_Z(0.0f, 0.0f, 1.0f),
	m_NX(-1.0f, 0.0f, 0.0f),
	m_NY(0.0f, -1.0f, 0.0f),
	m_NZ(0.0f, 0.0f, -1.0f)
{
	SetLens(0.25f*MathHelper::Pi, 1.0f, 1.0f, 1000.0f);
}

XMVECTOR ICamera::GetPositionXM()const
{
	return XMLoadFloat3(&m_Position);
}

XMFLOAT3 ICamera::GetPosition()const
{
	return m_Position;
}

void ICamera::SetPosition(float x, float y, float z)
{
	m_Position = XMFLOAT3(x, y, z);
}

void ICamera::SetPosition(const XMFLOAT3& v)
{
	m_Position = v;
}

XMVECTOR ICamera::GetRightXM()const
{
	return XMLoadFloat3(&m_Right);
}

XMFLOAT3 ICamera::GetRight()const
{
	return m_Right;
}

XMVECTOR ICamera::GetUpXM()const
{
	return XMLoadFloat3(&m_Up);
}

XMFLOAT3 ICamera::GetUp()const
{
	return m_Up;
}

XMVECTOR ICamera::GetLookXM()const
{
	return XMLoadFloat3(&m_Look);
}

XMFLOAT3 ICamera::GetLook()const
{
	return m_Look;
}

float ICamera::GetNearZ()const
{
	return m_NearZ;
}

float ICamera::GetFarZ()const
{
	return m_FarZ;
}

float ICamera::GetAspect()const
{
	return m_Aspect;
}

float ICamera::GetFovY()const
{
	return m_FovY;
}

float ICamera::GetFovX()const
{
	float halfWidth = 0.5f*GetNearWindowWidth();
	return 2.0f*atan(halfWidth / m_NearZ);
}

float ICamera::GetNearWindowWidth()const
{
	return m_Aspect * m_NearWindowHeight;
}

float ICamera::GetNearWindowHeight()const
{
	return m_NearWindowHeight;
}

float ICamera::GetFarWindowWidth()const
{
	return m_Aspect * m_FarWindowHeight;
}

float ICamera::GetFarWindowHeight()const
{
	return m_FarWindowHeight;
}

void ICamera::SetLens(float fovY, float aspect, float zn, float zf)
{
	// cache properties
	m_FovY = fovY;
	m_Aspect = aspect;
	m_NearZ = zn;
	m_FarZ = zf;
	 
	m_NearWindowHeight = 2.0f * m_NearZ * tanf( 0.5f*m_FovY );
	m_FarWindowHeight  = 2.0f * m_FarZ * tanf( 0.5f*m_FovY );

	XMMATRIX P = XMMatrixPerspectiveFovLH(m_FovY, m_Aspect, m_NearZ, m_FarZ);
	XMStoreFloat4x4(&m_Proj, P);
}

XMFLOAT3 GameManager::ICamera::GetLook()
{
	return m_Look;
}

void GameManager::ICamera::SetLook(XMFLOAT3 & look)
{
	XMVECTOR L = XMVector3Normalize(XMLoadFloat3(&look));
	XMStoreFloat3(&m_Look, L);
}

void GameManager::ICamera::SetUp(XMFLOAT3 & up)
{
	XMVECTOR U = XMVector3Normalize(XMLoadFloat3(&up));
	XMStoreFloat3(&m_Up, U);
}

void GameManager::ICamera::SetRight(XMFLOAT3 & right)
{
	XMVECTOR R = XMVector3Normalize(XMLoadFloat3(&right));
	XMStoreFloat3(&m_Right, R);
}

void ICamera::LookAt(FXMVECTOR pos, FXMVECTOR target, FXMVECTOR worldRight)
{
	XMVECTOR L = XMVector3Normalize(XMVectorSubtract(target, pos));
	XMVECTOR U = XMVector3Normalize(XMVector3Cross(L, worldRight));
	XMVECTOR R = XMVector3Cross(U, L);

	XMStoreFloat3(&m_Position, pos);
	XMStoreFloat3(&m_Look, L);
	XMStoreFloat3(&m_Right, R);
	XMStoreFloat3(&m_Up, U);
}

void ICamera::LookAt(const XMFLOAT3& pos, const XMFLOAT3& target, const XMFLOAT3& right)
{
	XMVECTOR P = XMLoadFloat3(&pos);
	XMVECTOR T = XMLoadFloat3(&target);
	XMVECTOR R = XMLoadFloat3(&right);

	LookAt(P, T, R);
}

XMMATRIX ICamera::View()const
{
	return XMLoadFloat4x4(&m_View);
}

XMMATRIX ICamera::Proj()const
{
	return XMLoadFloat4x4(&m_Proj);
}

XMMATRIX ICamera::ViewProj()const
{
	return XMMatrixMultiply(View(), Proj());
}

void ICamera::Strafe(float d)
{
	// m_Position += d*mRight
	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR r = XMLoadFloat3(&m_Right);
	XMVECTOR p = XMLoadFloat3(&m_Position);
	XMStoreFloat3(&m_Position, XMVectorMultiplyAdd(s, r, p));
}

void ICamera::Walk(float d)
{
	// m_Position += d*mLook
	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR l = XMLoadFloat3(&m_Look);
	XMVECTOR p = XMLoadFloat3(&m_Position);
	XMStoreFloat3(&m_Position, XMVectorMultiplyAdd(s, l, p));
}

void ICamera::Pitch(float angle)
{
	// Rotate up and look vector about the right vector.

	XMMATRIX R = XMMatrixRotationAxis(XMLoadFloat3(&m_Right), angle);

	XMStoreFloat3(&m_Up,   XMVector3TransformNormal(XMLoadFloat3(&m_Up), R));
	XMStoreFloat3(&m_Look, XMVector3TransformNormal(XMLoadFloat3(&m_Look), R));
}

void ICamera::RotateY(float angle)
{
	// Rotate the basis vectors about the world y-axis.
	XMMATRIX R = XMMatrixRotationY(angle);
	if (XMVector3Less(XMVector3Dot(XMLoadFloat3(&m_Up), XMLoadFloat3(&m_Y)),
					  MathHelper::Zero))
	{
		R = XMMatrixRotationAxis(XMLoadFloat3(&m_NY), angle);
	}
	//XMMATRIX R = XMMatrixRotationAxis(XMLoadFloat3(&m_Up), angle);

	XMStoreFloat3(&m_Right, XMVector3TransformNormal(XMLoadFloat3(&m_Right), R));
	XMStoreFloat3(&m_Up,	XMVector3TransformNormal(XMLoadFloat3(&m_Up), R));
	XMStoreFloat3(&m_Look,  XMVector3TransformNormal(XMLoadFloat3(&m_Look), R));
}

void GameManager::ICamera::RotateAxis(XMFLOAT3 &axis, XMFLOAT3 &axisPos, float angle)
{
	XMMATRIX R = XMMatrixRotationAxis(XMLoadFloat3(&axis), angle);
	XMFLOAT3 pTemp = {
		m_Position.x - axisPos.x,
		m_Position.y - axisPos.y,
		m_Position.z - axisPos.z
	};
	XMStoreFloat3(&pTemp, XMVector3TransformCoord(XMLoadFloat3(&pTemp), R));
	m_Position = {
		pTemp.x + axisPos.x,
		pTemp.y + axisPos.y,
		pTemp.z + axisPos.z,
	};
	//XMStoreFloat3(&m_Position, XMVector3TransformCoord(XMLoadFloat3(&m_Position), R));
	XMStoreFloat3(&m_Up, XMVector3TransformNormal(XMLoadFloat3(&m_Up), R));
	XMStoreFloat3(&m_Right, XMVector3TransformNormal(XMLoadFloat3(&m_Right), R));
	XMStoreFloat3(&m_Look, XMVector3TransformNormal(XMLoadFloat3(&m_Look), R));
}

void ICamera::RotateUp(float angle)
{
	XMMATRIX R = XMMatrixRotationAxis(XMLoadFloat3(&m_Up), angle);

	XMStoreFloat3(&m_Right, XMVector3TransformNormal(XMLoadFloat3(&m_Right), R));
	XMStoreFloat3(&m_Look, XMVector3TransformNormal(XMLoadFloat3(&m_Look), R));
}

void ICamera::RotateRight(float angle)
{
	XMMATRIX R = XMMatrixRotationAxis(XMLoadFloat3(&m_Right), angle);

	XMStoreFloat3(&m_Up, XMVector3TransformNormal(XMLoadFloat3(&m_Up), R));
	XMStoreFloat3(&m_Look, XMVector3TransformNormal(XMLoadFloat3(&m_Look), R));
}

void ICamera::RotateLook(float angle)
{
	XMMATRIX R = XMMatrixRotationAxis(XMLoadFloat3(&m_Look), angle);

	XMStoreFloat3(&m_Up, XMVector3TransformNormal(XMLoadFloat3(&m_Up), R));
	XMStoreFloat3(&m_Right, XMVector3TransformNormal(XMLoadFloat3(&m_Right), R));
}

void ICamera::UpdateViewMatrix()
{
	XMVECTOR R = XMLoadFloat3(&m_Right);
	XMVECTOR U = XMLoadFloat3(&m_Up);
	XMVECTOR L = XMLoadFloat3(&m_Look);
	XMVECTOR P = XMLoadFloat3(&m_Position);

	// Keep camera's axes orthogonal to each other and of unit length.
	L = XMVector3Normalize(L);
	U = XMVector3Normalize(XMVector3Cross(L, R));

	// U, L already ortho-normal, so no need to normalize cross product.
	R = XMVector3Cross(U, L); 

	// Fill in the view matrix entries.
	float x = -XMVectorGetX(XMVector3Dot(P, R));
	float y = -XMVectorGetX(XMVector3Dot(P, U));
	float z = -XMVectorGetX(XMVector3Dot(P, L));

	XMStoreFloat3(&m_Right, R);
	XMStoreFloat3(&m_Up, U);
	XMStoreFloat3(&m_Look, L);

	m_View(0,0) = m_Right.x; 
	m_View(1,0) = m_Right.y; 
	m_View(2,0) = m_Right.z; 
	m_View(3,0) = x;   
	 
	m_View(0,1) = m_Up.x;
	m_View(1,1) = m_Up.y;
	m_View(2,1) = m_Up.z;
	m_View(3,1) = y;  
	 
	m_View(0,2) = m_Look.x; 
	m_View(1,2) = m_Look.y; 
	m_View(2,2) = m_Look.z; 
	m_View(3,2) = z;   
	 
	m_View(0,3) = 0.0f;
	m_View(1,3) = 0.0f;
	m_View(2,3) = 0.0f;
	m_View(3,3) = 1.0f;
}


