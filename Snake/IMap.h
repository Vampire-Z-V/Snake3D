#pragma once
#include <DirectXMath.h>
#include <vector>

namespace GameManager
{
	class IMap
	{
	public:
		IMap();
		~IMap();

		const std::vector<DirectX::XMFLOAT3>* GetFromMap();

	private:
		std::vector<DirectX::XMFLOAT3> m_BoxWallPos;
	};
}
