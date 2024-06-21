#include "CLensFlare.h"


void UpdateVertexBuffer(ID3D11DeviceContext* context, ID3D11Buffer* vertexBuffer, const rage::Vec3V& start, const rage::Vec3V& end)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	if (FAILED(context->Map(vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
		return;
	
	float* data = reinterpret_cast<float*>(mappedResource.pData);

	data[0] = start.X();
	data[1] = start.Y();

	data[2] = 0.01f;
	data[3] = 0.0f;
	data[4] = 1.0f;
	data[5] = 1.0f;

	data[6] = end.X();
	data[7] = end.Y();

	data[8] = 1.0f;
	data[9] = 0.0f;
	data[10] = 0.0f;
	data[11] = 1.0f;

	context->Unmap(vertexBuffer, 0);
}

//void LensFlareHandler::LensFlaresDrawLines()
//{
//	if (!(self && sm_IsFlareFxRenderedOnThisFrame))
//		return;
//
//
//	//p_DeviceContext->IASetVertexBuffers(0, 1, self->m_VertexBuffer.GetAddressOf(), &stride, &offset);
//	//p_DeviceContext->IASetInputLayout(self->m_InputLayout.Get());
//	//p_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
//
//	//p_DeviceContext->VSSetShader(self->m_vertexShader.Get(), nullptr, 0);
//	//p_DeviceContext->PSSetShader(self->m_pixelShader.Get(), nullptr, 0);
//	//p_DeviceContext->GSSetShader(self->m_GeometryShader.Get(), nullptr, 0);
//
//
//	//p_DeviceContext->Draw(2, 0);
//}


rage::ScalarV LengthVec2(rage::Vec3V V)
{
	return std::sqrt(V.X() * V.X() + V.Y() * V.Y());
}

rage::ScalarV LengthVec3(rage::Vec3V V)
{
	return std::sqrt(V.X() * V.X() + V.Y() * V.Y() + V.Z() * V.Z());
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


LensFlareHandler* LensFlareHandler::self = nullptr;

rage::Vec3V	LensFlareHandler::sm_StartPoint = { 0.0f, 0.0f, 0.0f };
rage::Vec3V	LensFlareHandler::sm_EndPoint = { 0.0f, 0.0f, 0.0f };

gmAddress s_RenderFlareFxAddr;

void ScreenPosPointToDX11Format(rage::Vec3V& p1)
{
	p1 = { (p1.X() * 2.0f - 1.0f), -(p1.Y() * 2.0f - 1.0f) };
}

void(*RenderFlareFxfn)(u64, u64, u64, u64, float*);
void LensFlareHandler::n_RenderFlareFx(u64 arg1, u64 arg2, u64 arg3, u64 arg4, float* SunPos)
{
	using namespace rage;
	RenderFlareFxfn(arg1, arg2, arg3, arg4, SunPos);

	sm_StartPoint = { SunPos[0], SunPos[1], 0 };

	Vec3V vFlareDirNorm = Vec3V(0.5f, 0.5f) - sm_StartPoint;
	ScalarV fLen = LengthVec2(vFlareDirNorm);

	Vec3V vDist = vFlareDirNorm.Abs() + Vec3V(0.5f, 0.5f);
	vFlareDirNorm /= fLen;

	ScalarV fDistToEdge = LengthVec2(vDist);

	Vec3V vFlareDir = vFlareDirNorm * fDistToEdge;
	sm_EndPoint = sm_StartPoint + vFlareDir;
	
	ScreenPosPointToDX11Format(sm_StartPoint);
	ScreenPosPointToDX11Format(sm_EndPoint);

	if (self)
		self->Update();
}


void LensFlareHandler::Update()
{
	DrawLine2D(sm_StartPoint, sm_EndPoint, { 1.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f });
}

LensFlareHandler::LensFlareHandler()
{
	self = this;
	m_CLensFlare = gmAddress::Scan("48 8D 0D ?? ?? ?? ?? 75 ?? 33 D2").GetRef(3).To<gCLensFlare*>();
	s_RenderFlareFxAddr = gmAddress::Scan("48 8B C4 F3 0F 11 58 ?? 4C 89 40 ?? 55");
	Hook::Create(s_RenderFlareFxAddr, n_RenderFlareFx, &RenderFlareFxfn, "RenderFlareFx");
}


LensFlareHandler::~LensFlareHandler()
{
	Hook::Remove(s_RenderFlareFxAddr);
	self = nullptr;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


const char* LensFlareHandler::GetFileNameAtIndex(size_t idx)
{
	static std::map<LensFlareFile_e, const char*> FileNamesMap
	{
		{ lensflare_m, "M" },
		{ lensflare_f, "F" },
		{ lensflare_t, "T" },
	};

	auto fName_e = static_cast<LensFlareFile_e>(idx);
	if (!FileNamesMap.contains(fName_e)) {
		return "fNone";
	}
	else {
		return FileNamesMap.at(fName_e);
	}
}

const char* LensFlareHandler::GetTextureTypeName(u8 NumTexture)
{
	static std::map<FlareFxTextureType_e, const char*> NamesMap
	{
		{ AnimorphicType,"AnimorphicFx" },
		{ ArtefactType,	 "ArtefactFx" },
		{ ChromaticType, "ChromaticFx" },
		{ CoronaType,	 "CoronaFx"	},
	};

	auto name_e = static_cast<FlareFxTextureType_e>(NumTexture);
	if (!NamesMap.contains(name_e)) {
		return "Unknown";
	}
	else {
		return NamesMap.at(name_e);
	}
}


CFlareFX::CFlareFX(FlareFxTextureType_e type)
{
	switch (type)
	{
	case AnimorphicType:

		m_fDistFromLight = 0.2f;
		m_fSize = 0.2f;
		m_fWidthRotate = 0.0f;
		m_fScrollSpeed = 0.0f;
		m_fCurrentScroll = 0.0f;
		m_fDistanceInnerOffset = 0.0f;
		m_fIntensityScale = 100.f;
		m_fIntensityFade = 0.0f;
		m_fAnimorphicScaleFactorU = 0.0f;
		m_fAnimorphicScaleFactorV = 0.0f;
		m_color = { 1.0f, 1.0f, 1.0f, 1.0f };
		m_fTextureColorDesaturate = 0.0f;
		m_fGradientMultiplier = 0.0f;
		m_nTexture = 0;
		m_nSubGroup = 0;
		m_bAnimorphicBehavesLikeArtefact = false;
		m_bAlignRotationToSun = false;
		m_fPositionOffsetU = 0.0f;

		break;
	case ArtefactType:


		break;
	case ChromaticType:


		break;
	case CoronaType:


		break;
	default:
		break;
	}
}