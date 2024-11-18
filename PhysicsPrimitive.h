#ifndef PHYSICSCOURSEGRAPHICS_H
#define PHYSICSCOURSEGRAPHICS_H

#include "Primitive.h"

class PhysicsPrimitive:public Primitive
{
public:
    enum PhysicsType{SmoothSurface,RoughSurface,Parabola};
    PhysicsPrimitive();
    PhysicsPrimitive(QPen pen,PhysicsType type, QVector<QPoint> args);
    QPoint center() const;
    PhysicsType type() const;
    QPen pen();
    QVector<QPoint> args() const;
    QVector<QPoint> points() const;
    bool contain(QPoint p);
    bool containArgs(QPoint p);
    void setArgs(QVector<QPoint> args);
    void setPoints(QVector<QPoint> points);
    void addPoint(const QPoint &point);
    void setPenWidth(int width);
    static QVector<QPoint> DrawSmoothSurface(QVector<QPoint> args);
    static QVector<QPoint> DrawRoughSurface(QVector<QPoint> args);
    static QVector<QPoint> drawParabola(QVector<QPoint> args);
private:
    PhysicsType type_;
    QPoint center_;
    QVector<QPoint> args_;
    QVector<QPoint> points_;
    QPen pen_;
};

#endif // PHYSICSCOURSEGRAPHICS_H
