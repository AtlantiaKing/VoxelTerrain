#pragma once
#include <SDL_surface.h>
#include <string>
#include "ColorRGB.h"

namespace dae
{
	struct Vector2;

	class Texture final
	{
	public:
		~Texture();

		static Texture* LoadFromFile(ID3D11Device* pDevice, const std::string& path);

		ID3D11Texture2D* GetResource() const;
		ID3D11ShaderResourceView* GetSRV() const;
	private:
		Texture(ID3D11Device* pDevice, SDL_Surface* pSurface);

		ID3D11Texture2D* m_pResource{};
		ID3D11ShaderResourceView* m_pSRV{};
	};
}