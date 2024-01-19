#include <QApplication>

#include <string>

#include "MainWindow.hpp"
#include "MainWindowController.hpp"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    
    // Создание окна и контроллера
    MainWindow* window = new MainWindow();
    MainWindowController* controller = new MainWindowController(window);

    // Передача всех событий контроллеру
    app.installEventFilter(controller);

    // Показ окна
    window->resize(1350, 850);
    window->setWindowTitle("Path Tracer");
    window->show();
    
    return app.exec();
}
