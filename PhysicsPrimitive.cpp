#include "PhysicsPrimitive.h"

PhysicsPrimitive::PhysicsPrimitive()
{

}

PhysicsPrimitive::PhysicsPrimitive(QPen pen,PhysicsPrimitive::PhysicsType type, QVector<QPoint> args)
    : type_(type),pen_(pen)
{
    setArgs(args);
}

QPoint PhysicsPrimitive::center() const
{
    return center_;
}

PhysicsPrimitive::PhysicsType PhysicsPrimitive::type() const
{
    return type_;
}

QPen PhysicsPrimitive::pen()
{
    return pen_;
}

QVector<QPoint> PhysicsPrimitive::args() const
{
    return args_;
}

QVector<QPoint> PhysicsPrimitive::points() const
{
    return points_;
}

bool PhysicsPrimitive::contain(QPoint pos){
    foreach (QPoint p, _points)
    {
        QPoint diff = p - pos;
        if (diff.x() * diff.x() + diff.y() * diff.y() < 25)
            return true;
    }
    return false;
}

bool PhysicsPrimitive::containArgs(QPoint pos){
    foreach (QPoint p, _args)
    {
        QPoint diff = p - pos;
        if (diff.x() * diff.x() + diff.y() * diff.y() < 25)
            return true;
    }
    return false;
}

void PhysicsPrimitive::setArgs(QVector<QPoint> args)
{
    _args = args;
    setPoints(args);
    int x = 0, y = 0;
    if(_type == ARC){
        QPoint p1 = args[0];
        QPoint p2 = args[1];

        int radius = args[2].x();

        QPoint midPoint((p1.x() + p2.x()) / 2, (p1.y() + p2.y()) / 2);

        double dx = p2.x() - p1.x();
        double dy = p2.y() - p1.y();
        double dist = std::sqrt(dx * dx + dy * dy);

        double h = std::sqrt(radius * radius - (dist / 2) * (dist / 2));
        double cx = midPoint.x() - h * dy / dist;
        double cy = midPoint.y() + h * dx / dist;
        _center = QPoint(cx, cy);
        return;
    }

    for (QPoint p : args)
    {
        x += p.x();
        y += p.y();
    }
    int centerX = x / args.size();
    int centerY = y / args.size();
    _center = QPoint(centerX, centerY);
}

void PhysicsPrimitive::setPoints(QVector<QPoint> args) {
    switch (type_)
    {
    case PhysicsPrimitive::SmoothSurface:
        points_ = DrawSmoothSurface(args);
        break;
    case PhysicsPrimitive::RoughSurface:
        points_ = DrawRoughSurface(args);
        break;
    case PhysicsPrimitive::Parabola:
        points_ = drawParabola(args);
        break;
    default:
        break;
    }
}

void PhysicsPrimitive::addPoint(const QPoint &point)
{
    _args.append(point);
    setPoints(_args);
}

void PhysicsPrimitive::setPenWidth(int width)
{
    _pen.setWidth(width);
}

QVector<QPoint> PhysicsPrimitive::DrawSmoothSurface(QVector<QPoint> args)
{
    QVector<QPoint> points;
    points=MidpointDrawLine(args);
    return points;
}

QVector<QPoint> PhysicsPrimitive::DrawRoughSurface(QVector<QPoint> args)
{
    QVector<QPoint> points;
    points=MidpointDrawLine(args);
    return points;
}

QVector<QPoint> PhysicsPrimitive::drawParabola(QVector<QPoint> args)
{
    QVector<QPoint> points;
    points=drawBezierCurve(args);
    return points;
}
