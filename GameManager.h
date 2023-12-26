#pragma once

#include <chrono>
#include <memory>
#include <list>
#include "D2DFramework.h"
#include "Actor.h"

class GameManager : public D2DFramework {
private:
	std::shared_ptr<Actor> mspBackground;
	std::list<std::shared_ptr<Actor>> mBugList;
    // 동적 관리를 위해 list로 하고
    // 원소들은 다양한 곳에서 사용하므로 shared_ptr로 지정

    bool mGameStart;
    bool mBugGenerated;
    bool mGameSuccess;
    bool mGameFail;
    std::chrono::steady_clock::time_point mGameStartTime;

public:
    virtual HRESULT Initialize(HINSTANCE hInstance, LPCWSTR title = L"Bug Game", UINT width = 1024, UINT height = 768) override;
    virtual void Render() override;
    virtual void Release() override;

    void CheckBugs();
    void ResetGame();
};