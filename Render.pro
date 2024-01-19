QT       += core gui widgets
QT += openglwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

SOURCES += \
    AreaLight.cpp \
    Bitmap.cpp \
    Camera.cpp \
    Color.cpp \
    Input.cpp \
    Light.cpp \
    MainWindow.cpp \
    MainWindowController.cpp \
    Matrix4x4.cpp \
    Object.cpp \
    Plane.cpp \
    Quaternion.cpp \
    Random.cpp \
    Ray.cpp \
    Renderer.cpp \
    RendererStats.cpp \
    RendererWidget.cpp \
    Scene.cpp \
    Sphere.cpp \
    SurfaceHit.cpp \
    Triangle.cpp \
    Vector2.cpp \
    Vector3.cpp \
    Vector4.cpp \
    main.cpp


HEADERS += \
    AreaLight.hpp \
    Bitmap.hpp \
    Camera.hpp \
    Color.hpp \
    Input.hpp \
    Light.hpp \
    MainWindow.hpp \
    MainWindowController.hpp \
    Matrix4x4.hpp \
    Object.hpp \
    Plane.hpp \
    Quaternion.hpp \
    Random.hpp \
    Ray.hpp \
    Renderer.hpp \
    RendererSettings.hpp \
    RendererStats.hpp \
    RendererWidget.hpp \
    Scene.hpp \
    Sphere.hpp \
    SurfaceHit.hpp \
    Triangle.hpp \
    Vector2.hpp \
    Vector3.hpp \
    Vector4.hpp


FORMS += \
    mainwindow.ui
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    Render.pro.user
