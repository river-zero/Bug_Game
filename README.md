## 벌레 게임
게임은 총 두 가지 단계로 구성되어 있습니다. 1단계는 사방으로 돌아다니는 벌레들을 왼쪽 마우스로 클릭해 모두 제거하면 클리어됩니다. 2단계는 마우스에 부착된 사탕을 다가오는 벌레들로부터 1분 동안 지켜내면 클리어됩니다. 해당 프로젝트는 Direct2D를 이용해 구현하였습니다.

## 플레이 영상

## 클래스 설계도
![클래스 설계도](out/Memory_Game/Memory_Game.png)

## 주요 기능
- 벌레 랜덤 생성 및 움직임 구현
  - mt19937 난수 생성기와 std::uniform_real_distribution를 사용해 균등히 랜덤하게 벌레 이미지를 화면에 생성합니다.
  - 벌레가 클라이언트 영역을 벗어나지 않도록 std::clamp를 사용해 범위를 제한하였습니다.

## 시행착오
> 초기에는 벌레의 움직임을 구현하기 위해서 열거형으로 8가지 방향을 설정해 움직이도록 하였습니다. 그러나 이미지가 방향이 틀어지고 움직이는 것이 아닌 벌레가 고정된 그 상태 그대로 움직이게 되어 원하는 방식대로 구현되지 않았습니다. 따라서 해당 움직임을 구현하기 위해서 벡터와 행렬을 적용하는 방식을 배우게 되었고 원하는 방향을 설정할 UPVECTOR, 변환을 나타내는 행렬인 D2D1::Matrix3x2F, 그리고 행렬 곱셈을 사용해 원하는 움직임을 구현하였습니다. 해당 방식을 사용함으로써 물체의 움직임과 변환을 더 자연스럽게 다룰 수 있게 되었고, GPU에서 복잡한 연산을 처리해 매우 빠른 처리가 가능해졌습니다. 또한, 이미지 깨짐 현상도 자연스럽게 보간할 수 있게 되어 성능과 시각적인 품질 향상에 도움이 되었습니다.

> 위 작업을 거쳤지만 여전히 움직임이 부드럽지 못함을 느껴 조금 더 자연스러운 벌레의 움직임을 구현하고자 하였습니다. 따라서 목표 회전 각도를 멤버 변수로 추가하고, 해당 목표 회전 각도가 될 때까지 조금식 이동 및 회전을 하도록 Bug::Draw 함수를 코드를 수정하였습니다. 실행 결과 이미지들이 부드럽게 회전하면서 이동을 하지만 위쪽으로 쏠려 이동하는 문제가 발생하였습니다. 해당 문제를 해결하기 위해서 벌레 이미지들이 생성할 때 시작부터 서로 다른 방향을 가지도록 mRotation 초기화 코드를 수정하였고, 원하는 결과물을 얻게 되었습니다.

## 리소스 목록
|      이름      |   설명    |
| :------------: | :-------: |
| background.jpg |   배경    |
|    bug1.png    | 빨간 벌레 |
|    bug2.png    | 노란 벌레 |
|   candy.png    |   사탕    |

## 출처
- https://www.freepik.com/free-vector/hand-drawn-colorful-bug-collection_4176860.htm#query=bug&position=5&from_view=search&track=sph
- https://www.freepik.com/free-photo/design-space-paper-textured-background_3220799.htm#from_view=detail_author
- https://www.freepik.com/free-vector/set-delicious-candies-flat-style_2286190.htm#query=candy&position=8&from_view=search&track=sph&uuid=b1683c40-7980-4602-ace0-6b92f3fe264b