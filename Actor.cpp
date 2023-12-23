#include "Actor.h"

Actor::Actor(D2DFramework* pFramework, LPCWSTR filename) :
	mpFramework(pFramework),
	mpBitmap(),
	mX(),
	mY(),
	mOpacity(1.0f) {
	// BitmapManager¸¦ »ç¿ëÇÏ¿© ÀÌ¹ÌÁö ·Îµù ¹× ºñÆ®¸Ê È¹µæ
	mpBitmap = BitmapManager::Instance().LoadBitmap(filename);
}

Actor::Actor(D2DFramework* pFramework, LPCWSTR filename, float x, float y, float opacity) :
	Actor(pFramework, filename) {
	mX = x;
	mY = y;
	mOpacity = opacity;
}

Actor::~Actor() {
}

void Actor::Draw(float x, float y, float opacity) {
	// ·»´õ Å¸°Ù È¹µæ
	auto pRT = mpFramework->GetRenderTarget();
	if (pRT == nullptr) {
		return;
	}

	// ºñÆ®¸Ê Å©±â ¹× ±×·ÁÁú ¿µ¿ª ¼³Á¤
	auto size{ mpBitmap->GetPixelSize() };
	D2D1_RECT_F rect{ x, y,
		static_cast<float>(x + size.width),
		static_cast<float>(y + size.height) };

	// ºñÆ®¸ÊÀ» ·»´õ Å¸°Ù¿¡ ±×¸²
	pRT->DrawBitmap(mpBitmap, rect, opacity);
}

void Actor::Draw() {
	Draw(mX, mY, mOpacity);
}