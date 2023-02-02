#pragma once
#include "Material.h"

namespace dae
{
	class Face final
	{
	public:
		enum class FaceDirection : BYTE
		{
			FORWARD,
			RIGHT,
			LEFT,
			BACK,
			UP,
			BOTTOM
		};

		struct Vertex final
		{
			Vector3 position;
			Vector3 normal;
			Vector2 uv;
		};

		Face(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
		~Face();

		void SetTexture(Texture* pTexture);
		void SetMatrix(const Matrix& worldViewProjection);

		void Render(ID3D11DeviceContext* pDeviceContext, FaceDirection direction) const;
	private:
		Material* m_pMaterial{};

		ID3D11InputLayout* m_pInputLayout{};

		ID3D11Buffer* m_pVertexBuffer{};

		uint32_t m_NumIndices{};
		ID3D11Buffer* m_pIndexBuffer{};
	};
}

