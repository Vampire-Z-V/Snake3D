#include "MathHelper.h"
#include <float.h>
#include <cmath>

const float MathHelper::Infinity = FLT_MAX;
const float MathHelper::Pi = 3.1415926535f;
const XMVECTOR MathHelper::One = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
const XMVECTOR MathHelper::Zero = XMVectorZero();
const XMMATRIX MathHelper::Identity = XMMatrixIdentity();

float MathHelper::AngleFromXY(float x, float y)
{
	float theta = 0.0f;

	if (x >= 0)
	{
		theta = atanf(y / x);
		if (theta < 0)
			theta += 2 * Pi;
	}
	else
	{
		theta = atanf(y / x) + Pi;
	}

	return theta;
}

XMMATRIX MathHelper::InverseTranspose(CXMMATRIX M)
{
	XMMATRIX A = M;
	A.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

	XMVECTOR det = XMMatrixDeterminant(A);
	return XMMatrixTranspose(XMMatrixInverse(&det, A));
}

XMVECTOR MathHelper::RandUnitVector3()
{
	while (true)
	{
		XMVECTOR v = XMVectorSet(
			RandF(-1.0f, -1.0f),
			RandF(-1.0f, -1.0f),
			RandF(-1.0f, -1.0f),
			0.0f
		);
		if (XMVector3Greater(XMVector3LengthSq(v), One))
			continue;
		return XMVector3Normalize(v);
	}
}

XMVECTOR MathHelper::RandHemisphereUnitVector3(XMVECTOR n)
{
	while (true)
	{
		XMVECTOR v = XMVectorSet(
			RandF(-1.0f, -1.0f),
			RandF(-1.0f, -1.0f),
			RandF(-1.0f, -1.0f),
			0.0f
		);
		if (XMVector3Greater(XMVector3LengthSq(v), One))
			continue;

		// Ignore points in the bottom hemisphere.
		if (XMVector3Less(XMVector3Dot(n, v), Zero))
			continue;

		return XMVector3Normalize(v);
	}
}

bool MathHelper::isCollided(BOUNDING::SPHERE obj, BOUNDING::BOX target)
{
	obj.CenterPos.x = abs(obj.CenterPos.x);
	obj.CenterPos.y = abs(obj.CenterPos.y);
	obj.CenterPos.z = abs(obj.CenterPos.z);

	XMFLOAT3 h = {
		target.MaxPos.x - target.CenterPos.x,
		target.MaxPos.y - target.CenterPos.y,
		target.MaxPos.z - target.CenterPos.z
	};
	XMFLOAT3 v = {
		obj.CenterPos.x - target.CenterPos.x,
		obj.CenterPos.y - target.CenterPos.y,
		obj.CenterPos.z - target.CenterPos.z
	};
	XMFLOAT3 u = {
		v.x - h.x,
		v.y - h.y,
		v.z - h.z
	};

	u = {
		Max(u.x, 0.0f),
		Max(u.y, 0.0f),
		Max(u.z, 0.0f)
	};

	if (XMVector3Less(XMVector3LengthSq(XMLoadFloat3(&u)), 
					  XMVector3LengthSq(XMLoadFloat(&obj.Radius))))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool MathHelper::isCollided(BOUNDING::SPHERE obj, BOUNDING::SPHERE target)
{
	XMFLOAT3 v = {
		obj.CenterPos.x - target.CenterPos.x,
		obj.CenterPos.y - target.CenterPos.y,
		obj.CenterPos.z - target.CenterPos.z
	};

	float r = obj.Radius + target.Radius;

	if (XMVector3Less(XMVector3LengthSq(XMLoadFloat3(&v)),
					  XMVector3LengthSq(XMLoadFloat(&r))))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool MathHelper::isAround(XMFLOAT3 & curr, XMFLOAT3 & target, float error)
{
	XMVECTOR e = XMVectorReplicate(error);
	XMFLOAT3 dl = {
		curr.x - target.x,
		curr.y - target.y,
		curr.z - target.z
	};
	XMVECTOR length = XMVector3Length(XMLoadFloat3(&dl));

	if (XMVector3Less(length, e))
	{
		return true;
	}
	else
	{
		return false;
	}
}
