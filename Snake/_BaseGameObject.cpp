#include "_BaseGameObject.h"
using namespace GameManager;


_BaseGameObject::_BaseGameObject(ID3D11Device *device)
:	m_pD3DDevice(device)
{
}


_BaseGameObject::~_BaseGameObject()
{
	for (auto &i : m_TextureSRV)
	{
		ReleaseCOM(i.second);
	}
}

void _BaseGameObject::Create_VB_and_IB(ID3D11Buffer **VB, ID3D11Buffer **IB, GeometryInfo &info, BUFFER_TYPE type)
{
	GeometryGenerator::MeshData geometryTemp;
	GeometryGenerator geoGen;

	switch (type)
	{
		case GRID:
			geoGen.CreateGrid(100.0f, 100.0f, 100, 100, geometryTemp);
			info.BaseInfo.GridInfo.Width = 100.0f;
			info.BaseInfo.GridInfo.Height = 100.0f;
			info.Style = GRID;
			break;
		case BOX:
			geoGen.CreateBox(1.0f, 1.0f, 1.0f, geometryTemp);
			info.BaseInfo.BoxInfo.Length = 1.0f;
			info.BaseInfo.BoxInfo.Width = 1.0f;
			info.BaseInfo.BoxInfo.Height = 1.0f;
			info.Style = BOX;
			break;
		case GEOSPHERE:
			//geoGen.CreateGeosphere(0.5f, 3, geometryTemp);
			geoGen.CreateSphere(0.5f, 30, 30, geometryTemp);
			info.BaseInfo.GeosphereInfo.Radius = 0.5f;
			info.Style = GEOSPHERE;
			break;
		case CYLINDER:
			break;
	}

	info.VertexOffset = m_Vertics.size();
	info.IndexOffset = m_Indices.size();
	info.IndexCount = geometryTemp.Indices.size();

	size_t verticsCount = geometryTemp.Vertices.size();
	m_Vertics.resize((size_t)(info.VertexOffset + verticsCount));

	for (size_t i = 0; i < verticsCount; i++)
	{
		m_Vertics[info.VertexOffset + i].Pos = geometryTemp.Vertices[i].Position;
		m_Vertics[info.VertexOffset + i].Normal = geometryTemp.Vertices[i].Normal;
		m_Vertics[info.VertexOffset + i].Tex = geometryTemp.Vertices[i].TexC;
	}

	m_Indices.insert(m_Indices.end(), geometryTemp.Indices.begin(), geometryTemp.Indices.end());

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex::BasicVertex) * (info.VertexOffset + verticsCount);
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &m_Vertics[0];
	HR(m_pD3DDevice->CreateBuffer(&vbd, &vinitData, VB));

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * (info.IndexOffset + info.IndexCount);
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &m_Indices[0];
	HR(m_pD3DDevice->CreateBuffer(&ibd, &iinitData, IB));
}

ID3D11ShaderResourceView* _BaseGameObject::CreateSRVFromFile(std::wstring filename)
{
	ID3D11ShaderResourceView *srv = nullptr;

	auto iter = m_TextureSRV.find(filename);
	if (iter != m_TextureSRV.end())
	{
		srv = iter->second;
	}
	else
	{
		ID3D11Resource* texResource = nullptr;
		HR(DirectX::CreateDDSTextureFromFile(m_pD3DDevice, filename.c_str(), &texResource, &srv));
		ReleaseCOM(texResource); // view saves reference

		m_TextureSRV.insert(std::map<std::wstring, ID3D11ShaderResourceView*>::value_type (filename, srv));
	}

	return srv;
}

MathHelper::BOUNDING _BaseGameObject::CreateImpactBox(GeometryInfo &Info)
{
	MathHelper::BOUNDING temp;

	switch (Info.Style)
	{
		case GRID:
			temp.isBoundingBox = true;
			temp.Box.CenterPos = { 0.0f,0.0f,0.0f };
			temp.Box.MaxPos = { 
				Info.BaseInfo.GridInfo.Width / 2,
				0.0f,
				Info.BaseInfo.GridInfo.Height / 2 
			};
			return temp;
		case BOX:
			temp.isBoundingBox = true;
			temp.Box.CenterPos = { 0.0f,0.0f,0.0f };
			temp.Box.MaxPos = { 
				Info.BaseInfo.BoxInfo.Length / 2,
				Info.BaseInfo.BoxInfo.Height / 2,
				Info.BaseInfo.BoxInfo.Width / 2 
			};
			return temp;
		case GEOSPHERE:
			temp.isBoundingBox = false;
			temp.Sphere.CenterPos = { 0.0f,0.0f,0.0f };
			temp.Sphere.Radius = Info.BaseInfo.GeosphereInfo.Radius;
			return temp;
		case CYLINDER:
		default:
			return temp;
	}
}
