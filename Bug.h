#pragma once

#include <random>
#include "Actor.h"

class Bug : public Actor {
private:
    // 벌레가 이동하는 방향으로 현재 위쪽으로 지정
    const D2D_POINT_2F UPVECTOR{ 0.0f, -1.0f };

    // 벌레의 회전 각도와 이동 거리
    float mRotation;
    float mSteps;

    // 목표 회전 각도 및 이동 속도
    float mTargetRotation;
    float mMoveSpeed;

public:
    // 벌레의 생존 여부 플래그
    bool mIsDead;

public:
    Bug(D2DFramework* pFramework);

    virtual void Draw() override;

    // 주어진 화면 좌표에서 벌레가 클릭되었는지 확인
    bool IsClicked(POINT& pt);
};