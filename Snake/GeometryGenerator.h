#pragma once

#include "d3dUtility.h"

class GeometryGenerator
{
public:
	struct Vertex
	{
		XMFLOAT3 Position;
		XMFLOAT3 Normal;
		XMFLOAT3 TangentU;
		XMFLOAT2 TexC;

		Vertex(){}
		Vertex(const XMFLOAT3 &p, const XMFLOAT3 &n, const XMFLOAT3 &t, const XMFLOAT2 &uv)
			: Position(p), Normal(n), TangentU(t), TexC(uv){}
		Vertex(
			float p_x, float p_y, float p_z,
			float n_x, float n_y, float n_z,
			float t_x, float t_y, float t_z,
			float u, float v)
			: Position(p_x, p_y, p_z), Normal(n_x, n_y, n_z),
			  TangentU(t_x, t_y, t_z), TexC(u, v){}
	};

	struct MeshData
	{
		std::vector<Vertex> Vertices;
		std::vector<UINT> Indices;
	};

	void CreateBox(float width, float height, float depth, MeshData &meshData);
	void CreateSphere(float radius, UINT sliceCount, UINT stackCount, MeshData &meshData);
	void CreateGeosphere(float radius, UINT numSubdivisions, MeshData &meshData);
	void CreateCylinder(float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount, MeshData &meshData);
	void CreateGrid(float width, float depth, UINT rowCount, UINT columnCount, MeshData &meshData);
	void CreateFullscreenQuad(MeshData &meshData);

private:
	void Subdivide(MeshData &meshData);
	void BuildCylinderTopCap(float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount, MeshData &meshData);
	void BuildCylinderBottomCap(float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount, MeshData &meshData);
};
