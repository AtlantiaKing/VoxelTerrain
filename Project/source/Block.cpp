#include "pch.h"
#include "Block.h"
#include "Face.h"

bool dae::Block::m_Initialized{};
dae::Vector3Int dae::Block::m_NeighbouringBlocks[6]{};
dae::Matrix dae::Block::m_TranslationMatrix{ Vector3::UnitX, Vector3::UnitY, Vector3::UnitZ, Vector3::Zero };

dae::Block::Block(Texture* pSideTexture, Texture* pTopTexture, Texture* pBottomTexture)
	: m_pSideTexture{ pSideTexture }
	, m_pTopTexture{ pTopTexture }
	, m_pBottomTexture{ pBottomTexture }
{
	if (m_Initialized) return;

	for (int i{}; i <= static_cast<int>(Face::FaceDirection::BOTTOM); ++i)
	{
		switch (static_cast<Face::FaceDirection>(i))
		{
		case Face::FaceDirection::FORWARD:
		{
			m_NeighbouringBlocks[i] = -Vector3Int::UnitZ;
			break;
		}
		case Face::FaceDirection::BACK:
		{
			m_NeighbouringBlocks[i] = Vector3Int::UnitZ;
			break;
		}
		case Face::FaceDirection::RIGHT:
		{
			m_NeighbouringBlocks[i] = -Vector3Int::UnitX;
			break;
		}
		case Face::FaceDirection::LEFT:
		{
			m_NeighbouringBlocks[i] = Vector3Int::UnitX;
			break;
		}
		case Face::FaceDirection::UP:
		{
			m_NeighbouringBlocks[i] = Vector3Int::UnitY;
			break;
		}
		case Face::FaceDirection::BOTTOM:
		{
			m_NeighbouringBlocks[i] = -Vector3Int::UnitY;
			break;
		}
		}
	}

	m_Initialized = true;
}

void dae::Block::Render(ID3D11DeviceContext* pDeviceContext, const std::function<bool(const Vector3Int&)>& isBlockPredicate, const Vector3Int& position, const Matrix& viewProjection, Face* pFace) const
{
	m_TranslationMatrix[3][0] = static_cast<float>(position.x);
	m_TranslationMatrix[3][1] = static_cast<float>(position.y);
	m_TranslationMatrix[3][2] = static_cast<float>(position.z);

	const Matrix worldViewProjection{ m_TranslationMatrix * viewProjection };

	Vector4 projectedPosition{ worldViewProjection.TransformPoint(Vector4{ 0.0f, 0.0f, 0.0f, 1.0f }) };
	projectedPosition.x /= projectedPosition.w;
	projectedPosition.y /= projectedPosition.w;
	projectedPosition.z /= projectedPosition.w;

	constexpr float projectionXMargin{ 2.0f };
	constexpr float projectionYMargin{ 2.5f };
	if (projectedPosition.x < -projectionXMargin || projectedPosition.x > projectionXMargin ||
		projectedPosition.y < -projectionYMargin || projectedPosition.y > projectionYMargin ||
		projectedPosition.z < 0.0f || projectedPosition.z > 1.0f) return;

	pFace->SetTexture(m_pSideTexture);
	pFace->SetMatrix(worldViewProjection);

	for (int i{}; i <= static_cast<int>(Face::FaceDirection::BOTTOM); ++i)
	{
		if (isBlockPredicate(position + m_NeighbouringBlocks[i])) continue;

		switch (static_cast<Face::FaceDirection>(i))
		{
		case Face::FaceDirection::UP:
			if (m_pTopTexture) pFace->SetTexture(m_pTopTexture);
			break;
		case Face::FaceDirection::BOTTOM:
			if (m_pBottomTexture) pFace->SetTexture(m_pBottomTexture);
			break;
		}

		pFace->Render(pDeviceContext, static_cast<Face::FaceDirection>(i));
	}
}
