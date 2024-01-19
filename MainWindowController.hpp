#pragma once

#include <QEvent>
#include <QKeyEvent>

#include "MainWindow.hpp"
#include "Input.hpp"
#include "Vector2.hpp"

class MainWindowController : public QObject
{
    Q_OBJECT

public:
    MainWindowController(MainWindow* window);
    
protected:
    
    // Перехватывание событий для виджета рендеринга
    bool eventFilter(QObject* obj, QEvent* event);
    
public slots:
    
    // Виджеты настройки рендеринга
    void resolutionScaleToggled();
    void renderComponentToggled();
    void overlayToggled();
    
private:
    MainWindow* window_;
    InputManager inputManager_;
    
    bool mouseDragging_;
    Vector2 mousePosition_;
    
    // Вызов каждого кадра
    void update(float deltaTime);
    void applyCameraMovement(float deltaTime);
    void updateStatsUI();
    
    // Обработка событий Qt
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
};
