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
			faceWorldViewProjection = Matrix::CreateTranslation({ 0.0f, 0.0f, -0.5f }) * worldViewProjection;
			faceWorldMatrix = Matrix::CreateTranslation({ 0.0f, 0.0f, -0.5f }) * m_TranslationMatrix;
			break;
		}
		case Face::FaceDirection::RIGHT:
		{
			faceWorldViewProjection = Matrix::CreateRotationY(90.0f * TO_RADIANS) * Matrix::CreateTranslation({ -0.5f, 0.0f, 0.0f }) * worldViewProjection;
			faceWorldMatrix = Matrix::CreateRotationY(90.0f * TO_RADIANS) * Matrix::CreateTranslation({ -0.5f, 0.0f, 0.0f }) * m_TranslationMatrix;
			break;
		}
		case Face::FaceDirection::LEFT:
		{
			faceWorldViewProjection = Matrix::CreateRotationY(-90.0f * TO_RADIANS) * Matrix::CreateTranslation({ 0.5f, 0.0f, 0.0f }) * worldViewProjection;
			faceWorldMatrix = Matrix::CreateRotationY(-90.0f * TO_RADIANS) * Matrix::CreateTranslation({ 0.5f, 0.0f, 0.0f }) * m_TranslationMatrix;
			break;
		}
		case Face::FaceDirection::BACK:
		{
			faceWorldViewProjection = Matrix::CreateRotationY(180.0f * TO_RADIANS) * Matrix::CreateTranslation({ 0.0f, 0.0f, 0.5f }) * worldViewProjection;
			faceWorldMatrix = Matrix::CreateRotationY(180.0f * TO_RADIANS) * Matrix::CreateTranslation({ 0.0f, 0.0f, 0.5f }) * m_TranslationMatrix;
			break;
		}
		case Face::FaceDirection::UP:
		{
			faceWorldViewProjection = Matrix::CreateRotationX(-90.0f * TO_RADIANS) * Matrix::CreateTranslation({ 0.0f, 0.5f, 0.0f }) * worldViewProjection;
			faceWorldMatrix = Matrix::CreateRotationX(-90.0f * TO_RADIANS) * Matrix::CreateTranslation({ 0.0f, 0.5f, 0.0f }) * m_TranslationMatrix;
			break;
		}
		case Face::FaceDirection::BOTTOM:
		{
			faceWorldViewProjection = Matrix::CreateRotationX(90.0f * TO_RADIANS) * Matrix::CreateTranslation({ 0.0f, -0.5f, 0.0f }) * worldViewProjection;
			faceWorldMatrix = Matrix::CreateRotationX(90.0f * TO_RADIANS) * Matrix::CreateTranslation({ 0.0f, -0.5f, 0.0f }) * m_TranslationMatrix;
			break;
		}
		}

		pFace->Render(pDeviceContext, faceWorldMatrix, faceWorldViewProjection, Face::FaceDirection::FORWARD);
	}
}
