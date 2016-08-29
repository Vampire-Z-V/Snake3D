#ifndef MATHHELPER_H
#define MATHHELPER_H

#include <DirectXMath.h>
#include <Windows.h>
#include <vector>
using namespace std;
using namespace DirectX;

#define MAX(a,b) (a > b ? a : b)
#define MIN(a,b) (a < b ? a : b)
class MathHelper
{
public:
	struct BOUNDING
	{
		struct BOX
		{
			XMFLOAT3 MaxPos;
			XMFLOAT3 CenterPos;
		} Box;
		struct SPHERE
		{
			XMFLOAT3 CenterPos;
			float Radius;
		} Sphere;

		bool isBoundingBox;
	};

	struct COORDSYSTEM
	{
		XMFLOAT3 Position;
		XMFLOAT3 Right;
		XMFLOAT3 Up;
		XMFLOAT3 Look;
	};
public:
	static const float Infinity;
	static const float Pi;
	static const XMVECTOR One;
	static const XMVECTOR Zero;
	static const XMMATRIX Identity;

public:
	// Returns random float in [0, 1).
	static float RandF()
	{
		return (float)( rand() ) / (float)RAND_MAX;
	}

	// Returns random float in [a, b).
	static float RandF(float a, float b)
	{
		return a + RandF()*( b - a );
	}

	template<typename T>
	static T Min(const T &a, const T &b)
	{
		return MIN(a, b);
	}

	template<typename T>
	static T Max(const T &a, const T &b)
	{
		return MAX(a, b);
	}

	template<typename T>
	static T Lerp(const T &begin, const T &end, float t)
	{
		return begin + ( end - begin )*t;
	}

	template<typename T>
	static T Clamp(const T &x, const T &low, const T &high)
	{
		return MAX(low, MIN(x, high));
	}

	//template<typename T>
	static void findNumberInVector(vector<UINT> &set, int num, size_t &size, size_t &index)
	{
		for (int i = size-1; i >= 0; --i)
		{
			if (set[i] == num)
			{
				index = i;
				return;
			}
		}
		index = -1;
	}
public:
	// Returns the polar angle of the point (x,y) in [0, 2*PI).
	static float AngleFromXY(float x, float y);

	static XMMATRIX InverseTranspose(CXMMATRIX M);
	static XMVECTOR RandUnitVector3();
	static XMVECTOR RandHemisphereUnitVector3(XMVECTOR n);

	static bool isCollided(BOUNDING::SPHERE obj, BOUNDING::BOX target);
	static bool isCollided(BOUNDING::SPHERE obj, BOUNDING::SPHERE target);
	static bool isAround(XMFLOAT3 &curr, XMFLOAT3 &target, float error);

	
};

#endif // MATHHELPER_H

