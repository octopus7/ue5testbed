# 동물 캐릭터 블루프린트 생성 및 AnimSwitch 컴포넌트 연결 가이드

본 문서는 AnimSwitcher 플러그인의 동물 캐릭터용 애니 전환 컴포넌트를 적용하는 절차를 안내합니다. 현재 버전은 실제 동작을 수행합니다(속도 기반 Walk/Run 전환, 앉기 전환, 액션/리액션 원샷, 수영/지상 루프 등). 애셋은 외부에서 지정합니다.

## 사전 준비
- 플러그인 확인: `Edit → Plugins → Project → Anim Switcher`가 Enabled 인지 확인하고, 필요시 에디터 재시작/리컴파일.
- 애니메이션 자산: 아래와 같은 이름 규칙을 가진 애니를 준비합니다. 내부 키에서는 `Bird_` 접두사가 제거됩니다.
  - Bird_Attack01, Bird_Death, Bird_Eating, Bird_Hit_Reaction
  - Bird_Idle, Bird_Idle_sitting, Bird_Idle_Variation
  - Bird_Run_Forward, Bird_Walk_Forward, Bird_Walk_Backwards
  - Bird_Turn_90_L, Bird_Turn_90_R
  - Bird_swim_idle, Bird_swim_forward, Bird_swim_eating

## 1) 동물 캐릭터 블루프린트 생성
1. 콘텐츠 브라우저에서 `Add → Blueprint Class` 선택
2. 검색창에 `AnimSwitchCharacter` 입력 → `AAnimSwitchCharacter` 선택
3. 이름 예: `BP_Bird`
4. `BP_Bird`를 열고 `Mesh (SkeletalMeshComponent)`에 사용 중인 동물 스켈레탈 메시를 지정
   - 기본값으로 컴포넌트가 메시를 `AnimationSingleNode` 모드로 전환하여 직접 재생합니다(`bForceSingleNodeMode = true`).
   - AnimBP를 유지·사용하려면 `bForceSingleNodeMode = false`로 바꾸고, ABP 쪽에서 상태 변수/이벤트를 연동하세요.

참고: `AAnimSwitchCharacter`에는 기본으로 `AnimSwitchComponent`가 포함되어 있습니다. 다른 액터/캐릭터에 붙이고 싶다면 `Add → Anim Switch Component`로 수동 추가해도 됩니다.

## 2) AnimSwitchComponent 기본 설정
블루프린트 디테일 패널에서 `AnimSwitchComponent`를 선택하고 아래 항목을 확인합니다.
- AssetPrefixToStrip: `Bird_` (기본값). 자동 등록(방법 B) 시 접두사를 제거합니다.
- InitialStateName: `Idle` 권장 (원하는 기본 상태명으로 지정)
- AnimSet: 필드별로 애니메이션을 직접 지정(권장)
- Animations 맵: 키-애셋 매핑을 직접 지정(보조용)
- Run/Walk 임계값: `RunSpeedThreshold(기본 300)`, `WalkSpeedThreshold(기본 10)`

## 3) 애니메이션 등록 방법(둘 중 택1 또는 혼용)
### 방법 A: AnimSet 필드에 직접 지정(권장)
- `AnimSet` 섹션의 각 필드에 해당 애셋을 할당하세요.
- 매핑(예: 제공된 Bird_* 애셋):
  - Bird_Idle → AnimSet.Idle
  - Bird_Idle_sitting → AnimSet.IdleSitting
  - Bird_Idle_Variation → AnimSet.IdleVariation
  - Bird_Walk_Forward → AnimSet.WalkForward
  - Bird_Walk_Backwards → AnimSet.WalkBackwards
  - Bird_Run_Forward → AnimSet.RunForward
  - Bird_Turn_90_L → AnimSet.Turn90L
  - Bird_Turn_90_R → AnimSet.Turn90R
  - Bird_swim_idle → AnimSet.SwimIdle
  - Bird_swim_forward → AnimSet.SwimForward
  - Bird_swim_eating → AnimSet.SwimEating
  - Bird_Attack01 → AnimSet.Attack01
  - Bird_Eating → AnimSet.Eating
  - Bird_Hit_Reaction → AnimSet.HitReaction
  - Bird_Death → AnimSet.Death
  - Bird_Stand_to_Sit → AnimSet.StandToSit
  - Bird_Sit_to_Stand → AnimSet.SitToStand

### 방법 B: 자동 키 생성 유틸 사용(보조)
컴포넌트가 제공하는 등록 함수로 애셋 이름에서 키를 자동 생성합니다.
- 블루프린트 그래프에서 다음 중 하나를 사용:
  - `AddAnimationAsset(Asset, bOverrideExisting)`
  - `AddAnimationAssets(AssetsArray, bOverrideExisting)`
- 사용 예:
  1) Construction Script 또는 BeginPlay에서 `Make Array`로 애니 자산 배열을 만들고 → `AddAnimationAssets`에 연결
  2) 기본 접두사 `Bird_`는 `AssetPrefixToStrip`으로 자동 제거됨
  3) 키 생성 규칙: 언더스코어 기준 토큰을 TitleCase로 합쳐 생성 (예: `Bird_Walk_Forward` → `WalkForward`, `Bird_Turn_90_L` → `Turn90L`)

## 4) 런타임 동작과 제어
- 기본 루프(자동):
  - 수영 여부(`SetSwimming(true/false)`)에 따라 SwimIdle/SwimForward 또는 Idle/WalkForward/RunForward로 루프 전환
  - 속도 임계값으로 Walk/Run 전환 (소유 액터의 `GetVelocity()` 기준, 수평 속도 사용)
  - 앉은 상태면 IdleSitting 루프 유지
- 액션/리액션(원샷 재생 후 루프 복귀):
  - `RequestAttack()`, `RequestEat()`(수영 중이면 SwimEating 우선), `RequestHit()`
  - `RequestTurnLeft90()`, `RequestTurnRight90()`
  - `RequestDie()`는 Death 재생 후 기본 루프 복귀 없이 정지
- 앉기 전환:
  - `RequestSit()` → `StandToSit` 원샷 재생 후 IdleSitting 루프
  - `RequestStand()` → `SitToStand` 원샷 재생 후 Idle/Walk/Run 루프

## 5) 레벨 배치
- `BP_Bird`를 레벨에 드래그 앤 드롭으로 배치하면 준비 완료입니다.

## 6) UMG 버튼으로 애니/상태 전환 제어
아래 절차로 버튼 UI를 만들어 각 애니/상태 전환을 호출할 수 있습니다.

1) 위젯 블루프린트 생성
- `Add → User Interface → Widget Blueprint` 선택 → 부모 클래스에서 `AnimSwitcherControlWidget` 선택
- 이름 예: `WBP_AnimControls`
- 위젯에 버튼을 배치하고, 아래 변수명을 정확히 설정(Variable 체크)하면 C++에서 자동으로 OnClicked가 바인딩됩니다.
  - `AttackButton`, `EatButton`, `HitButton`, `TurnLeftButton`, `TurnRightButton`, `SitToggleButton`, `SwimToggleButton`, `DieButton`
  - 버튼 텍스트는 자유롭게 설정
- 필요 시 그래프에서 `SetTargetActor(Target)` 또는 `SetTargetComponent(Comp)` 호출로 대상 지정 가능

2) 레벨에 UI 드라이버 액터 배치(자동 생성 옵션)
- `Add Actor` → `AnimSwitcherUIDriver`를 레벨에 배치
- 디테일 패널
  - `UIClass`에 `WBP_AnimControls` 지정
  - `TargetActor`에 제어할 동물 캐릭터(BP_Bird 등) 지정
  - `ZOrder`/`bShowMouseCursor` 필요 시 조정

3) 플레이 시 동작
- 각 버튼 클릭 시 다음 함수가 호출됩니다.
  - AttackButton → `RequestAttack()`
  - EatButton → `RequestEat()`
  - HitButton → `RequestHit()`
  - TurnLeftButton → `RequestTurnLeft90()`
  - TurnRightButton → `RequestTurnRight90()`
  - SitToggleButton → `SetSitting(!IsSitting())`
  - SwimToggleButton → `SetSwimming(!IsSwimming())`
  - DieButton → `RequestDie()`

## 키 매핑 표 (애셋명 → 내부 키)
- Idle 계열
  - Bird_Idle → Idle
  - Bird_Idle_sitting → IdleSitting
  - Bird_Idle_Variation → IdleVariation
- 지상 이동
  - Bird_Walk_Forward → WalkForward
  - Bird_Walk_Backwards → WalkBackwards
  - Bird_Run_Forward → RunForward
  - Bird_Turn_90_L → Turn90L
  - Bird_Turn_90_R → Turn90R
- 수영 이동
  - Bird_swim_idle → SwimIdle
  - Bird_swim_forward → SwimForward
  - Bird_swim_eating → SwimEating
- 액션/리액션
  - Bird_Attack01 → Attack01
  - Bird_Eating → Eating
  - Bird_Hit_Reaction → HitReaction
  - Bird_Death → Death
- 자세 전환
  - Bird_Stand_to_Sit → StandToSit
  - Bird_Sit_to_Stand → SitToStand

## 참고 및 다음 단계
- 현재 구현은 애니 시퀀스를 직접 재생합니다. 다음을 원하시면 확장 가능합니다.
  - AnimBP 기반 구조로 이관(상태머신/BlendSpace/슬롯): 컴포넌트 이벤트 → ABP 변수/Notify 연동
  - 전환 정책 강화: 인터럽트 우선순위(예: Hit > Attack > Locomotion), 블렌드 타임 세분화
  - DataAsset 프리셋: 종(새/물고기/포유류 등)별 애니 세트 관리 및 공유
  - GameplayTag/BT/환경 트리거 연동

원하시는 방향을 알려주시면 해당 구조를 추가해 드립니다.
