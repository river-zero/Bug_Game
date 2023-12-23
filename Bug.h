#pragma once

#include <random>
#include "Actor.h"

class Bug : public Actor {
private:
    // ������ �̵��ϴ� �������� ���� �������� ����
    const D2D_POINT_2F UPVECTOR{ 0.0f, -1.0f };

    // ������ ȸ�� ������ �̵� �Ÿ�
    float mRotation;
    float mSteps;

    // ��ǥ ȸ�� ���� �� �̵� �ӵ�
    float mTargetRotation;
    float mMoveSpeed;

public:
    // ������ ���� ���� �÷���
    bool mIsDead;

public:
    Bug(D2DFramework* pFramework);

    virtual void Draw() override;

    // �־��� ȭ�� ��ǥ���� ������ Ŭ���Ǿ����� Ȯ��
    bool IsClicked(POINT& pt);
};