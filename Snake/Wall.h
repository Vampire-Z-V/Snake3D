#pragma once
#include "_BaseGameObject.h"
#include "IMap.h"

namespace GameManager
{
	class Wall :
		public _BaseGameObject
	{
	public:
		Wall(ID3D11Device *device);
		~Wall();

		void Update();
		void Render();
		MathHelper::BOUNDING GetImpactBox(BUFFER_TYPE type);
		const std::vector<XMFLOAT4X4>* GetWorld(BUFFER_TYPE type);
	private:
		ID3D11Buffer* m_pWallVB;
		ID3D11Buffer* m_pWallIB;
		
		std::vector<XMFLOAT4X4> m_GridWorld;
		GeometryInfo m_GridInfo;
		MathHelper::BOUNDING m_GridBounding;

		IMap m_BoxPos;
		std::vector<XMFLOAT4X4> m_BoxWorld;
		GeometryInfo m_BoxInfo;
		MathHelper::BOUNDING m_BoxBounding;

		std::vector<ID3D11ShaderResourceView*> m_SRV;
	};
}
