#include "BitmapManager.h"

#pragma comment (lib, "WindowsCodecs.lib")

using namespace std;
using namespace Microsoft::WRL;

HRESULT BitmapManager::LoadWICBitmap(std::wstring filename, ID2D1Bitmap** ppBitmap) {
	// WIC 디코더 및 포맷 컨버터 생성
	Microsoft::WRL::ComPtr<IWICBitmapDecoder> decoder;
	HRESULT hr;

	auto pWICFactory = mspWICFactory.Get();
	if (pWICFactory == nullptr) {
		return E_FAIL;
	}

	// WIC 디코더를 생성하고 이미지 파일에서 비트맵을 로드
	hr = pWICFactory->CreateDecoderFromFilename(
		filename.c_str(), 
		nullptr, 
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad, 
		decoder.GetAddressOf());
	ThrowIfFailed(hr);

	Microsoft::WRL::ComPtr<IWICBitmapFrameDecode> frame;
	ThrowIfFailed(decoder->GetFrame(0, frame.GetAddressOf()));

	Microsoft::WRL::ComPtr<IWICFormatConverter> converter;
	ThrowIfFailed(pWICFactory->CreateFormatConverter(converter.GetAddressOf()));

	hr = converter->Initialize(
		frame.Get(), 
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone, 
		nullptr,
		0, 
		WICBitmapPaletteTypeCustom);
	ThrowIfFailed(hr);

	// 렌더 타겟에서 비트맵 생성
	auto pRT = mpRenderTarget;
	if (pRT == nullptr) {
		return E_FAIL;
	}
	hr = pRT->CreateBitmapFromWicBitmap(converter.Get(), ppBitmap);
	ThrowIfFailed(hr);

	return S_OK;
}

HRESULT BitmapManager::Initialize(ID2D1HwndRenderTarget* pRT) {
	if (!pRT) {
		return E_FAIL;
	}

	mpRenderTarget = pRT;

	HRESULT hr = ::CoCreateInstance(
		CLSID_WICImagingFactory,
		nullptr,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(mspWICFactory.GetAddressOf()));
	ThrowIfFailed(hr, "WICFactory Creation Failed");

	return S_OK;
}

void BitmapManager::Release() {
	mBitmapResources.clear();
	mspWICFactory.Reset();
}

ID2D1Bitmap* BitmapManager::LoadBitmap(std::wstring filename) {
	if (!mspWICFactory) {
		ThrowIfFailed(E_FAIL, "WICFactory must not null");
		return nullptr;
	}

	// 이미지 파일 로드 여부를 확인하고 로드되지 않았다면 맵에 추가
	// 이미지 파일에 대한 비트맵이 맵에 이미 있다면 해당 비트맵을 반환
	auto result = mBitmapResources.insert({ filename, nullptr });
	if (result.second == true) { // 키가 새로 추가되었다면 실행해 불필요한 탐색을 줄임
		auto spBitmap = ComPtr<ID2D1Bitmap>();
		LoadWICBitmap(filename, spBitmap.GetAddressOf());
		result.first->second = spBitmap;
	}

	return result.first->second.Get();
}