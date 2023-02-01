#include "pch.h"
#include "Face.h"
#include "Material.h"
#include "Texture.h"

namespace dae
{
	Face::Face(ID3D11Device* pDevice, const std::string& texturePath)
	{
		// Create Material
		m_pMaterial = new Material{ pDevice, L"Resources/PosTex3D.fx" };

		// Load vertices and indices from a file
		std::vector<Vertex> vertices
		{
			{ { -0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f } },
			{ { -0.5f, 0.5f, 0.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 1.0f } },
			{ { 0.5f, 0.5f, 0.0f }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 1.0f } },
			{ { 0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 0.0f } }
		};
		std::vector<uint32_t> indices{ 1, 2, 0, 3 };

		// Create Input Layout
		m_pInputLayout = m_pMaterial->LoadInputLayout(pDevice);

		// Create vertex buffer
		D3D11_BUFFER_DESC bd{};
		bd.Usage = D3D11_USAGE_IMMUTABLE;
		bd.ByteWidth = sizeof(Vertex) * static_cast<uint32_t>(vertices.size());
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA initData{};
		initData.pSysMem = vertices.data();

		HRESULT result{ pDevice->CreateBuffer(&bd, &initData, &m_pVertexBuffer) };
		if (FAILED(result)) return;

		// Create index buffer
		m_NumIndices = static_cast<uint32_t>(indices.size());
		bd.Usage = D3D11_USAGE_IMMUTABLE;
		bd.ByteWidth = sizeof(uint32_t) * m_NumIndices;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		initData.pSysMem = indices.data();

		result = pDevice->CreateBuffer(&bd, &initData, &m_pIndexBuffer);
		if (FAILED(result)) return;

		// Create texture
		Texture* pTexture{ Texture::LoadFromFile(pDevice, texturePath) };
		m_pMaterial->SetTexture(pTexture);
		delete pTexture;
	}

	Face::~Face()
	{
		if (m_pIndexBuffer) m_pIndexBuffer->Release();
		if (m_pVertexBuffer) m_pVertexBuffer->Release();

		if (m_pInputLayout) m_pInputLayout->Release();

		delete m_pMaterial;
	}

	void Face::Render(ID3D11DeviceContext* pDeviceContext, const Matrix& worldMatrix, const Matrix& worldViewProjection) const
	{
		m_pMaterial->SetMatrices(worldMatrix, worldViewProjection);

		// Set primitive topology
		pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		// Set input layout
		pDeviceContext->IASetInputLayout(m_pInputLayout);

		// Set vertex buffer
		constexpr UINT stride{ sizeof(Vertex) };
		constexpr UINT offset{ 0 };
		pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

		// Set index buffer
		pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		// Draw
		D3DX11_TECHNIQUE_DESC techniqueDesc{};
		m_pMaterial->GetTechnique()->GetDesc(&techniqueDesc);
		for (UINT p{}; p < techniqueDesc.Passes; ++p)
		{
			m_pMaterial->GetTechnique()->GetPassByIndex(p)->Apply(0, pDeviceContext);
			pDeviceContext->DrawIndexed(m_NumIndices, 0, 0);
		}
	}
}
