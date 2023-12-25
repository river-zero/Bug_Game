#include "GameManager.h"
#include "Bug.h"

HRESULT GameManager::Initialize(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height) {
    // 상속받은 D2DFramework 초기화
    HRESULT hr = D2DFramework::Initialize(hInstance, title, width, height);
    ThrowIfFailed(hr, "D2DFrmaework::Initialize Failed");

    // 배경 이미지를 나타내는 Actor 객체 초기화
    mspBackground = std::make_shared<Actor>(this, L"Images/background.jpg", 0.0f, 0.0f, 1.0f);

    // Bug 클래스로부터 파생된 Bug 객체를 40개 생성하여 리스트에 추가
    for (int i = 0; i < 1; i++) {
        mBugList.push_back(std::make_shared<Bug>(this, L"Images/bug1.png", 2.5f, true));
    }

    mGameStart = false;
    mBugGenerated = false;

    return S_OK;
}

void GameManager::Render() {
    mspRenderTarget->BeginDraw();

    // 변환 매트릭스 초기화 및 배경 색상 설정
    // 변환 매트릭스를 단위 행렬로 설정하면 어떤 변환도 적용되지 않은 상태로 초기화됨
    mspRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
    mspRenderTarget->Clear(D2D1::ColorF(0.0f, 0.2f, 0.4f, 1.0f));

    if (!mGameStart) {
        // 배경 그리기
        mspBackground->Draw();

        // 게임 시작 전 문구 출력
        PresentText(L"게임을 시작하세요", 350, 290, 800, 50, L"맑은고딕", 40);
        PresentText(L"마우스 왼쪽 버튼 클릭", 410, 370, 800, 50, L"맑은고딕", 20);

        // 왼쪽 마우스 버튼이 클릭되면 게임 시작
        if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
            mGameStart = true;
        }
    } else {
        mspBackground->Draw();

        CheckBugs();

        // 벌레 이미지 그리기
        for (auto& bug : mBugList) {
            bug->Draw();
        }

        if (mBugList.empty() && !mBugGenerated) {
            PresentText(L"1단계 클리어", 400, 290, 800, 50, L"맑은고딕", 40);
            PresentText(L"계속하려면 마우스 왼쪽 버튼 클릭", 370, 370, 800, 50, L"맑은고딕", 20);

            Sleep(50);

            if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
                for (int i = 0; i < 40; i++) {
                    mBugList.push_back(std::make_shared<Bug>(this, L"Images/bug2.png", 3.0f, false));
                }
                mBugGenerated = true;
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
    // 왼쪽 마우스가 클릭되었는지 확인
    if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
        // 현재 마우스 커서 위치 가져오기
        POINT pt;
        GetCursorPos(&pt);
        ScreenToClient(mHwnd, &pt);

        // 벌레 리스트 순회 및 클릭된 벌레 확인
        auto itr = std::remove_if(mBugList.begin(), mBugList.end(),
            [&](auto& actor) {
                // 클릭된 벌레를 체크하고 죽었다면 리스트에서 제거
                Bug* p = static_cast<Bug*>(actor.get());
                p->IsClicked(pt);
                if (p->mIsDead) {
                    return true;
                } else {
                    return false;
                }
            }
        );

        // 제거된 벌레 요소들을 벡터에서 실제로 삭제
        mBugList.erase(itr, mBugList.end());
    }
}