#pragma once

#include <QWidget>
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSlider>

#include "RendererWidget.hpp"

class MainWindow : public QWidget
{
public:
    MainWindow();

    // Отрисовщик и боковая панель
    RendererWidget* rendererWidget() const { return rendererWidget_; }
    QWidget* sidePanelWidget() const { return sidePanelWidget_; }

    // Виджеты статистики
    QLabel* resolutionLabel() const { return resolutionLabel_; }
    QLabel* renderTimeLabel() const { return renderTimeLabel_; }
    QLabel* sampleCountLabel() const { return sampleCountLabel_; }
    QLabel* samplesPerSecondLabel() const { return samplesPerSecondLabel_; }
    QLabel* samplesPerPixelLabel() const { return samplesPerPixelLabel_; }

    // Списки виджетов настроек рендеринга
    QObjectList resolutionScaleRadios() { return resolutionScaleRadios_->children(); }
    QObjectList renderComponentToggles() { return renderComponentToggles_->children(); }
    QObjectList overlayRadios() { return overlayRadios_->children(); }
    
private:
    
    // Главный виджет рендеринга
    RendererWidget* rendererWidget_;
    
    // Виджет боковой панели
    QWidget* sidePanelWidget_;
    
    // Ярлыки статистики
    QGroupBox* statsGroupBox_;
    QLabel* resolutionLabel_;
    QLabel* renderTimeLabel_;
    QLabel* sampleCountLabel_;
    QLabel* samplesPerSecondLabel_;
    QLabel* samplesPerPixelLabel_;
    
    // Группы виджетов настройки рендеринга
    QGroupBox* resolutionScaleRadios_;
    QGroupBox* renderComponentToggles_;
    QGroupBox* overlayRadios_;
    
    QLabel* createStatsLabel();
    QRadioButton* createResolutionScaleRadio(RenderResolution scale, const char* label);
    QCheckBox* createRenderComponentToggle(RenderComponent component, const char* label);
    QRadioButton* createOverlayRadio(RenderOverlay overlay, const char* label);
    QSlider* createSlider(QGroupBox* parent, float value, float min, float max);
};
