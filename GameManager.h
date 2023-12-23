#pragma once

#include <memory>
#include <list>
#include "D2DFramework.h"
#include "Actor.h"

class GameManager : public D2DFramework {
	std::shared_ptr<Actor> mspBackground;
	std::list<std::shared_ptr<Actor>> mBugList;
    // ���� ������ ���� list�� �ϰ�
    // ���ҵ��� �پ��� ������ ����ϹǷ� shared_ptr�� ����

public:
    virtual HRESULT Initialize(HINSTANCE hInstance, LPCWSTR title = L"Bug Game", UINT width = 1024, UINT height = 768) override;
    virtual void Render() override;
    virtual void Release() override;

    void CheckBugs();
};