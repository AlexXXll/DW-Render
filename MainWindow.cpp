#include "MainWindow.hpp"

#include <QVariant>
#include <QScrollArea>
#include <QCheckBox>

MainWindow::MainWindow()
{
    // Создание основного рендера
    rendererWidget_ = new RendererWidget();
    
    // Создание групп
    statsGroupBox_ = new QGroupBox("Статистика");
    resolutionScaleRadios_ = new QGroupBox("Масштаб разрешения");
    renderComponentToggles_ = new QGroupBox("Компоненты рендеринга");
    overlayRadios_ = new QGroupBox("Наложение отладки");
    
    // Использование вертикальной компоновки для всех групп.
    statsGroupBox_->setLayout(new QBoxLayout(QBoxLayout::TopToBottom));
    resolutionScaleRadios_->setLayout(new QBoxLayout(QBoxLayout::TopToBottom));
    renderComponentToggles_->setLayout(new QBoxLayout(QBoxLayout::TopToBottom));
    overlayRadios_->setLayout(new QBoxLayout(QBoxLayout::TopToBottom));

    // Создание виджетов статистики
    resolutionLabel_ = createStatsLabel();
    renderTimeLabel_ = createStatsLabel();
    sampleCountLabel_ = createStatsLabel();
    samplesPerSecondLabel_ = createStatsLabel();
    samplesPerPixelLabel_ = createStatsLabel();
    
    // Создание преключателей с масштабом разрешения
    createResolutionScaleRadio(RS_FullRes, "Native (очень медленный)");
    createResolutionScaleRadio(RS_HalfRes, "1/2 Разрешение");
    createResolutionScaleRadio(RS_QuarterRes, "1/4 Разрешение");
    createResolutionScaleRadio(RS_EighthRes, "1/8 Разрешение")->setChecked(true); // Default
    createResolutionScaleRadio(RS_SixteenthRes, "1/16 Разрешениеn");
    
    // Создание переключателей компонентов рендеринга
    createRenderComponentToggle(RC_Direct, "Прямой свет");
    createRenderComponentToggle(RC_Impulse, "Импульсный свет");
    createRenderComponentToggle(RC_Indirect, "Непрямой свет");
    createRenderComponentToggle(RC_Shadows, "Тени");
    createRenderComponentToggle(RC_Spheres, "Сферы");
    
    // Создание переключателей наложений
    createOverlayRadio(RO_None, "Без наложения")->setChecked(true); // Default = No Overlay
    createOverlayRadio(RO_PixelRays, "Пиксельные лучи");
    createOverlayRadio(RO_Distance, "Расстояние попадания");
    createOverlayRadio(RO_Position, "Позиция попадания");
    createOverlayRadio(RO_Color, "Цвет поверхности");
    createOverlayRadio(RO_Normals, "Нормали поверхности");
    createOverlayRadio(RO_Emission, "Поверхностная эмиссия");
    
    // Добавление виджетов на боковую панель
    QBoxLayout* sidePanelLayout = new QBoxLayout(QBoxLayout::TopToBottom);
    sidePanelLayout->addWidget(statsGroupBox_);
    sidePanelLayout->addWidget(resolutionScaleRadios_);
    sidePanelLayout->addWidget(renderComponentToggles_);
    sidePanelLayout->addWidget(overlayRadios_);
    sidePanelLayout->setSpacing(20);
    sidePanelLayout->addStretch();
    
    QWidget* sidePanel = new QWidget();
    sidePanel->setFixedWidth(240);
    sidePanel->setLayout(sidePanelLayout);
    
    // Создание области прокрутки для боковой панели.
    QScrollArea* scrollArea = new QScrollArea();
    scrollArea->setMaximumWidth(260);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    scrollArea->setWidget(sidePanel);
    scrollArea->setWidgetResizable(false);
    scrollArea->setFrameShape(QFrame::NoFrame);
    sidePanelWidget_ = scrollArea;
    
    // Добавление виджетов в основной макет
    QBoxLayout* mainLayout = new QBoxLayout(QBoxLayout::LeftToRight, this);
    mainLayout->addWidget(rendererWidget_);
    mainLayout->addWidget(sidePanelWidget_);
}

QLabel* MainWindow::createStatsLabel()
{
    QLabel* label = new QLabel();
    statsGroupBox_->layout()->addWidget(label);
    return label;
}

QRadioButton* MainWindow::createResolutionScaleRadio(RenderResolution scale, const char* label)
{
    QRadioButton* radio = new QRadioButton(label);
    radio->setProperty("scale", (int)scale);
    
    resolutionScaleRadios_->layout()->addWidget(radio);
    return radio;
}

QCheckBox* MainWindow::createRenderComponentToggle(RenderComponent component, const char* label)
{
    QCheckBox* checkBox = new QCheckBox(label);
    checkBox->setProperty("составной", (int)component);
    checkBox->setChecked(true);
    
    renderComponentToggles_->layout()->addWidget(checkBox);
    return checkBox;
}

QRadioButton* MainWindow::createOverlayRadio(RenderOverlay overlay, const char* label)
{
    // Создание переключателя
    QRadioButton* radio = new QRadioButton(label);
    radio->setProperty("наложение", (int)overlay);
    
    // Добавление в группу наложенных переключателей
    overlayRadios_->layout()->addWidget(radio);
    
    return radio;
}

QSlider* MainWindow::createSlider(QGroupBox* parent, float value, float min, float max)
{
    QSlider* slider = new QSlider();
    slider->setOrientation(Qt::Orientation::Horizontal);
    slider->setMinimum((int)(min * 1000.0f));
    slider->setMaximum((int)(max * 1000.0f));
    slider->setValue((int)(value * 1000.0f));
    parent->layout()->addWidget(slider);
    return slider;
}
