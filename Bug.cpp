#include <algorithm>
#include "Bug.h"

Bug::Bug(D2DFramework* pFramework, LPCWSTR imagePath, float moveSpeed, bool isClickable) : Actor(pFramework, imagePath) {
	// ���� ������ ���� �õ� �ʱ�ȭ
	std::random_device rd;
	std::mt19937 rand(rd());

	// ������ Ŭ���̾�Ʈ ������ ����
	RECT clientRect{};
	GetClientRect(pFramework->GetHWND(), &clientRect);

	// Ŭ���̾�Ʈ ���� ������ �յ��ϰ� ������ ��ǥ ����
	std::uniform_real_distribution<float> distributionX(static_cast<float>(clientRect.left), static_cast<float>(clientRect.right));
	std::uniform_real_distribution<float> distributionY(static_cast<float>(clientRect.top), static_cast<float>(clientRect.bottom));
	mX = distributionX(rand);
	mY = distributionY(rand);

	// ������ ȸ�� ������ �ʱ�ȭ
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

	// ���� �̵� �Ÿ��� �� ������ ȸ�� ���� ����
	if (mSteps++ > 30) {
		mSteps = 0;

		// �������� ��ǥ ȸ�� ���� ����
		mTargetRotation = (1 - rand() % 3) * 45.0f;
	}

	if (mIsClickable) {
		// Bug�� ���� ��ġ���� �̵� �������� �̵�
		auto forward = UPVECTOR * D2D1::Matrix3x2F::Rotation(mRotation);
		mX += forward.x * mMoveSpeed;
		mY += forward.y * mMoveSpeed;

		// ���� ȸ�� ������ ��ǥ ȸ�� ������ ���ݾ� ����
		if (mRotation < mTargetRotation) {
			mRotation += 1.0f;
		} else if (mRotation > mTargetRotation) {
			mRotation -= 1.0f;
		}
	} else {
		// ���� ���콺 ��ġ�� ����
		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(mpFramework->GetHWND(), &pt);

		// ������ ���콺 ������ ���͸� ���
		D2D_VECTOR_2F direction = { pt.x - mX, pt.y - mY };

		// ���͸� ����ȭ
		float length = sqrt(direction.x * direction.x + direction.y * direction.y);
		direction.x /= length;
		direction.y /= length;

		// ������ ���콺�� �������� �̵�
		mX += direction.x * mMoveSpeed;
		mY += direction.y * mMoveSpeed;

		// ���콺�� ���� ������ ���
		float targetRotation = atan2(direction.y, direction.x) * 180.0f / 3.14159265f;

		// ���� ȸ�� ������ ��ǥ ȸ�� ������ ���ݾ� ����
		if (mRotation < targetRotation) {
			mRotation += 1.0f;
		} else if (mRotation > targetRotation) {
			mRotation -= 1.0f;
		}
	}

	// Ŭ���̾�Ʈ ���� �������� �����̵��� ���� ����
	auto clientSize = pRT->GetSize();
	mX = std::clamp(mX, 0.0f, clientSize.width - size.width);
	mY = std::clamp(mY, 0.0f, clientSize.height - size.height);

	// ���� ������ Ŭ���̾�Ʈ ������ ���� �����ߴٸ� 180�� ȸ��
	if (mX <= 0.0f || mX >= clientSize.width - size.width || mY <= 0.0f || mY >= clientSize.height - size.height) {
		mRotation += 180.0f;
	}

	// Bug�� ��ġ�� ȸ���� �ݿ��� ��ȯ ��� ����
	auto matTranslate = D2D1::Matrix3x2F::Translation(mX, mY);
	auto matRotation = D2D1::Matrix3x2F::Rotation(mRotation, D2D_POINT_2F{ size.width * 0.5f, size.height * 0.5f });
	pRT->SetTransform(matRotation * matTranslate);
	// ����̴ϱ� ���ϱ� ������ ����
	// ��ȯ ����� ����̽� �����̱� ������ �� �� �����ϸ� ������ ��� �׸��⿡ ����
	// ���� ������ �׸��⿡�� ���ǰ� �ʿ�

	// Bug �̹����� ���� ��ġ�� ��� ��ȯ�� ���� �׸���
	D2D1_RECT_F rect{ 0, 0, static_cast<float>(size.width), static_cast<float>(size.height) };
	mpFramework->GetRenderTarget()->DrawBitmap(mpBitmap, rect, mOpacity);
}

bool Bug::IsClicked(POINT& pt) {
	auto size = mpBitmap->GetPixelSize();

	// Bug�� ������ Ŭ���� �߻��ϸ� mIsDead�� true�� ����
	if (mIsClickable && pt.x >= mX && pt.y >= mY
		&& pt.x <= mX + size.width && pt.y <= mY + size.height) {
		mIsDead = true;
		return true;
	}

	return false;
}