# WinWPI_FrameWork
WinAPI 프레임워크




clang-uml 사용법 </br>
-// .clang-uml2 파일의 특정 다이어그램을 class_overview 이걸로 사용 </br>
clang-uml --config .clang-uml class_overview </br>
// .clang-uml2 -> 전체 mmdd로 제작 -g mermaid 로 지정해서 뽑도록 하는 방법들 </br>
clang-uml --config .clang-uml2 -g 2  </br>

YAML 에디터  </br>
https://jsonformatter.org/yaml-validator  </br>


 </br>
  </br>

# PlantUML 방식
> ## 현재 UML Class 
![다이어그램](http://www.plantuml.com/plantuml/proxy?fmt=svg&src=https://raw.githubusercontent.com/magotoolivesource/WinWPI_FrameWork/refs/heads/main/diagrams/class_overview.puml?v=0)

>## 현재 Include
![다이어그램](http://www.plantuml.com/plantuml/proxy?fmt=svg&src=https://raw.githubusercontent.com/magotoolivesource/WinWPI_FrameWork/refs/heads/main/diagrams/include_dependencies.puml?v=0)

>## 현재 Package
![다이어그램](http://www.plantuml.com/plantuml/proxy?fmt=svg&src=https://raw.githubusercontent.com/magotoolivesource/WinWPI_FrameWork/refs/heads/main/diagrams/package_structure.puml?v=0)




# mermaid UML 방식
# 기본 UML Class 
```mermaid
classDiagram
    class C_0016480947060394780245["Vec2"]
    class C_0016480947060394780245 {
        +operator*(float scalar) [const] Vec2
        +operator*(const Vec2 & other) [const] Vec2
        +operator*=(float scalar) Vec2
        +operator+(const Vec2 & other) [const] Vec2
        +operator+=(const Vec2 & other) Vec2
        +operator+=(float scalar) Vec2
        +operator-(const Vec2 & other) [const] Vec2
        +operator-() [const] Vec2
        +operator-=(const Vec2 & other) Vec2
        +operator/(float scalar) [const] Vec2
        +operator/(const Vec2 & other) [const] Vec2
        +operator/=(float scalar) Vec2
        +operator==(const Vec2 & other) [const] bool
        +Abs() [const] Vec2
        +Ceil() [const] Vec2
        +Clamp(const Vec2 & min, const Vec2 & max) [const] Vec2
        +Floor() [const] Vec2
        +Length() [const] float
        +LengthSquared() [const] float
        +Lerp(const Vec2 & other, float t) [const] Vec2
        +Max(const Vec2 & other) [const] Vec2
        +Min(const Vec2 & other) [const] Vec2
        +Normalize() [const] Vec2
        +Perpendicular() [const] Vec2
        +Project(const Vec2 & other) [const] Vec2
        +Reflect(const Vec2 & normal) [const] Vec2
        +Rotate(float angleDegrees) [const] Vec2
        +Round() [const] Vec2
        +x : float
        +y : float
    }
    class C_0008364433741114752039["Rect"]
    class C_0008364433741114752039 {
        +Intersects(const Rect & other) bool
        +height : float
        +pos : Vec2
        +width : float
    }
    class C_0012021292812520796288["Component"]
    class C_0012021292812520796288 {
        +~Component() void
        +Destroy() void
        +Initialize() void
        +Initialize_AddCompoment() void
        +IsEnabled() [const] bool
        +Release() void
        +Render(HDC hdc) void
        +SetEnabled(bool e) void
        +Start() void
        +Update(float dt) void
        +enabled : bool
        +owner : GameObject *
        +transform : Transform *
    }
    class C_0000370005033936394450["Gdiplus::Image"]
    class C_0000370005033936394450 {
    }
    class C_0006111862596534002478["Camera"]
    class C_0006111862596534002478 {
        +Camera() [default] void
        +~Camera() void
        +ApplyToMatrix(Gdiplus::Matrix & matrix) [const] void
        +ApplyToPoint(const Vec2 & worldPos) [const] Vec2
        +Destroy() void
        +Initialize() void
        +Initialize_AddCompoment() void
        +Release() void
        +Render(HDC hdc) void
        +SetLocalPosition(const Vec2 & pos) void
        +SetLocalPosition(int p_x, int p_y) void
        +SetZoom(float z) void
        +Start() void
        +Update(float dt) void
        #zoom : float
    }
    class C_0010250684592540512228["GameObject"]
    class C_0010250684592540512228 {
        +GameObject() void
        +~GameObject() void
        +AddComponent<T,Args...>(Args &&... args) T *
        +AddTag(const std::string & tag) void
        +GetComponent<T>() T *
        +GetName() [const] const std::string &
        +HasTag(const std::string & tag) [const] bool
        #InitCreateTransform() void
        #Initialize() void
        +IsActive() [const] bool
        +RemoveComponent<T,Args...>() _Bool
        +Render(HDC hdc) void
        +SetActive(bool a) void
        +SetName(const std::string & n) void
        +Start() void
        +Update(float dt) void
        #active : bool
        #components : std::unordered_map&lt;std::type_index,std::unique_ptr&lt;Component&gt;&gt;
        #name : std::string
        #tags : std::unordered_set&lt;std::string&gt;
        +transform : Transform *
    }
    class C_0013979161094353040123["Transform"]
    class C_0013979161094353040123 {
        #Transform() [default] void
        +GetDepth() [const] float
        +GetWorldDepth() float
        +GetWorldMatrix() Gdiplus::Matrix &
        +GetWorldPosition() Vec2 &
        +GetWorldRotation() float
        +GetWorldScale() Vec2 &
        #MarkDirty() void
        +SetDepth(float d) void
        +SetLocalRotation(float angleDegrees) void
        +SetLocalScale(Vec2 scale) void
        +SetLocalScale(float p_scalex, float p_scaley) void
        +SetParent(Transform * newParent) void
        +SetWorldPosition(Vec2 & p_wpos) void
        +SetWorldPosition(float p_x, float p_y) void
        +SetWorldRotation(float worldAngle) void
        +SetWorldScale(Vec2 & worldScale) void
        +UpdateAllPositions() void
        #UpdateIfDirty() void
        +getHeight() [const] float
        +getLocalPosition() [const] Vec2
        +getRotation() [const] float
        +getWidth() [const] float
        +setHeight(float h) void
        +setLocalPosition(const Vec2 & pos) void
        +setLocalPosition(float p_x, float p_y) void
        +setWidth(float w) void
        -cachedWorldDepth : float
        -cachedWorldMatrix : Gdiplus::Matrix
        -cachedWorldPosition : Vec2
        -cachedWorldRotation : float
        -cachedWorldScale : Vec2
        -children : std::vector&lt;Transform *&gt;
        +height : float
        -isDirty : bool
        +localScale : Vec2
        +localposition : Vec2
        -m_tempcachedmatrix : float[6]
        -parent : Transform *
        +rotation : float
        +width : float
        +zdepth : float
    }
    class C_0014710440344490690589["Collider"]
    class C_0014710440344490690589 {
        +GetWorldRect() Rect
        +Intersects(Collider * other) bool
    }
    class C_0007426470158294810457["ImageComponent"]
    class C_0007426470158294810457 {
        +ImageComponent() void
        +ImageComponent(Gdiplus::Image * p_img, int p_width, int p_height, bool p_useDrawRect) void
        +~ImageComponent() void
        +Draw(HDC hdc) void
        +ImageLoadImage(const std::wstring & path) bool
        +Initialize() void
        +Initialize_AddCompoment() void
        +Render(HDC hdc) void
        +SetDrawRect(int left, int top, int right, int bottom) void
        +SetSize(int width, int height) void
        -drawHeight : int
        -drawRect : RECT
        -drawWidth : int
        -image : Gdiplus::Image *
        -transform : Transform *
        -useDrawRect : bool
    }
    class C_0011044441207668165372["SingletonT&lt;T&gt;"]
    class C_0011044441207668165372 {
        #SingletonT() [default] void
        #SingletonT(const SingletonT&lt;T&gt; &) void
        #~SingletonT() [default] void
        #operator=(const SingletonT&lt;T&gt; &) SingletonT&lt;T&gt; &
        +GetI() T *$
        +Instance() T &$
        -instance : T *
    }
    class C_0011492085545754070408["SingletonT&lt;ImageManager&gt;"]
    class C_0011492085545754070408 {
    }
    class C_0017393003743977498017["ImageManager"]
    class C_0017393003743977498017 {
        +~ImageManager() void
        +Load(const std::wstring & path) Gdiplus::Image *
        -imageMap : std::unordered_map&lt;std::wstring,Gdiplus::Image *&gt;
    }
    class C_0007529291253653007674["InputManager"]
    class C_0007529291253653007674 {
        +GetMousePosition() POINT$
        +Init() void$
        +IsKeyPressed(int key) bool$
        +IsMouseLeftDown() bool$
        +KeyDown(WPARAM key) void$
        +KeyUp(WPARAM key) void$
        +MouseDown(WPARAM btn) void$
        +MouseMove(LPARAM lParam) void$
        +MouseUp(WPARAM btn) void$
        +keys : bool[256]
        +mouseLeftDown : bool
        +mousePosition : POINT
    }
    class C_0012798265388312005056["Material"]
    class C_0012798265388312005056 {
        +Material(Gdiplus::Image * img = nullptr) void
        +SetUV(int left, int top, int width, int height) void
        +image : Gdiplus::Image *
        +uvRect : Gdiplus::Rect
    }
    class C_0012543508281110658058["MyTime"]
    class C_0012543508281110658058 {
    }
    class C_0012676504793219311971["Viewport"]
    class C_0012676504793219311971 {
        +GetViewRect() [const] const Gdiplus::RectF &
        +IsVisible(const Vec2 & screenPos, float width, float height) [const] bool
        +SetViewRect(float x, float y, float width, float height) void
        -viewRect : Gdiplus::RectF
    }
    class C_0013661311382128640226["SingletonT&lt;CameraManager&gt;"]
    class C_0013661311382128640226 {
    }
    class C_0010602408877764505757["CameraManager"]
    class C_0010602408877764505757 {
        +~CameraManager() void
    }
    class C_0005375663403181666835["Scene"]
    class C_0005375663403181666835 {
        +CreateObject(const std::string & name) GameObject *
        +DestroyObject(GameObject * obj) void
        +Release() void
        +Render(HDC p_hdc) void
        +Start() void
        +Update(float dt) void
        +objects : std::vector&lt;std::unique_ptr&lt;GameObject&gt;&gt;
    }
    class C_0000221405838615140689["Button"]
    class C_0000221405838615140689 {
        +Button() [default] void
        +Button(const std::function&lt;void &lpar;Button *&rpar;&gt; & onClick) void
        +~Button() [default] void
        +Render(HDC hdc) void
        +Update(float dt) void
        +isHovered : bool
        +isPressed : bool
        +onClick : std::function&lt;void &lpar;Button *&rpar;&gt;
    }
    class C_0014246112670391732192["InputFieldComponent"]
    class C_0014246112670391732192 {
        +Backspace() void
        +Draw(HDC hdc) void
        +InputChar(wchar_t c) void
        +SetFocus(bool isFocused) void
        +SetOnChange(std::function&lt;void &lpar;const std::wstring &&rpar;&gt; callback) void
        +SetPlaceholder(const std::wstring & hint) void
        +SetTransform(Transform * t) void
        -focused : bool
        -onChange : std::function&lt;void &lpar;const std::wstring &&rpar;&gt;
        -placeholder : std::wstring
        -text : std::wstring
        -transform : Transform *
    }
    class C_0006468325429240224623["RichTextComponent"]
    class C_0006468325429240224623 {
        +RichTextComponent() [default] void
        +~RichTextComponent() void
        +Draw(HDC hdc) void
        +Initialize() void
        +Initialize_AddCompoment() void
        -ParseColor(const std::wstring & hex) Gdiplus::Color
        -ParseRichText() void
        +Release() void
        +Render(HDC hdc) void
        +SetAlignment(Gdiplus::StringAlignment horizontal, Gdiplus::StringAlignment vertical) void
        +SetFont(const std::wstring & family, float size) void
        +SetOutline(bool use, BYTE a, BYTE r, BYTE g, BYTE b) void
        +SetSize(int w, int h) void
        +SetStyle(bool bold, bool italic, bool underline) void
        +SetText(const std::wstring & text) void
        +SetTransform(Transform * t) void
        +Update(float dt) void
        -family : Gdiplus::FontFamily *
        -font : Gdiplus::Font *
        -fontFamily : std::wstring
        -fontSize : float
        -fontStyle : int
        -hAlign : Gdiplus::StringAlignment
        -height : int
        -outlineColor : Gdiplus::Color
        -parsedSegments : std::vector&lt;TextSegment&gt;
        -rawText : std::wstring
        -transform : Transform *
        -useOutline : bool
        -vAlign : Gdiplus::StringAlignment
        -width : int
    }
    class C_0010200758658661813511["RichTextComponent::TextSegment"]
    class C_0010200758658661813511 {
        +color : Gdiplus::Color
        +text : std::wstring
    }
    class C_0012700147483782556045["TextComponent"]
    class C_0012700147483782556045 {
        +TextComponent() void
        +~TextComponent() void
        +EnableBlinking(bool enabled) void
        +Initialize() void
        +Initialize_AddCompoment() void
        +Release() void
        +Render(HDC hdc) void
        +SetAlignment(Gdiplus::StringAlignment horizontal, Gdiplus::StringAlignment vertical) void
        +SetFont(const std::wstring & family, float size) void
        +SetFontColor(BYTE a, BYTE r, BYTE g, BYTE b) void
        +SetOutlineColor(BYTE a, BYTE r, BYTE g, BYTE b) void
        +SetSize(int w, int h) void
        +SetText(const std::wstring & str) void
        +SetTransform(Transform * t) void
        +Update(float dt) void
        -elapsedTime : double
        -family : Gdiplus::FontFamily *
        -font : Gdiplus::Font *
        -fontColor : Gdiplus::Color
        -fontFamily : std::wstring
        -fontSize : float
        -hAlign : Gdiplus::StringAlignment
        -height : int
        -outlineColor : Gdiplus::Color
        -text : std::wstring
        -transform : Transform *
        -useBlink : bool
        -useOutline : bool
        -vAlign : Gdiplus::StringAlignment
        -width : int
    }
    class C_0005365101254653150826["BaseGame"]
    class C_0005365101254653150826 {
        +BaseGame() void
        +~BaseGame() void
        +CleanUp() void
        +Init() void
        #InitGDIPlus() void
        +Release() void
        #ReleaseGDIPlus() void
        +Render(HDC p_hdc, RECT & p_clientRect) void
        +Run() void
        +Test_InitScene() void
        +Update() void
        +UpdateInput(UINT message, WPARAM wParam, LPARAM lParam) void
        +UpdateTimer() void
        #gdiplusToken : ULONG_PTR
        +isRunning : bool
        #m_BGColor : COLORREF
        +m_CurrentScene : Scene *
        #m_pInputManager : InputManager *
        #m_pTimerManager : TimerManager *
    }
    class C_0006104864286880803545["TimerManager"]
    class C_0006104864286880803545 {
        +TimerManager() void
        +GetDeltaTime() [const] double
        +GetTotalTime() [const] double
        +Update() void
        -deltaTime : double
        -frequency : LARGE_INTEGER
        -lastTime : LARGE_INTEGER
        -startTime : LARGE_INTEGER
        -totalTime : double
    }
    C_0008364433741114752039 o-- C_0016480947060394780245 : +pos
    C_0012021292812520796288 --> C_0010250684592540512228 : +owner
    C_0012021292812520796288 --> C_0013979161094353040123 : +transform
    C_0006111862596534002478 ..> C_0016480947060394780245 : 
    C_0012021292812520796288 <|-- C_0006111862596534002478 : 
    C_0010250684592540512228 o-- C_0012021292812520796288 : #components
    C_0010250684592540512228 --> C_0013979161094353040123 : +transform
    C_0013979161094353040123 o-- C_0016480947060394780245 : +localposition
    C_0013979161094353040123 o-- C_0016480947060394780245 : +localScale
    C_0013979161094353040123 --> C_0013979161094353040123 : -parent
    C_0013979161094353040123 --> C_0013979161094353040123 : -children
    C_0013979161094353040123 o-- C_0016480947060394780245 : -cachedWorldPosition
    C_0013979161094353040123 o-- C_0016480947060394780245 : -cachedWorldScale
    C_0013979161094353040123 <.. C_0010250684592540512228 : +[friend]
    C_0012021292812520796288 <|-- C_0013979161094353040123 : 
    C_0014710440344490690589 ..> C_0008364433741114752039 : 
    C_0012021292812520796288 <|-- C_0014710440344490690589 : 
    C_0007426470158294810457 --> C_0013979161094353040123 : -transform
    C_0012021292812520796288 <|-- C_0007426470158294810457 : 
    C_0011492085545754070408 ..> C_0017393003743977498017 : 
    C_0011492085545754070408 ..|> C_0011044441207668165372 : 
    C_0017393003743977498017 --> C_0000370005033936394450 : -imageMap
    C_0011492085545754070408 <|-- C_0017393003743977498017 : 
    C_0012676504793219311971 ..> C_0016480947060394780245 : 
    C_0013661311382128640226 ..> C_0010602408877764505757 : 
    C_0013661311382128640226 ..|> C_0011044441207668165372 : 
    C_0013661311382128640226 <|-- C_0010602408877764505757 : 
    C_0005375663403181666835 o-- C_0010250684592540512228 : +objects
    C_0000221405838615140689 --> C_0000221405838615140689 : +onClick
    C_0012021292812520796288 <|-- C_0000221405838615140689 : 
    C_0014246112670391732192 --> C_0013979161094353040123 : -transform
    C_0006468325429240224623 --> C_0013979161094353040123 : -transform
    C_0012021292812520796288 <|-- C_0006468325429240224623 : 
    C_0006468325429240224623 ()-- C_0010200758658661813511 : 
    C_0012700147483782556045 --> C_0013979161094353040123 : -transform
    C_0012021292812520796288 <|-- C_0012700147483782556045 : 
    C_0005365101254653150826 --> C_0005375663403181666835 : +m_CurrentScene
    C_0005365101254653150826 --> C_0006104864286880803545 : #m_pTimerManager
    C_0005365101254653150826 --> C_0007529291253653007674 : #m_pInputManager

%% Generated with clang-uml, version 0.6.2
%% LLVM version clang version 20.1.7 (https://github.com/llvm/llvm-project.git 6146a88f60492b520a36f8f8f3231e15f3cc6082)
```


# 기본 UML Include
```mermaid
flowchart
    subgraph C_0005987407375906095790[FrameWork_2DAPI]
    subgraph C_0009079422266128183806[Compoment]
    C_0000542131175853268262[Camera.cpp]
    C_0002233206018820285275[Camera.h]
    C_0012006922542282881227[Component.h]
    C_0005719404820632631720[Transform.h]
    C_0002189911845737725449[Collider.cpp]
    C_0006009038626991597040[Collider.h]
    C_0013968366448891272726[Component.cpp]
    C_0008580941411690267369[ImageComponent.cpp]
    C_0013959013035525480656[ImageComponent.h]
    C_0005358857604292455793[Transform.cpp]
    end
    subgraph C_0011265527563778237801[Core]
    C_0011403750343696262879[Rect.h]
    C_0009241864309396972362[Vector.h]
    C_0004042546161801080444[GameObject.h]
    C_0005103878031763531942[SingletonT.h]
    C_0004593396786342407477[GameObject.cpp]
    C_0006715012369522388591[InputManager.cpp]
    C_0008490262948685167826[InputManager.h]
    C_0013530310977228940505[Material.cpp]
    C_0015703106579219591232[Material.h]
    C_0013268641514221186231[MyTime.cpp]
    C_0012761271053487719786[MyTime.h]
    C_0015204365231920836278[Viewport.cpp]
    C_0002856403762398272363[Viewport.h]
    C_0015350113566451572951[DefineHeader.h]
    C_0001728310481282724229[TimerManager.h]
    end
    subgraph C_0007357198411899871477[Manager]
    C_0005264507623711508609[ImageManager.h]
    C_0011795145165219994652[CameraManager.cpp]
    C_0000975214415346362189[CameraManager.h]
    C_0016749223998467583664[ImageManager.cpp]
    end
    subgraph C_0006349955410683132918[Scene]
    C_0013424696354905452145[Scene.cpp]
    C_0014113046361969808040[Scene.h]
    end
    subgraph C_0014903899459858743212[UICompoment]
    C_0015108746298517492543[Button.cpp]
    C_0014675849799759254562[Button.h]
    C_0005596289685074760462[RichTextAndInputFieldComponent.cpp]
    C_0016995368971307459379[RichTextAndInputFieldComponent.h]
    C_0001805303676707499043[RichTextComponent.cpp]
    C_0017931631642561992518[RichTextComponent.h]
    C_0010503176285208422829[TextComponent.cpp]
    C_0011619476113675404708[TextComponent.h]
    end
    C_0007522631347308452242[BaseGame.cpp]
    C_0001748520534377182215[BaseGame.h]
    C_0017691936045283160068[FrameWork_2DAPI.cpp]
    C_0017495671912026114932[framework.h]
    C_0012947420176480543526[targetver.h]
    C_0009079868531278845189[FrameWork_2DAPI.h]
    C_0006231518002176939064[Resource.h]
    end
    C_0000542131175853268262 --> C_0002233206018820285275
    C_0000542131175853268262 --> C_0004042546161801080444
    C_0000542131175853268262 --> C_0005719404820632631720
    C_0002233206018820285275 --> C_0011403750343696262879
    C_0002233206018820285275 --> C_0012006922542282881227
    C_0005719404820632631720 --> C_0012006922542282881227
    C_0005719404820632631720 --> C_0009241864309396972362
    C_0002189911845737725449 --> C_0006009038626991597040
    C_0002189911845737725449 --> C_0004042546161801080444
    C_0002189911845737725449 --> C_0012006922542282881227
    C_0002189911845737725449 --> C_0005719404820632631720
    C_0006009038626991597040 --> C_0011403750343696262879
    C_0006009038626991597040 --> C_0012006922542282881227
    C_0006009038626991597040 --> C_0005719404820632631720
    C_0013968366448891272726 --> C_0012006922542282881227
    C_0013968366448891272726 --> C_0004042546161801080444
    C_0013968366448891272726 --> C_0005719404820632631720
    C_0008580941411690267369 --> C_0013959013035525480656
    C_0008580941411690267369 --> C_0004042546161801080444
    C_0008580941411690267369 --> C_0005719404820632631720
    C_0008580941411690267369 --> C_0005264507623711508609
    C_0013959013035525480656 --> C_0012006922542282881227
    C_0005358857604292455793 --> C_0005719404820632631720
    C_0011403750343696262879 --> C_0009241864309396972362
    C_0004042546161801080444 --> C_0012006922542282881227
    C_0004593396786342407477 --> C_0004042546161801080444
    C_0004593396786342407477 --> C_0005719404820632631720
    C_0006715012369522388591 --> C_0008490262948685167826
    C_0013530310977228940505 --> C_0015703106579219591232
    C_0013268641514221186231 --> C_0012761271053487719786
    C_0015204365231920836278 --> C_0002856403762398272363
    C_0002856403762398272363 --> C_0009241864309396972362
    C_0002856403762398272363 --> C_0011403750343696262879
    C_0002856403762398272363 --> C_0012006922542282881227
    C_0005264507623711508609 --> C_0005103878031763531942
    C_0011795145165219994652 --> C_0000975214415346362189
    C_0000975214415346362189 --> C_0005103878031763531942
    C_0016749223998467583664 --> C_0005264507623711508609
    C_0013424696354905452145 --> C_0014113046361969808040
    C_0014113046361969808040 --> C_0004042546161801080444
    C_0015108746298517492543 --> C_0004042546161801080444
    C_0015108746298517492543 --> C_0005719404820632631720
    C_0015108746298517492543 --> C_0008490262948685167826
    C_0015108746298517492543 --> C_0014675849799759254562
    C_0014675849799759254562 --> C_0012006922542282881227
    C_0014675849799759254562 --> C_0005719404820632631720
    C_0005596289685074760462 --> C_0016995368971307459379
    C_0005596289685074760462 --> C_0004042546161801080444
    C_0005596289685074760462 --> C_0005719404820632631720
    C_0005596289685074760462 --> C_0015350113566451572951
    C_0016995368971307459379 --> C_0012006922542282881227
    C_0001805303676707499043 --> C_0017931631642561992518
    C_0001805303676707499043 --> C_0004042546161801080444
    C_0001805303676707499043 --> C_0005719404820632631720
    C_0001805303676707499043 --> C_0015350113566451572951
    C_0017931631642561992518 --> C_0012006922542282881227
    C_0010503176285208422829 --> C_0011619476113675404708
    C_0010503176285208422829 --> C_0004042546161801080444
    C_0010503176285208422829 --> C_0005719404820632631720
    C_0010503176285208422829 --> C_0015350113566451572951
    C_0011619476113675404708 --> C_0012006922542282881227
    C_0007522631347308452242 --> C_0001748520534377182215
    C_0007522631347308452242 --> C_0001728310481282724229
    C_0007522631347308452242 --> C_0008490262948685167826
    C_0007522631347308452242 --> C_0012006922542282881227
    C_0007522631347308452242 --> C_0005719404820632631720
    C_0007522631347308452242 --> C_0013959013035525480656
    C_0007522631347308452242 --> C_0014675849799759254562
    C_0007522631347308452242 --> C_0011619476113675404708
    C_0007522631347308452242 --> C_0017931631642561992518
    C_0001748520534377182215 --> C_0014113046361969808040
    C_0017691936045283160068 --> C_0017495671912026114932
    C_0017691936045283160068 --> C_0009079868531278845189
    C_0017691936045283160068 --> C_0001748520534377182215
    C_0017495671912026114932 --> C_0012947420176480543526
    C_0009079868531278845189 --> C_0006231518002176939064

%% Generated with clang-uml, version 0.6.2
%% LLVM version clang version 20.1.7 (https://github.com/llvm/llvm-project.git 6146a88f60492b520a36f8f8f3231e15f3cc6082)


```


UML 작업하기

