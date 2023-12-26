#include "GameManager.h"
#include "Bug.h"

HRESULT GameManager::Initialize(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height) {
    // ��ӹ��� D2DFramework �ʱ�ȭ
    HRESULT hr = D2DFramework::Initialize(hInstance, title, width, height);
    ThrowIfFailed(hr, "D2DFrmaework::Initialize Failed");

    // ��� �̹����� ��Ÿ���� Actor ��ü �ʱ�ȭ
    mspBackground = std::make_shared<Actor>(this, L"Images/background.jpg", 0.0f, 0.0f, 1.0f);

    // Bug Ŭ�����κ��� �Ļ��� Bug ��ü�� 40�� �����Ͽ� ����Ʈ�� �߰�
    for (int i = 0; i < 15; i++) {
        mBugList.push_back(std::make_shared<Bug>(this, L"Images/bug1.png", 2.5f, true));
    }

    mGameStart = false;
    mGameFail = false;
    mGameSuccess = false;
    mBugGenerated = false;
    mGameStartTime = std::chrono::steady_clock::now();

    return S_OK;
}

void GameManager::Render() {
    mspRenderTarget->BeginDraw();

    // ��ȯ ��Ʈ���� �ʱ�ȭ �� ��� ���� ����
    // ��ȯ ��Ʈ������ ���� ��ķ� �����ϸ� � ��ȯ�� ������� ���� ���·� �ʱ�ȭ��
    mspRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
    mspRenderTarget->Clear(D2D1::ColorF(0.0f, 0.2f, 0.4f, 1.0f));

    // ��� �׸���
    mspBackground->Draw();

    if (!mGameStart) {
        // ���� ���� �� ���� ���
        PresentText(L"������ �����ϼ���", 350, 290, 800, 50, L"�������", 40);
        PresentText(L"���콺 ���� ��ư Ŭ��", 410, 370, 800, 50, L"�������", 20);

        // ���� ���콺 ��ư�� Ŭ���Ǹ� ���� ����
        if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
            mGameStart = true;
        }
    } else if (mGameFail) {
        PresentText(L"����", 470, 320, 800, 50, L"�������", 40);

        if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
            ResetGame();
            Render();
        }
    } else if (mGameSuccess) {
        PresentText(L"����", 470, 320, 800, 50, L"�������", 40);

        if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
            ResetGame();
            Render();
        }
    } else {
        CheckBugs();

        // ���� �̹��� �׸���
        for (auto& bug : mBugList) {
            bug->Draw();
        }

        if (mBugList.empty() && !mBugGenerated) {
            PresentText(L"1�ܰ� Ŭ����", 400, 290, 800, 50, L"�������", 40);
            PresentText(L"����Ϸ��� ���콺 ���� ��ư Ŭ��", 370, 370, 800, 50, L"�������", 20);

            Sleep(70);

            if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
                // ���� ���� �� ������ �غ��� �ð�
                mspBackground->Draw();
                PresentText(L"3", 500, 320, 800, 50, L"�������", 40);
                mspRenderTarget->EndDraw();
                Sleep(1000);
                mspRenderTarget->BeginDraw();

                mspBackground->Draw();
                PresentText(L"2", 500, 320, 800, 50, L"�������", 40);
                mspRenderTarget->EndDraw();
                Sleep(1000);
                mspRenderTarget->BeginDraw();

                mspBackground->Draw();
                PresentText(L"1", 500, 320, 800, 50, L"�������", 40);
                mspRenderTarget->EndDraw();
                Sleep(1000);
                mspRenderTarget->BeginDraw();

                for (int i = 0; i < 15; i++) {
                    mBugList.push_back(std::make_shared<Bug>(this, L"Images/bug2.png", 3.0f, false));
                }
                mBugGenerated = true;

                // 2�ܰ� ���� �ð� ����
                mGameStartTime = std::chrono::steady_clock::now();
            }
        }
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
    // ���� ���콺 Ŀ�� ��ġ ��������
    POINT pt;
    GetCursorPos(&pt);
    ScreenToClient(mHwnd, &pt);

    if (mBugGenerated) {
        // 2�ܰ谡 ���۵ǰ� 7�ʸ� ����ٸ� ����, �ƴϸ� ����
        for (auto& bug : mBugList) {
            Bug* p = static_cast<Bug*>(bug.get());
            p->IsClicked(pt);
            if (p->mIsDead) {
                mGameFail = true;
                return;
            }
        }

        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - mGameStartTime).count();
        if (elapsed >= 7) {
            mGameSuccess = true;
            return;
        }
    } else {
        // ���� ���콺�� Ŭ���Ǿ����� Ȯ��
        if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
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
}

void GameManager::ResetGame() {
    mGameStart = false;
    mGameFail = false;
    mGameSuccess = false;
    mBugGenerated = false;
    mGameStartTime = std::chrono::steady_clock::now();
    mBugList.clear();

    for (int i = 0; i < 15; i++) {
        mBugList.push_back(std::make_shared<Bug>(this, L"Images/bug1.png", 2.5f, true));
    }
}