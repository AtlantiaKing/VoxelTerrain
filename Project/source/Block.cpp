#include "pch.h"
#include "Block.h"
#include "Face.h"

bool dae::Block::m_Initialized{};
dae::Matrix dae::Block::m_FaceTransformations[6]{};
dae::Vector3Int dae::Block::m_NeighbouringBlocks[6]{};

dae::Block::Block(const Vector3Int& position)
	: m_TranslationMatrix{ Matrix::CreateTranslation(position) }
{
	if (m_Initialized) return;

	m_Initialized = true;

	for (int i{}; i <= static_cast<int>(Face::FaceDirection::BOTTOM); ++i)
	{
		switch (static_cast<Face::FaceDirection>(i))
		{
		case Face::FaceDirection::FORWARD:
		{
			m_FaceTransformations[i] = Matrix::CreateTranslation({ 0.0f, 0.0f, -0.5f });
			m_NeighbouringBlocks[i] = -Vector3Int::UnitZ;
			break;
		}
		case Face::FaceDirection::BACK:
		{
			m_FaceTransformations[i] = Matrix::CreateRotationY(180.0f * TO_RADIANS) * Matrix::CreateTranslation({ 0.0f, 0.0f, 0.5f });
			m_NeighbouringBlocks[i] = Vector3Int::UnitZ;
			break;
		}
		case Face::FaceDirection::RIGHT:
		{
			m_FaceTransformations[i] = Matrix::CreateRotationY(90.0f * TO_RADIANS) * Matrix::CreateTranslation({ -0.5f, 0.0f, 0.0f });
			m_NeighbouringBlocks[i] = -Vector3Int::UnitX;
			break;
		}
		case Face::FaceDirection::LEFT:
		{
			m_FaceTransformations[i] = Matrix::CreateRotationY(-90.0f * TO_RADIANS) * Matrix::CreateTranslation({ 0.5f, 0.0f, 0.0f });
			m_NeighbouringBlocks[i] = Vector3Int::UnitX;
			break;
		}
		case Face::FaceDirection::UP:
		{
			m_FaceTransformations[i] = Matrix::CreateRotationX(-90.0f * TO_RADIANS) * Matrix::CreateTranslation({ 0.0f, 0.5f, 0.0f });
			m_NeighbouringBlocks[i] = Vector3Int::UnitY;
			break;
		}
		case Face::FaceDirection::BOTTOM:
		{
			m_FaceTransformations[i] = Matrix::CreateRotationX(90.0f * TO_RADIANS) * Matrix::CreateTranslation({ 0.0f, -0.5f, 0.0f });
			m_NeighbouringBlocks[i] = -Vector3Int::UnitY;
			break;
		}
		}
	}
}

void dae::Block::Render(ID3D11DeviceContext* pDeviceContext, std::function<bool(const Vector3Int&)> isBlockPredicate, const Matrix& viewProjection, Face* pFace) const
{
	const Matrix worldViewProjection{ m_TranslationMatrix * viewProjection };
	const Vector3Int position{ m_TranslationMatrix.GetTranslation() };

	for (int i{}; i <= static_cast<int>(Face::FaceDirection::BOTTOM); ++i)
	{
		if (isBlockPredicate(position + m_NeighbouringBlocks[i])) continue;

		const Matrix& faceTransformation{ m_FaceTransformations[i] };

		pFace->Render(pDeviceContext, faceTransformation * m_TranslationMatrix, faceTransformation * worldViewProjection);
	}
}
