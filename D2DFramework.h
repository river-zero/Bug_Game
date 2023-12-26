#pragma once

#include <d2d1.h>
#include <dwrite.h>
#include <wrl/client.h>
#include "com_exception.h"
#include "BitmapManager.h"

class D2DFramework {
private:
	const LPCWSTR gClassName{ L"MyWindowClass" };

protected:
	HWND mHwnd{};

	Microsoft::WRL::ComPtr<ID2D1Factory> mspD2DFactory{};
	Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> mspRenderTarget{};

	// 화면에 텍스트를 그리기 위한 변수들
	Microsoft::WRL::ComPtr<IDWriteFactory> mspWriteFactory{};
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> mspTextBrush{};

protected:
	HRESULT InitWindow(HINSTANCE hInstance, LPCWSTR title = L"D2DFramework", UINT w = 1024, UINT h = 768);
	virtual HRESULT InitD2D(HWND hwnd);
	virtual HRESULT CreateDeviceResources();

public:
	virtual HRESULT Initialize(HINSTANCE hInstance, LPCWSTR title = L"D2DFramework", UINT w = 1024, UINT h = 768);

	virtual void PresentText(
		const WCHAR* text, 
		FLOAT x,
		FLOAT y,
		FLOAT width, 
		FLOAT height, 
		const WCHAR* fontFamily = L"맑은고딕", 
		FLOAT fontSize = 20.0f, 
		const D2D1_COLOR_F& textColor = D2D1::ColorF(D2D1::ColorF::Black));

	virtual void Release();
	virtual int GameLoop();
	virtual void Render();

	void ShowError(LPCWSTR msg, LPCWSTR title = L"Error");

	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

public:
	inline ID2D1HwndRenderTarget* GetRenderTarget() const { return mspRenderTarget.Get(); }
	inline ID2D1Factory* GetD2DFactory() const { return mspD2DFactory.Get(); }
	inline HWND GetHWND() const { return mHwnd; }
};