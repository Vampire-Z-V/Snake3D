#pragma once
#include "_BaseGameObject.h"

namespace GameManager
{
	class Snake :
		public _BaseGameObject
	{
	public:
		enum SNAKE_TYPE
		{
			HEAD,
			BODY,
		};

	public:
		Snake(ID3D11Device *device);
		~Snake();

		void Init();
		void Update();
		void Render();
		MathHelper::BOUNDING GetImpactBox(SNAKE_TYPE type, int = 0);
		XMFLOAT3 GetHeadUp();
		XMFLOAT3 GetHeadRight();
		XMFLOAT3 GetHeadLook();
		XMFLOAT3 GetHeadPos();

		void Walk(float d);
		void RotateUp(float angle);
		void RotateRight(float angle);
		void RotateLook(float angle);
		void IncreaseBodyLength();

	private:
		void ResetData();
		void UpdateHeadWorld(XMMATRIX &rotateAboutUp, XMMATRIX &rotateAboutRight, XMMATRIX &rotateAboutLook,
							 XMFLOAT3 &headPrevPos, XMFLOAT3 &headCurrPos,
							 XMMATRIX &headPrevWorld);
		void CreateNewBody(XMFLOAT3 lastBodyPos, float distance, XMFLOAT4X4 lastBodyWorld, MathHelper::COORDSYSTEM lastSystem);
		void UpdateBody();

	private:
		ID3D11Buffer* m_pSnakeVB;
		ID3D11Buffer* m_pSnakeIB;

		XMFLOAT4X4 m_HeadWorld;
		XMFLOAT4X4 m_HeadRotate_Up;
		XMFLOAT4X4 m_HeadRotate_Right;
		XMFLOAT4X4 m_HeadRotate_Look;
		MathHelper::COORDSYSTEM m_HeadCoordSystem;
		XMFLOAT3 m_HeadPrevPos;
		GeometryInfo m_HeadInfo;
		MathHelper::BOUNDING m_HeadBounding;

		std::vector<XMFLOAT3> m_HeadTurnPos;
		std::vector<MathHelper::COORDSYSTEM> m_CoordSystemTemp;
		std::vector<XMFLOAT4X4> m_HeadWorldTemp;

		//std::vector<XMMATRIX> m_URL;
		std::vector<UINT> m_TurnCount;

		std::vector<XMFLOAT4X4> m_BodyWorld;
		std::vector<XMFLOAT3> m_BodyPos;
		std::vector<float> m_ToLastBodyDistance;

		std::vector<MathHelper::COORDSYSTEM> m_BodyCoordSystem;
		std::vector<XMVECTOR> m_SL;
		float m_Speed;

		GeometryInfo m_BodyInfo;
		MathHelper::BOUNDING m_BodyBounding;
		UINT m_BodyLength;

		std::vector<ID3D11ShaderResourceView*> m_SRV;
	};
}
