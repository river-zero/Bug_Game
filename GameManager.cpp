#include "GameManager.h"
#include "Bug.h"

HRESULT GameManager::Initialize(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height) {
    // ��ӹ��� D2DFramework �ʱ�ȭ
    HRESULT hr = D2DFramework::Initialize(hInstance, title, width, height);
    ThrowIfFailed(hr, "D2DFrmaework::Initialize Failed");

    // ��� �̹����� ��Ÿ���� Actor ��ü �ʱ�ȭ
    mspBackground = std::make_shared<Actor>(this, L"Images/background.jpg", 0.0f, 0.0f, 1.0f);

    // Bug Ŭ�����κ��� �Ļ��� Bug ��ü�� 40�� �����Ͽ� ����Ʈ�� �߰�
    for (int i = 0; i < 40; i++) {
        mBugList.push_back(std::make_shared<Bug>(this));
    }

    return S_OK;
}

void GameManager::Render() {
    mspRenderTarget->BeginDraw();

    // ��ȯ ��Ʈ���� �ʱ�ȭ �� ��� ���� ����
    // ��ȯ ��Ʈ������ ���� ��ķ� �����ϸ� � ��ȯ�� ������� ���� ���·� �ʱ�ȭ��
    mspRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
    mspRenderTarget->Clear(D2D1::ColorF(0.0f, 0.2f, 0.4f, 1.0f));

    CheckBugs();

    // ���� ���� �̹��� �׸���
    mspBackground->Draw();
    for (auto& bug : mBugList) {
        bug->Draw();
    }

    HRESULT hr = mspRenderTarget->EndDraw();

    if (hr == D2DERR_RECREATE_TARGET) {
        CreateDeviceResources();
    }
}

void GameManager::Release() {
    mBugList.clear();
    mspBackground.reset();

    D2DFramework::Release();
}

void GameManager::CheckBugs() {
    // ���� ���콺�� Ŭ���Ǿ����� Ȯ��
    if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
        // ���� ���콺 Ŀ�� ��ġ ��������
        POINT pt;
        GetCursorPos(&pt);
        ScreenToClient(mHwnd, &pt);

        // ���� ����Ʈ ��ȸ �� Ŭ���� ���� Ȯ��
        auto itr = std::remove_if(mBugList.begin(), mBugList.end(),
            [&](auto& actor) {
                // Ŭ���� ������ üũ�ϰ� �׾��ٸ� ����Ʈ���� ����
                Bug* p = static_cast<Bug*>(actor.get());
                p->IsClicked(pt);
                if (p->mIsDead) {
                    return true;
                } else {
                    return false;
                }
            }
        );

        // ���ŵ� ���� ��ҵ��� ���Ϳ��� ������ ����
        mBugList.erase(itr, mBugList.end());
    }
}