#pragma once

// 실제 게임은 수백에서 수천의 이미지를 사용하기 때문에
// 어떤 그림 파일이 중복으로 로딩되는지 확인하기 어려움
// 따라서 이런 과정을 관리해주는 관리자를 두면 편함
// 관리자의 위치는 D2DFramework 내부가 적절

#include "com_exception.h"
#include <wrl/client.h>
#include <d2d1.h>
#include <wincodec.h>
#include <map>
#include <string>

// 싱글턴 패턴 적용
class BitmapManager final {
public:
	static BitmapManager& Instance() {
		static BitmapManager instance;
		return instance;
	}

private:
	BitmapManager() {}
	BitmapManager(const BitmapManager&) {}
	void operator=(const BitmapManager&) {}

public:
	~BitmapManager() {}

private:
	ID2D1HwndRenderTarget* mpRenderTarget{};
	Microsoft::WRL::ComPtr<IWICImagingFactory> mspWICFactory{};

	std::map<std::wstring, Microsoft::WRL::ComPtr<ID2D1Bitmap>> mBitmapResources;
	// 이미지를 관리하기 위해서 <파일명, 비트맵> 형태의 쌍이 필요

private:
	HRESULT LoadWICBitmap(std::wstring filename, ID2D1Bitmap** ppBitmap);

public:
	// 싱글턴 패턴에서는 생성자와 소멸자에 의지하기 어려우니 
	// 명시적인 초기화 및 해제 함수가 있으면 좋음
	HRESULT Initialize(ID2D1HwndRenderTarget* pRT);
	void Release();

	ID2D1Bitmap* LoadBitmap(std::wstring filename);
	// 관리자의 핵심 함수
	// 맵에 데이터의 존재를 확인하고 관리하는 기능
};