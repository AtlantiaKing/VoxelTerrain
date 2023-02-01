#include "pch.h"
#include "Block.h"
#include "Face.h"

dae::Block::Block(const Vector3 position)
	: m_TranslationMatrix{ Matrix::CreateTranslation(position) }
{
}

void dae::Block::Render(ID3D11DeviceContext* pDeviceContext, const Matrix& viewProjection, Face* pFace) const
{
	const Matrix worldViewProjection{ m_TranslationMatrix * viewProjection };

	for (int i{}; i <= static_cast<int>(Face::FaceDirection::BOTTOM); ++i)
	{
		Matrix faceWorldViewProjection{};
		Matrix faceWorldMatrix{};

		switch (static_cast<Face::FaceDirection>(i))
		{
		case Face::FaceDirection::FORWARD:
		{
			const Matrix faceTransformation{ Matrix::CreateTranslation({ 0.0f, 0.0f, -0.5f }) };
			faceWorldViewProjection = faceTransformation * worldViewProjection;
			faceWorldMatrix = faceTransformation * m_TranslationMatrix;
			break;
		}
		case Face::FaceDirection::RIGHT:
		{
			const Matrix faceTransformation{ Matrix::CreateRotationY(90.0f * TO_RADIANS) * Matrix::CreateTranslation({ -0.5f, 0.0f, 0.0f }) };
			faceWorldViewProjection = faceTransformation * worldViewProjection;
			faceWorldMatrix = faceTransformation * m_TranslationMatrix;
			break;
		}
		case Face::FaceDirection::LEFT:
		{
			const Matrix faceTransformation{ Matrix::CreateRotationY(-90.0f * TO_RADIANS) * Matrix::CreateTranslation({ 0.5f, 0.0f, 0.0f }) };
			faceWorldViewProjection = faceTransformation * worldViewProjection;
			faceWorldMatrix = faceTransformation * m_TranslationMatrix;
			break;
		}
		case Face::FaceDirection::BACK:
		{
			const Matrix faceTransformation{ Matrix::CreateRotationY(180.0f * TO_RADIANS) * Matrix::CreateTranslation({ 0.0f, 0.0f, 0.5f }) };
			faceWorldViewProjection = faceTransformation * worldViewProjection;
			faceWorldMatrix = faceTransformation * m_TranslationMatrix;
			break;
		}
		case Face::FaceDirection::UP:
		{
			const Matrix faceTransformation{ Matrix::CreateRotationX(-90.0f * TO_RADIANS) * Matrix::CreateTranslation({ 0.0f, 0.5f, 0.0f }) };
			faceWorldViewProjection = faceTransformation * worldViewProjection;
			faceWorldMatrix = faceTransformation * m_TranslationMatrix;
			break;
		}
		case Face::FaceDirection::BOTTOM:
		{
			const Matrix faceTransformation{ Matrix::CreateRotationX(90.0f * TO_RADIANS) * Matrix::CreateTranslation({ 0.0f, -0.5f, 0.0f }) };
			faceWorldViewProjection = faceTransformation * worldViewProjection;
			faceWorldMatrix = faceTransformation * m_TranslationMatrix;
			break;
		}
		}

		pFace->Render(pDeviceContext, faceWorldMatrix, faceWorldViewProjection, Face::FaceDirection::FORWARD);
	}
}
