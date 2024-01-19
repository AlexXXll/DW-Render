#include "MainWindowController.hpp"

#include <cstdint>

MainWindowController::MainWindowController(MainWindow* window)
    : window_(window),
    inputManager_(),
    mouseDragging_(false),
    mousePosition_(Vector2(0, 0))
{
    // Сигналы переключения шкалы разрешения
    for(int i = 1; i < window_->resolutionScaleRadios().size(); ++i)
    {
        connect(window_->resolutionScaleRadios()[i], SIGNAL(toggled(bool)), this, SLOT(resolutionScaleToggled()));
    }
    
    // Сигналы переключения компонентов рендеринга
    for(int i = 1; i < window_->renderComponentToggles().size(); ++i)
    {
        connect(window_->renderComponentToggles()[i], SIGNAL(stateChanged(int)), this, SLOT(renderComponentToggled()));
    }
    
    // Сигналы переключателя наложения отладки
    for(int i = 1; i < window_->overlayRadios().size(); ++i)
    {
        connect(window_->overlayRadios()[i], SIGNAL(toggled(bool)), this, SLOT(overlayToggled()));
    }
}

bool MainWindowController::eventFilter(QObject* obj, QEvent* event)
{
    if(event->type() == QEvent::Paint)
    {
        update((1.0 / 60.0));
    }
    else if(event->type() == QEvent::MouseButtonPress && obj == window_->rendererWidget())
    {
        mousePressEvent(static_cast<QMouseEvent*>(event));
    }
    else if(event->type() == QEvent::MouseButtonRelease && obj == window_->rendererWidget())
    {
        mouseReleaseEvent(static_cast<QMouseEvent*>(event));
    }
    else if(event->type() == QEvent::MouseMove && obj == window_->rendererWidget())
    {
        mouseMoveEvent(static_cast<QMouseEvent*>(event));
    }
    
    // InputManager обрабатывает события нажатия/отпускания клавиш.
    if(event->type() == QEvent::KeyPress)
    {
        inputManager_.keyPressed((InputKey)static_cast<QKeyEvent*>(event)->key());
    }
    else if(event->type() == QEvent::KeyRelease)
    {
        inputManager_.keyReleased((InputKey)static_cast<QKeyEvent*>(event)->key());
    }
    
    // Необработанные события передаются обратно в Qt
    return QObject::eventFilter(obj, event);
}

void MainWindowController::resolutionScaleToggled()
{
    // Отправитель — переключатель шкалы разрешения.
    QRadioButton* sender = (QRadioButton*)QObject::sender();
    RenderResolution scale = (RenderResolution)sender->property("scale").toInt();
    
    // Обновление метода теней
    window_->rendererWidget()->setResolutionScale(scale);
}

void MainWindowController::renderComponentToggled()
{
    // Отправитель — флажок компонента рендеринга.
    QCheckBox* checkbox = (QCheckBox*)QObject::sender();
    RenderComponent component = (RenderComponent)checkbox->property("component").toInt();
    
    // Обновление виджета рендеринга
    window_->rendererWidget()->toggleRenderComponent(component);
}

void MainWindowController::overlayToggled()
{
    // Отправитель — переключатель плоскости.
    QRadioButton* radio = (QRadioButton*)QObject::sender();
    RenderOverlay overlay = (RenderOverlay)radio->property("overlay").toInt();
    
    // Обновление индекса плоскости
    window_->rendererWidget()->setOverlay(overlay);
}

void MainWindowController::update(float deltaTime)
{
    // Перемещение камеры с помощью пользовательского ввода
    applyCameraMovement(deltaTime);
    
    // Обновление интерфейса статистики
    updateStatsUI();
}

void MainWindowController::applyCameraMovement(float deltaTime)
{
    Camera* camera = window_->rendererWidget()->camera();

    // Рассчёт движения на основе входных данных
    Vector3 movement;
    movement.x = inputManager_.getSidewaysMovement();
    movement.z = inputManager_.getForwardsMovement();
    movement.y = inputManager_.getVerticalMovement();
    movement = movement * inputManager_.getMovementSpeed();
    movement = camera->localToWorldVector(movement);
    
    // Применение движения к камере (независимо от частоты кадров)
    camera->translate(movement * deltaTime);
    
    // Перезапуск рендеринга, если камера переместилась
    if(movement.sqrMagnitude() > 0.0001f)
    {
        window_->rendererWidget()->restartRendering();
    }
}

void MainWindowController::updateStatsUI()
{
    // Получение разрешения рендеринга
    int resX = window_->rendererWidget()->resolutionX();
    int resY = window_->rendererWidget()->resolutionY();
    
    // Получение статистики производительности
    const RendererStats* stats = window_->rendererWidget()->stats();
    double renderTime = stats->currentRenderTime();
    uint64_t sampleCount = stats->sampleCount();
    uint64_t samplesPerSecond = stats->averageSamplesPerSecond();
    uint64_t samplesPerPixel = stats->averageSamplesPerPixel();

    // Создание текста для каждой метки
    QString resolutionText = QString("%1 x %2").arg(resX).arg(resY);
    QString renderTimeText = QString("Время рендера: %1").arg(renderTime);
    QString sampleCountText = QString("Всего Samples: %1").arg(sampleCount);
    QString samplesPerSecondText = QString("Samples/s: %1").arg(samplesPerSecond);
    QString samplesPerPixelText = QString("Среднее количество Samples: %1").arg(samplesPerPixel);

    // Обновление метки статистики
    window_->resolutionLabel()->setText(resolutionText);
    window_->renderTimeLabel()->setText(renderTimeText);
    window_->sampleCountLabel()->setText(sampleCountText);
    window_->samplesPerSecondLabel()->setText(samplesPerSecondText);
    window_->samplesPerPixelLabel()->setText(samplesPerPixelText);
}

void MainWindowController::mousePressEvent(QMouseEvent* event)
{
    if(event->button() == Qt::MouseButton::LeftButton)
    {
        mouseDragging_ = true;
        mousePosition_ = Vector2(event->pos().x(), event->pos().y());
    }
}

void MainWindowController::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::MouseButton::LeftButton)
    {
        mouseDragging_ = false;
    }
}

void MainWindowController::mouseMoveEvent(QMouseEvent *event)
{
    if(mouseDragging_)
    {
        Camera* camera = window_->rendererWidget()->camera();
        
        // Определение движения мыши
        Vector2 oldPosition = mousePosition_;
        Vector2 newPosition = Vector2 (event->pos().x(), event->pos().y());
        Vector2 deltaPosition = newPosition - oldPosition;
        mousePosition_ = newPosition;
        
        // Применение горизонтального вращения
        Quaternion horizontal = Quaternion::rotation(deltaPosition.x * 0.5, Vector3::up());

        // Рассчёт вертикального вращения
        Vector3 upAxis = camera->up();
        Vector3 forwardAxis = camera->forward();
        Vector3 sidewaysAxis = Vector3::cross(upAxis, forwardAxis);
        Quaternion vertical = Quaternion::rotation(deltaPosition.y * 0.5, sidewaysAxis);
    
        // Применение вращения, сначала по вертикали
        camera->setRotation(horizontal * vertical * camera->rotation());
        
        // Перезапуск рендеринга, вращая камеру.
        window_->rendererWidget()->restartRendering();
    }
}
