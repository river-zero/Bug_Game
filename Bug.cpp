#include <algorithm>
#include "Bug.h"

Bug::Bug(D2DFramework* pFramework, LPCWSTR imagePath, float moveSpeed, bool isClickable) : Actor(pFramework, imagePath) {
	// 난수 생성을 위한 시드 초기화
	std::random_device rd;
	std::mt19937 rand(rd());

	// 윈도우 클라이언트 영역을 얻어옴
	RECT clientRect{};
	GetClientRect(pFramework->GetHWND(), &clientRect);

	// 클라이언트 영역 내에서 균등하게 랜덤한 좌표 생성
	std::uniform_real_distribution<float> distributionX(static_cast<float>(clientRect.left), static_cast<float>(clientRect.right));
	std::uniform_real_distribution<float> distributionY(static_cast<float>(clientRect.top), static_cast<float>(clientRect.bottom));
	mX = distributionX(rand);
	mY = distributionY(rand);

	// 랜덤한 회전 각도로 초기화
	std::uniform_real_distribution<float> distributionRotation(0.0f, 360.0f);
	mRotation = distributionRotation(rand);

	mIsClickable = isClickable;
	mMoveSpeed = moveSpeed;
	mTargetRotation = 0.0f;
	mSteps = 0.0f;
	mIsDead = false;
}

void Bug::Draw() {
	auto pRT = mpFramework->GetRenderTarget();
	if (!pRT) { return; }

	auto size = mpBitmap->GetPixelSize();

	// 일정 이동 거리가 될 때마다 회전 각도 변경
	if (mSteps++ > 30) {
		mSteps = 0;

		// 랜덤으로 목표 회전 각도 설정
		mTargetRotation = (1 - rand() % 3) * 45.0f;
	}

	if (mIsClickable) {
		// Bug를 현재 위치에서 이동 방향으로 이동
		auto forward = UPVECTOR * D2D1::Matrix3x2F::Rotation(mRotation);
		mX += forward.x * mMoveSpeed;
		mY += forward.y * mMoveSpeed;

		// 현재 회전 각도를 목표 회전 각도로 조금씩 변경
		if (mRotation < mTargetRotation) {
			mRotation += 1.0f;
		} else if (mRotation > mTargetRotation) {
			mRotation -= 1.0f;
		}
	} else {
		// 현재 마우스 위치를 얻음
		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(mpFramework->GetHWND(), &pt);

		// 벌레와 마우스 사이의 벡터를 계산
		D2D_VECTOR_2F direction = { pt.x - mX, pt.y - mY };

		// 벡터를 정규화
		float length = sqrt(direction.x * direction.x + direction.y * direction.y);
		direction.x /= length;
		direction.y /= length;

		// 벌레를 마우스의 방향으로 이동
		mX += direction.x * mMoveSpeed;
		mY += direction.y * mMoveSpeed;

		// 마우스를 향한 각도를 계산
		float targetRotation = atan2(direction.y, direction.x) * 180.0f / 3.14159265f;

		// 현재 회전 각도를 목표 회전 각도로 조금씩 변경
		if (mRotation < targetRotation) {
			mRotation += 1.0f;
		} else if (mRotation > targetRotation) {
			mRotation -= 1.0f;
		}
	}

	// 클라이언트 영역 내에서만 움직이도록 범위 제한
	auto clientSize = pRT->GetSize();
	mX = std::clamp(mX, 0.0f, clientSize.width - size.width);
	mY = std::clamp(mY, 0.0f, clientSize.height - size.height);

	// 만약 벌레가 클라이언트 영역의 끝에 도달했다면 180도 회전
	if (mX <= 0.0f || mX >= clientSize.width - size.width || mY <= 0.0f || mY >= clientSize.height - size.height) {
		mRotation += 180.0f;
	}

	// Bug의 위치와 회전을 반영한 변환 행렬 설정
	auto matTranslate = D2D1::Matrix3x2F::Translation(mX, mY);
	auto matRotation = D2D1::Matrix3x2F::Rotation(mRotation, D2D_POINT_2F{ size.width * 0.5f, size.height * 0.5f });
	pRT->SetTransform(matRotation * matTranslate);
	// 행렬이니까 곱하기 순서에 주의
	// 변환 행렬은 디바이스 종속이기 때문에 한 번 지정하면 이후의 모든 그리기에 영향
	// 따라서 복잡한 그리기에는 주의가 필요

	// Bug 이미지를 현재 위치와 행렬 변환에 따라 그리기
	D2D1_RECT_F rect{ 0, 0, static_cast<float>(size.width), static_cast<float>(size.height) };
	mpFramework->GetRenderTarget()->DrawBitmap(mpBitmap, rect, mOpacity);
}

bool Bug::IsClicked(POINT& pt) {
	auto size = mpBitmap->GetPixelSize();

	// Bug의 영역에 클릭이 발생하면 mIsDead를 true로 설정
	if (mIsClickable && pt.x >= mX && pt.y >= mY
		&& pt.x <= mX + size.width && pt.y <= mY + size.height) {
		mIsDead = true;
		return true;
	}

	return false;
}