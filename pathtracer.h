#ifndef PATHTRACER_H
#define PATHTRACER_H

#include <QObject>

class PathTracer : public QObject
{
    Q_OBJECT
public:
    explicit PathTracer(QObject *parent = nullptr);

signals:

};

#endif // PATHTRACER_H
