#pragma once

namespace dae
{
	class Texture;

	class Material final
	{
	public:
		Material(ID3D11Device* pDevice, const std::wstring& assetFile);
		~Material();

		Material(const Material&)				= delete;
		Material(Material&&)					= delete;
		Material& operator=(const Material&)	= delete;
		Material& operator=(Material&&)			= delete;

		void SetMatrix(const Matrix& worldViewProjection);
		void SetTexture(Texture* pTexture);
		ID3DX11Effect* GetEffect() const;
		ID3DX11EffectTechnique* GetTechnique() const;

		ID3D11InputLayout* LoadInputLayout(ID3D11Device* pDevice);
	protected:
		ID3DX11Effect* m_pEffect{};
		ID3DX11EffectTechnique* m_pTechnique{};
		ID3DX11EffectMatrixVariable* m_pMatWorldViewProjVariable{};
		ID3DX11EffectSamplerVariable* m_pSamplerStateVariable{};
		ID3DX11EffectShaderResourceVariable* m_pDiffuseMapVariable{};

		static ID3DX11Effect* LoadEffect(ID3D11Device* pDevice, const std::wstring& assetFile);
	};
}

