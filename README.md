## UE4 — Графики в трёхмерном пространстве

<img src="Imgs/ChartsVideo.gif" width="320" height="180" />

### Краткое руководство

Приложение разработано для движка Unreal Engine 4 для отрисовки графиков в трёхмерном пространстве. Графики рисуются с использованием компонента Procedural Mesh и имеют различные настройки для стилизации. Все компоненты и функции могут применяться в Blueprint'ах.

Компонент, используемый для отрисовки графиков — ChartViewComponent. Сначала необходимо вызвать функцию CreateChartViewport для создания вьюпорта графика, затем можно добавлять во вьюпорт линии и изменять дизайн графика.

##### Структура проекта:
- ChartViewComponent
    - ChartLinesComponent
        - ChartLineSection
    - ChartBackgroundComponent

*Примечания*: в проекте также содержится класс GameCard, демонстрирующий работу с графиками.

***

#### ChartViewComponent

ChartViewComponent — это компонент, объединяющий в себе фон графика и линии графика. Он должен преобразовывать входные координаты линии под размер окна графика.

ChartViewComponent хранит координаты линий в массиве структур(1), содержащих минимальные и максимальные значения точек линии на всём диапазоне точек, а также на переданном диапазоне.

##### *functions*:
Область видимости | Возвращаемое значение | Название | Назначение
:-:|:-:|-|-|
public | void | CreateChartViewport<br>() | Создаёт график без линий со значениями параметров по умолчанию.
public | void | AddLine<br>(<br>int32 Index,<br> const TArray\<FVector2D>& InVertices<br>) | Добавляет линию на данный график.
public | void | UpdateLine<br>(<br>int32 Index,<br> const TArray\<FVector2D>& InVertices<br>) | Обновляет линию данного графика.
public | void | ClearLine<br>(<br>int32 Index<br>) | Удаляет линию данного графика.
public | void | ClearAllLines<br>()<br> | Удаляет все линии данного графика.
public | void | SetXRange<br>(<br>const FVector2D& XRangeValue<br>) | Устанавливает диапазон отображения точек графика по оси х.
public | void | SetYRange<br>(<br>const FVector2D& YRangeValue<br>) | 
public | void | SetXAutoscale<br>(<br>bool bAuto<br>) | Устанавливает автомасштабирование по оси х.
public | void | SetYAutoscale<br>(<br>bool bAuto<br>) | 
public | void | SetViewportSize<br>(<br>const FVector2D& ViewportSizeValue<br>) | Устанавливает размер окна графика.
public | void | SetChartGridType<br>(<br>EChartGridType Type<br>) | Устанавливает тип отображения сетки фона графика.
public | void | SetChartBackgroundColor<br>(<br>const FLinearColor& Color<br>) | Устанавливает цвет фона графика.
public | void | SetChartGridColor<br>(<br>const FLinearColor& Color<br>) | Устанавливает цвет сетки графика.
public | void | SetTextColor<br>(<br>EChartTextNames Type,<br> const FColor& Color<br>) | Устанавливает цвет текста.
public | void | SetTextSize<br>(<br>EChartTextNames Type,<br> float Value<br>) | Устанавливает размер текста.
public | void | SetTextTitle<br>(<br>const FText& Value<br>) | Устанавливает заголовок графика.
public | void | SetTextXLabel<br>(<br>const FText& Value<br>) | Устанавливает заголовок оси х графика.
public | void | SetTextYLabel<br>(<br>const FText& Value<br>) | Устанавливает заголовок оси у графика.
public | void | SetLineWidth<br>(<br>int32 Index,<br> float Value<br>) | Устанавливает ширину выбранной линии графика.
public | void | SetLineColor<br>(<br>int32 Index,<br> const FLinearColor& Color<br>) | Устанавливает цвет выбранной линии графика.
public | void | SetLineType<br>(<br>int32 Index,<br> EChartLineType Value<br>) | Устанавливает тип отображения выбранной линии графика.
public | void | SetLineDashLength<br>(<br>int32 Index,<br> float Value<br>) | Устанавливает длину штриха пунктирной линии.
public | void | SetLineDashInterval<br>(<br>int32 Index,<br> float Value<br>) | Устанавливает расстояние между штрихами пунктирной линии.
private | void | Redraw<br>() | Отрисовка графика.
private | void | Trim<br>() | Обрезка линий под окно графика.

##### *variables*:
Область видимости | Тип переменной | Название | Назначение
:-:|:-:|-|-|
private | bool | bAutoscaleX | Применять ли автоматическое масштабирование по оси х.
private | bool | bAutoscaleY |
private | FVector2D | ViewportSize | Размер окна графика.
private | FVector2D | XRange | Отображаемый диапазон значений в окне графика по оси х.
private | FVector2D | YRange |
private | TArray\<FLine> | Lines | Массив структур(1) линий.
private | UChartBackgroundComponent* | ChartBackgroundComponent | Компонент фона графика.
private | UChartLinesComponent* | ChartLinesComponent | Компонент линий графика.

***

#### ChartLinesComponent

ChartLinesComponent — это компонент, содержащий линии. Он отрисовывает линии по переданным координатам с учётом ширины линии, типа и т. п.

##### *functions*:
Область видимости | Возвращаемое значение | Название | Назначение
:-:|:-:|-|-|
public | void | CreateChartLine<br>(<br>int32 Index,<br> const TArray\<FVector2D>& InVertices<br>) | Создаёт линию с параметрами по умолчанию.
public | void | ClearChartLine<br>(<br>int32 Index<br>) | Удаляет конкретную линию.
public | void | ClearAllChartLines<br>() | Удаляет все линии.
public | int32 | GetNumChartLines<br>() const | Возвращает количество линий в компоненте.
public | FLineSection* | GetChartLineSection<br>(<br>int32 Index<br>) | Возвращает конкретную линию.
public | void | SetChartLineSection<br>(<br>int32 Index,<br> const FLineSection& Section<br>) | Устанавливает конкретную линию.
public | void | SetChartLineVertices<br>(<br>int32 Index,<br> const TArray\<FVector2D>& InVertices<br>) | Устанавливает координаты точек линии.
public | void | SetChartLineWidth<br>(<br>int32 Index,<br> float Value<br>) | Устанавливает ширину конкретной линии.
public | void | SetChartLineColor<br>(<br>int32 Index,<br> FLinearColor Value<br>) | Устанавливает цвет конкретной линии.
public | void | SetChartLineType<br>(<br>int32 Index,<br> EChartLineType Value<br>) | Устанавливает тип отображения конкретной линии.
public | void | SetChartLineDashLength<br>(<br>int32 Index,<br> float Value<br>) | Устанавливает длину штриха конкретной пунктирной линии.
public | void | SetChartLineDashInterval<br>(<br>int32 Index,<br> float Value<br>) | Устанавливает расстояние между штрихами конкретной пунктирной линии.
public | void | TrimToChart<br>(<br>const TArray<FVector>& InPlanePositions,<br> const TArray<FVector>& InPlaneNormals<br>) | Обрезает все линии по переданным плоскостям.
private | void | UpdateChartLine<br>(<br>int32 Index,<br> bool bUpdate = false<br>) | Отрисовывает/перерисовывает конкретную линию с заданными параметрами.

##### *variables*:
Область видимости | Тип переменной | Название | Назначение
:-:|:-:|-|-|
private | UProceduralMeshComponent* | BackgroundControllerProcMesh | Компонент процедурной сетки, отвечающий за отрисовку линий.
private | UMaterialInterface* | Material | Материал для всех линий.
private | TArray\<FLineSection> | Lines | Массив структур с параметрами линий.
private | float | Step | Расстояние между линиями по оси z.

***

#### ChartLineSection

ChartLineSection — это структура, содержащая параметры линии. По переданным координатам линии возвращает вершины линии, рассчитанные с учётом её параметров.

Файл со структурой содержит также enum значения с типами отображения линии.

##### *enum EChartLineType*:
Название | Назначение
:-:|-|
Solid | Сплошная линия.<br><img src="Imgs/SolidLine.png" width="100" height="70" />
Dashed | Пунктирная линия.<br><img src="Imgs/DashedLine.png" width="100" height="70" />
Dotted | Вместо линии отображаются точки.<br><img src="Imgs/DottedLine.png" width="100" height="70" />

##### *functions*:
Область видимости | Возвращаемое значение | Название | Назначение
:-:|:-:|-|-|
public | void | Reset<br>() | Сбрасывает значения параметров линии к значениям по умолчанию.
public | void | BuildLine<br>(<br>TArray\<FVector>& OutVertices,<br> TArray\<FVector2D>& OutUVs<br>) | Возвращает значения вершин линии и UV-координат с учётом её параметров.
private | void | BuildSolidLine<br>(<br>TArray\<FVector>& OutVertices,<br> TArray\<FVector2D>& OutUVs<br>) const | 
private | void | BuildDashedLine<br>(<br>TArray\<FVector>& OutVertices,<br> TArray\<FVector2D>& OutUVs<br>) const | 
private | void | BuildDottedLine<br>(<br>TArray\<FVector>& OutVertices,<br> TArray\<FVector2D>& OutUVs<br>) const | 
private | bool | BuildInnerVertices<br>(<br>const FVector& Start,<br> const FVector& Middle,<br> const FVector& End,<br> TArray\<FVector>& OutVertices<br>) const | Получение координат вершин для стыков линии.<br><img src="Imgs/Adj.png" width="150" height="150" />
private | void | BuildOuterVertices<br>(<br>const FVector& Start,<br> const FVector& End,<br> TArray\<FVector>& OutVertices,<br> bool bStart) const | Получение координат граничных вершин.<br><img src="Imgs/Outer.png" width="150" height="150" />
private | float | AttachToDecimalRange<br>(<br>float Value,<br> const FVector2D& Range<br>) const | Нахождение ближайшего значения с заданной десятичной частью в диапазоне.
private | float | AttachToDecimal<br>(<br>float Value,<br> float Decimal<br>) const | Нахождение ближайшего значения с заданной десятичной частью.
private | FVector | InvertAxisYZ<br>(<br>const FVector& Value<br>) const | Инвертирование координат по осям у и z.

##### *variables*:
Область видимости | Тип переменной | Название | Назначение
:-:|:-:|-|-|
public | UMaterialInstanceDynamic* | DynMaterial | Динамический инстанс материала линии.
public | float | ZDepth | Глубина линии по оси z.
public | TArray\<FVector2D> | Vertices | Координаты линии.
public | float | Width | Ширина линии.
public | FLinearColor | Color | Цвет линии.
public | EChartLineType | Type | Тип линии.
public | float | DashLength | Длина штриха пунктирной линии.
public | float | DashInterval | Расстояние между штрихами пунктирной линии.

***
#### ChartBackgroundComponent

ChartBackgroundComponent — это компонент, отображающий фон графика. Отображает также числовые значения по осям графика, подписи осей, заголовок графика.

Файл с компонентом фона содержит также enum значения с типами отображения сетки фона и enum значения с типами текста (заголовок, подписи осей и т. п.).

##### *enum EChartGridType*:
Название | Назначение
:-:|-|
NoGrid | Без сетки.<br><img src="Imgs/NoGrid.png" width="70" height="70" />
Solid | Сетка из сплошных линий.<br><img src="Imgs/SolidGrid.png" width="70" height="70" />
SolidHorizontal | Сплошные линии только по горизонтали.<br><img src="Imgs/SolidGridH.png" width="70" height="70" />
SolidVertical | Сплошные линии только по вертикали.<br><img src="Imgs/SolidGridV.png" width="70" height="70" />
Dashed | Сетка из пунктирных линий.<br><img src="Imgs/DashedGrid.png" width="70" height="70" />
DashedHorizontal | Пунктирные линии только по горизонтали.<br><img src="Imgs/DashedGridH.png" width="70" height="70" />
DashedVertical | Пунктирные линии только по вертикали.<br><img src="Imgs/DashedGridV.png" width="70" height="70" />
StripedHorizontal | Горизонтальные полосы.<br><img src="Imgs/StripedGridH.png" width="70" height="70" />
StripedVertical | Вертикальные полосы.<br><img src="Imgs/StripedGridV.png" width="70" height="70" />

##### *functions*:
Область видимости | Возвращаемое значение | Название | Назначение
:-:|:-:|-|-|
public | void | CreateChartBackground<br>(<br>const FVector2D& XRangeValue,<br> const FVector2D& YRangeValue<br>) | Создаёт фон со значениями по умолчанию.
public | void | SetChartBackgroundXRange<br>(<br>const FVector2D& XRangeValue<br>) | Устанавливает диапазон по оси х, в котором отображаются числовые значения.
public | FVector2D | GetChartBackgroundXRange<br>() const | Возвращает диапазон по оси х, в котором отображаются числовые значения.
public | void | SetChartBackgroundYRange<br>(<br>const FVector2D& YRangeValue<br>) | Устанавливает диапазон по оси у, в котором отображаются числовые значения.
public | FVector2D | GetChartBackgroundYRange<br>() const | Возвращает диапазон по оси у, в котором отображаются числовые значения.
public | void | SetChartBackgroundViewportSize<br>(<br>const FVector2D& ViewportSize<br>) | Устанавливает размеры фона графика по вертикали и горизонтали.
public | FVector2D | GetChartBackgroundViewportSize<br>() const |
public | void | SetChartGridType<br>(<br>EChartGridType Type<br>) | Устанавливает тип координатной сетки фона графика.
public | void | SetChartBackgroundColor<br>(<br>const FLinearColor& Color<br>) | Устанавливает цвет фона графика.
public | void | SetChartGridColor<br>(<br>const FLinearColor& Color<br>) | Устанавливает цвет координатной сетки графика.
public | void | SetTextColor<br>(<br>EChartTextNames Type,<br> const FColor& Color<br>) | Устанавливает цвет текста на выбор: заголовка, числовых значений, подписей осей.
public | void | SetTextSize<br>(<br>EChartTextNames Type,<br> float Value<br>) | Устанавливает размер текста на выбор: заголовка, числовых значений, подписей осей.
public | void | SetTextTitle<br>(<br>const FText& Value<br>) | Устанавливает текст заголовка.
public | void | SetTextXLabel<br>(<br>const FText& Value<br>) | Устанавливает подпись оси х.
public | void | SetTextYLabel<br>(<br>const FText& Value<br>) | Устанавливает подпись оси у. 
private | void | DrawChartBackground<br>() | Отрисовывает (обновляет) фон с заданными параметрами.
private | void | DrawBackgroundPart<br>(<br>EBackgroundPartNames Part,<br> const TArray\<FVector2D>& UVs<br>) | Отрисовывает часть фона на выбор: фон, сетку...
private | void | RenderAxisValuesText<br>(<br>const FVector2D& ClampedStart,<br> const FVector2D& Start,<br> const FVector2D& Step<br>) | Отрисовывает все текстовые значения.
private | void | SetMaterialGridParams<br>(<br>EBackgroundPartNames Part,<br> const FVector2D& VertWidthOffset,<br> const FVector2D& HorWidthOffset,<br> float bSecondGridLayer = 0.0<br>) | Устанавливает параметры материала для части фона на выбор.
private | void | BuildSolidUVs<br>(<br>TArray\<FVector2D>& UV0s,<br> TArray\<FVector2D>& UV1s<br>) const | Рассчитывает UV-координаты для сетки из сплошных линий по горизонтали и по вертикали.
private | void | BuildSolidUVs<br>(<br>TArray\<FVector2D>& UVs,<br> bool bHorisontal = false<br>) const | Рассчитывает UV-координаты для сетки из сплошных линий только по горизонтали или по вертикали.
private | void | BuildDashedUVs<br>(<br>TArray\<FVector2D>& UV0s,<br> TArray\<FVector2D>& UV1s<br>) const | Рассчитывает UV-координаты для сетки из пунктирных линий по горизонтали и по вертикали.
private | void | BuildDashedUVs<br>(<br>TArray\<FVector2D>& UVs,<br> bool bHorisontal = false<br>) const | Рассчитывает UV-координаты для сетки из пунктирных линий только по горизонтали или по вертикали.
private | void | BuildStripedUVs<br>(<br>TArray\<FVector2D>& UVs,<br> bool bHorisontal = false<br>) const | Рассчитывает UV-координаты для сетки из полос только по горизонтали или по вертикали.
private | float | GetGridStep<br>(<br>const FVector2D& Range<br>) const | Рассчитывает шаг между тиками (числовыми значениями по осям).
private | float | GetGridOffset<br>(<br>float Start,<br> float Step<br>) const | Рассчитывает отступ тиков от начальной точки фона.
private | float | AttachToDecimal<br>(<br>float Value,<br> float Decimal<br>) const | Нахождение ближайшего значения с заданной десятичной частью.

##### *variables*:
Область видимости | Тип переменной | Название | Назначение
:-:|:-:|-|-|
private | UProceduralMeshComponent* | BackgroundControllerProcMesh | Компонент процедурной сетки, отвечающий за отрисовку фона.
private | UMaterialInterface* | Material | Материал для фона.
private | TArray<UMaterialInstanceDynamic*> | DynMaterials | Динамические материал инстансы для частей фона (фона, сетки, рамки (не используется в настоящее время)).
private | TArray<UTextRenderComponent*> | AxisValues | Рендер таргеты для отрисовки числовых значений по осям.
private | TArray<UTextRenderComponent*> | TextParts | Рендер таргеты для отрисовки остальных текстовых значений.
private | FVector2D | StepClamped | Шаг между тиками (числовыми значениями по осям), подогнанный под размеры фона.
private | int32 | MaxGridLinesCount | Максимально возможное число координатных линий по обеим осям.
private | float | GridLineWidth | Ширина координатных линий (процент от максимальной размерности (ширины или длины) фона).
private | float | GridDashLength | Длина штрихов координатных линий (только для пунктирной сетки).
private | float | GridDashInterval | Расстояние между штрихами координатных линий (только для пунктирной сетки).
private | int32 | MaximumFractionalDigits | Максимальное количество знаков после запятой у числовых значений.
private | float | TextOffset | Отступы текста от границ фона.
private | float | DistBackgroundParts | Расстояние между частями фона (фоном, сеткой, рамкой (не используется в настоящее время)).
private | float | FrameWidth | Ширина рамки (не используется в настоящее время).
private | FLinearColor | FrameColor | Цвет рамки (не используется в настоящее время).
private | FVector2D | BackgroundViewportSize | Размер фона графика по вертикали и горизонтали.
private | FVector2D | XRange | Диапазон по оси х, в котором отображаются числовые значения.
private | FVector2D | YRange | Диапазон по оси у, в котором отображаются числовые значения.
private | EChartGridType | GridType | Тип координатной сетки.
private | FLinearColor | BackgroundColor | Цвет фона графика.
private | FLinearColor | GridColor | Цвет координатной сетки.
