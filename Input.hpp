#pragma once

#include <QEvent>
#include <QKeyEvent>

#include <vector>

using namespace std;

// Использует клавиши Qt для клавиш ввода
typedef Qt::Key InputKey;

class InputManager
{
public:
    InputManager();
    
    // Состояние клавиш
    bool isKeyUp(InputKey key) const;
    bool isKeyDown(InputKey key) const;
    bool anyKeyDown() const;
    
    // Направление осей
    float getForwardsMovement() const;
    float getSidewaysMovement() const;
    float getVerticalMovement() const;
    float getMovementSpeed() const;
    
    // Обновление состояния нажатия клавиши
    void keyPressed(InputKey key);
    void keyReleased(InputKey key);
    
private:
    vector<InputKey> pressedKeys_;
    
    float getAxisMovement(InputKey positiveDir, InputKey negativeDir) const;
};
