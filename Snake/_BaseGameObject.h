#pragma once
#include "GeometryGenerator.h"
#include "Vertex.h"
#include "LightHelper.h"
#include <vector>
#include <map>
#include "MathHelper.h"
namespace GameManager
{
	class _BaseGameObject
	{
	public:
		enum BUFFER_TYPE
		{
			GRID,
			BOX,
			GEOSPHERE,
			CYLINDER,
		};

		struct GeometryInfo
		{
			union
			{
				struct
				{
					float Width;
					float Height;
				} GridInfo;
				struct
				{
					float Length;
					float Width;
					float Height;
				} BoxInfo;
				struct
				{
					float Radius;
				} GeosphereInfo;
				struct
				{
					float TopRadius;
					float BottomRadius;
				} CylinderInfo;
			} BaseInfo;

			BUFFER_TYPE Style;
			int  VertexOffset;
			UINT IndexOffset;
			UINT IndexCount;
			Material Material;
		};

	public:
		_BaseGameObject(ID3D11Device *device);
		~_BaseGameObject();

		virtual void Update() = 0;
		virtual void Render() = 0;
		//virtual const std::vector<XMFLOAT4X4>* GetWorld(BUFFER_TYPE type) = 0;

	protected:
		void Create_VB_and_IB(ID3D11Buffer **VB, ID3D11Buffer **IB, GeometryInfo &info, BUFFER_TYPE type);
		ID3D11ShaderResourceView* CreateSRVFromFile(std::wstring filename);
		MathHelper::BOUNDING CreateImpactBox(GeometryInfo &Info);

	protected:
		std::vector<MathHelper::BOUNDING> m_ImpactBoxs;

	private:
		ID3D11Device* m_pD3DDevice;

		std::vector<Vertex::BasicVertex> m_Vertics;
		std::vector<UINT> m_Indices;

		std::map<std::wstring, ID3D11ShaderResourceView*> m_TextureSRV;
	};
}
