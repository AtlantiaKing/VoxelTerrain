#include "pch.h"
#include "Block.h"
#include "Face.h"

bool dae::Block::m_Initialized{};
dae::Vector3Int dae::Block::m_NeighbouringBlocks[6]{};
dae::Matrix dae::Block::m_TranslationMatrix{ Vector3::UnitX, Vector3::UnitY, Vector3::UnitZ, Vector3::Zero };
dae::Matrix dae::Block::m_FaceTransformations[6]{};

dae::Block::Block(Texture* pSideTexture, Texture* pTopTexture, Texture* pBottomTexture)
	: m_pSideTexture{ pSideTexture }
	, m_pTopTexture{ pTopTexture }
	, m_pBottomTexture{ pBottomTexture }
{
	for (int i{}; i <= static_cast<int>(Face::FaceDirection::BOTTOM); ++i)
	{
		switch (static_cast<Face::FaceDirection>(i))
		{
		case Face::FaceDirection::FORWARD:
		{
			m_FaceTransformations[i] = Matrix::CreateTranslation({ 0.0f, 0.0f, -0.5f });
			if (!m_Initialized) m_NeighbouringBlocks[i] = -Vector3Int::UnitZ;
			break;
		}
		case Face::FaceDirection::BACK:
		{
			m_FaceTransformations[i] = Matrix::CreateRotationY(180.0f * TO_RADIANS) * Matrix::CreateTranslation({ 0.0f, 0.0f, 0.5f });
			if (!m_Initialized) m_NeighbouringBlocks[i] = Vector3Int::UnitZ;
			break;
		}
		case Face::FaceDirection::RIGHT:
		{
			m_FaceTransformations[i] = Matrix::CreateRotationY(90.0f * TO_RADIANS) * Matrix::CreateTranslation({ -0.5f, 0.0f, 0.0f });
			if (!m_Initialized) m_NeighbouringBlocks[i] = -Vector3Int::UnitX;
			break;
		}
		case Face::FaceDirection::LEFT:
		{
			m_FaceTransformations[i] = Matrix::CreateRotationY(-90.0f * TO_RADIANS) * Matrix::CreateTranslation({ 0.5f, 0.0f, 0.0f });
			if (!m_Initialized) m_NeighbouringBlocks[i] = Vector3Int::UnitX;
			break;
		}
		case Face::FaceDirection::UP:
		{
			m_FaceTransformations[i] = Matrix::CreateRotationX(-90.0f * TO_RADIANS) * Matrix::CreateTranslation({ 0.0f, 0.5f, 0.0f });
			if (!m_Initialized) m_NeighbouringBlocks[i] = Vector3Int::UnitY;
			break;
		}
		case Face::FaceDirection::BOTTOM:
		{
			m_FaceTransformations[i] = Matrix::CreateRotationX(90.0f * TO_RADIANS) * Matrix::CreateTranslation({ 0.0f, -0.5f, 0.0f });
			if (!m_Initialized) m_NeighbouringBlocks[i] = -Vector3Int::UnitY;
			break;
		}
		}
	}

	m_Initialized = true;
}

void dae::Block::Render(ID3D11DeviceContext* pDeviceContext, std::function<bool(const Vector3Int&)> isBlockPredicate, const Vector3Int& position, const Matrix& viewProjection, Face* pFace) const
{
	m_TranslationMatrix[3] = Vector4{ static_cast<float>(position.x), static_cast<float>(position.y), static_cast<float>(position.z), 1.0f };

	Vector4 projectedPosition{ (m_TranslationMatrix * viewProjection).TransformPoint(Vector4{ 0.0f, 0.0f, 0.0f, 1.0f }) };
	projectedPosition.x /= projectedPosition.w;
	projectedPosition.y /= projectedPosition.w;
	projectedPosition.z /= projectedPosition.w;

	constexpr float projectionXMargin{ 2.0f };
	constexpr float projectionYMargin{ 2.5f };
	if (projectedPosition.x < -projectionXMargin || projectedPosition.x > projectionXMargin ||
		projectedPosition.y < -projectionYMargin || projectedPosition.y > projectionYMargin ||
		projectedPosition.z < 0.0f || projectedPosition.z > 1.0f) return;

	pFace->SetTexture(m_pSideTexture);

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

		const Matrix faceTransformation{ m_FaceTransformations[i] * m_TranslationMatrix };

		pFace->Render(pDeviceContext, faceTransformation, faceTransformation * viewProjection);
	}
}
