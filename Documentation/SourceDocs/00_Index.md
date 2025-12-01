# 소스 코드 문서화 프로젝트 (S3)

## 개요 (Overview)

이 디렉토리(`Documentation/SourceDocs`)는 "S3" 단계에서 소스 코드 분석을 통해 생성된 문서를 저장하는 공간입니다.

추상적인 개념과 실제 코드 구현 사이의 간극을 줄여, 학생이나 미래의 개발자를 위한 포괄적인 가이드를 만드는 것을 목표로 합니다.

## 사용 가이드 (Usage Guide)

새로운 문서를 작성할 때는 `Template.md`에 정의된 구조를 따라주세요.

### 작업 흐름 (Workflow)
1.  **소스 파일 또는 모듈 선택**: 분석할 파일(예: `Scene.cpp`)이나 논리적 모듈(예: `Input System`)을 선택합니다.
2.  **분석**: 코드의 의존성과 아키텍처 내 역할을 이해합니다.
3.  **문서화**: `Template.md`의 내용을 바탕으로 새로운 `.md` 파일을 생성합니다 (예: 번호 체계에 따라 `11_SceneManager.md`).
4.  **연결**: 이 인덱스 파일이나 메인 `README.md`를 업데이트하여 새 문서를 연결합니다.

## 문서 목차 (Documentation Index)

*(작업이 진행됨에 따라 이 목록이 업데이트됩니다)*

*   [문서 작성 템플릿 (Template)](Template.md)
*   [11. 윈도우 생성 및 메인 루프 (Window Creation & Main Loop)](11_WindowAndMainLoop.md)
*   [12. 게임 매니저와 생명주기 (Game Manager & Lifecycle)](12_GameManager_BaseGame.md)
*   [13. 핵심 아키텍처: GameObject와 Component (Core Architecture)](13_Core_GameObject_Component.md)
*   [14. 씬 시스템 (Scene System)](14_Scene_System.md)
*   [15. 핵심 매니저: 입력과 시간 (Core Managers: Input & Time)](15_Core_Managers_Input_Timer.md)
*   [16. 물리와 충돌 시스템 (Physics & Collision System)](16_Physics_Collision_System.md)
*   [17. 렌더링과 카메라 시스템 (Rendering & Camera System)](17_Rendering_Camera_System.md)
*   [18. UI 시스템 (UI System)](18_UI_System.md)
*   [19. 애니메이션 시스템 (Animation System)](19_Animation_System.md)
*   [20. 리소스 및 유틸리티 매니저 (Resource & Utility Managers)](20_Resource_Utility_Managers.md)
*   [21. 유틸리티와 트랜스폼 시스템 (Utilities & Transform System)](21_Utilities_Transform_System.md)
*   [22. 튜토리얼: 플레이어 만들기 (Tutorial: Making a Player)](22_Tutorial_Making_Player.md)
*   [23. 고급 UI 시스템: RichText와 입력창 (Advanced UI: RichText & InputField)](23_Advanced_UI_RichText_Input.md)

## 다음 단계 / 할 일 목록 (Next Steps / To-Do List)

작업의 연속성을 보장하고 중단을 방지하기 위한 로드맵입니다:

1.  [x] **타겟 선정**: 다음 문서화 대상 모듈 식별 (추천: `Scene.cpp` 또는 `Application.cpp`).
2.  [x] **콘텐츠 생성**: AI를 활용하여 소스 코드를 분석하고 문서 작성 (`11_WindowAndMainLoop.md` 완료).
3.  [x] **콘텐츠 생성**: `BaseGame` 클래스 및 게임 루프 분석 문서 작성 (`12_GameManager_BaseGame.md` 완료).
4.  [x] **콘텐츠 생성**: `Core` 아키텍처(GameObject, Component) 및 `Scene` 시스템 문서화 완료.
5.  [x] **콘텐츠 생성**: 주요 매니저 클래스 (`TimerManager`, `InputManager`) 문서화 완료.
6.  [x] **콘텐츠 생성**: 물리 및 충돌 시스템 (`CollisionManager`, `PhysicsManager`) 문서화 완료.
7.  [x] **콘텐츠 생성**: 카메라 및 렌더링 시스템 (`CameraManager`, `ImageComponent`) 문서화 완료.
8.  [x] **콘텐츠 생성**: UI 시스템 (`UICompoment` 폴더) 문서화 완료.
9.  [x] **콘텐츠 생성**: 애니메이션 및 리소스/유틸리티 매니저 문서화 완료.
10. [x] **콘텐츠 생성**: 유틸리티/트랜스폼 시스템 및 튜토리얼(플레이어 만들기) 문서화 완료.
11. [x] **콘텐츠 생성**: 고급 UI 시스템(RichText, InputField) 문서화 완료.
12. [ ] **완료**: S3 문서화 단계 최종 마무리.
