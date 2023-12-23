#include "Bug.h"

Bug::Bug(D2DFramework* pFramework) : Actor(pFramework, L"Images/bug1.png") {
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

	mRotation = 0.0f;
	mSteps = 0.0f;
	mIsDead = false;
}

void Bug::Draw() {
	auto pRT = mpFramework->GetRenderTarget();
	if (!pRT) {
		return;
	}

	auto size = mpBitmap->GetPixelSize();

	// ���� �̵� �Ÿ��� �� ������ ȸ�� ���� ����
	if (mSteps++ > 30) {
		mSteps = 0;

		// �������� ȸ�� ���� ����
		mRotation += (1 - rand() % 3) * 45.0f;
	}

	// Bug�� ���� ��ġ���� �̵� �������� �̵�
	auto forward = UPVECTOR * D2D1::Matrix3x2F::Rotation(mRotation);
	mX += forward.x;
	mY += forward.y;

	// Bug�� ��ġ�� ȸ���� �ݿ��� ��ȯ ��� ����
	auto matTranslate = D2D1::Matrix3x2F::Translation(mX, mY);
	auto matRotation = D2D1::Matrix3x2F::Rotation(mRotation, D2D_POINT_2F{ size.width * 0.5f, size.height * 0.5f });
	pRT->SetTransform(matRotation * matTranslate);
	// ����̴ϱ� ���ϱ� ������ ����
	// ��ȯ ����� ����̽� �����̱� ������ �� �� �����ϸ� ������ ��� �׸��⿡ ����
	// ���� ������ �׸��⿡�� ���ǰ� �ʿ�

	// Bug �̹����� ���� ��ġ�� ��� ��ȯ�� ���� �׸���
	D2D1_RECT_F rect{ 0,0,static_cast<float>(size.width), static_cast<float>(size.height) };
	mpFramework->GetRenderTarget()->DrawBitmap(mpBitmap, rect, mOpacity);
}

bool Bug::IsClicked(POINT& pt) {
	auto size = mpBitmap->GetPixelSize();

	// Bug�� ������ Ŭ���� �߻��ϸ� mIsDead�� true�� ����
	if (pt.x >= mX && pt.y >= mY
		&& pt.x <= mX + size.width && pt.y <= mY + size.height) {
		mIsDead = true;
		return true;
	}

	return false;
}