#pragma once

#include "D3DUtility.h"

namespace GameManager
{
	class ICamera
	{
	public:
		ICamera();

		// Get/Set world camera position.
		XMVECTOR GetPositionXM()const;
		XMFLOAT3 GetPosition()const;
		void SetPosition(float x, float y, float z);
		void SetPosition(const XMFLOAT3& v);

		// Get camera basis vectors.
		XMVECTOR GetRightXM()const;
		XMFLOAT3 GetRight()const;
		XMVECTOR GetUpXM()const;
		XMFLOAT3 GetUp()const;
		XMVECTOR GetLookXM()const;
		XMFLOAT3 GetLook()const;

		// Get frustum properties.
		float GetNearZ()const;
		float GetFarZ()const;
		float GetAspect()const;
		float GetFovY()const;
		float GetFovX()const;

		// Get near and far plane dimensions in view space coordinates.
		float GetNearWindowWidth()const;
		float GetNearWindowHeight()const;
		float GetFarWindowWidth()const;
		float GetFarWindowHeight()const;

		// Set frustum.
		void SetLens(float fovY, float aspect, float zn, float zf);

		XMFLOAT3 GetLook();
		void SetLook(XMFLOAT3 &look);
		void SetUp(XMFLOAT3 &up);
		void SetRight(XMFLOAT3 &right);

		// Define camera space via LookAt parameters.
		void LookAt(FXMVECTOR pos, FXMVECTOR target, FXMVECTOR worldRight);
		void LookAt(const XMFLOAT3& pos, const XMFLOAT3& target, const XMFLOAT3& right);

		// Get View/Proj matrices.
		XMMATRIX View()const;
		XMMATRIX Proj()const;
		XMMATRIX ViewProj()const;

		// Strafe/Walk the camera a distance d.
		void Strafe(float d);
		void Walk(float d);

		// Rotate the camera.
		void Pitch(float angle);
		void RotateY(float angle);
		void RotateAxis(XMFLOAT3 &axis, XMFLOAT3 &axisPos, float angle);
		void RotateUp(float angle);
		void RotateRight(float angle);
		void RotateLook(float angle);

		// After modifying camera position/orientation, call to rebuild the view matrix.
		void UpdateViewMatrix();

	private:
		// ICamera coordinate system with coordinates relative to world space.
		XMFLOAT3 m_Position;
		XMFLOAT3 m_Right;
		XMFLOAT3 m_Up;
		XMFLOAT3 m_Look;

		// Cache frustum properties.
		float m_NearZ;
		float m_FarZ;
		float m_Aspect;
		float m_FovY;
		float m_NearWindowHeight;
		float m_FarWindowHeight;

		// Basic coordinate axis.
		XMFLOAT3 m_X;
		XMFLOAT3 m_Y;
		XMFLOAT3 m_Z;
		XMFLOAT3 m_NX;
		XMFLOAT3 m_NY;
		XMFLOAT3 m_NZ;

		// Cache View/Proj matrices.
		XMFLOAT4X4 m_View;
		XMFLOAT4X4 m_Proj;
	};
}