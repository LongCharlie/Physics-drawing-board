#ifndef PRIMITIVE_H
#define PRIMITIVE_H
#include<QStack>
#include <QPoint>
#include <QPen>
#include <QVector>
#include <QPainter>
#include <QtMath>
#include <QtAlgorithms>
#include <QDebug>
#include <QPixmap>
#include "MathModule.h"

class Primitive {
public:
    enum Type { Line, Line2, DashedLine, Polygon, Circle, Ellipse, BezierCurve, ARC, BSplineCurve,Fill,
                Surface, Ramp,Semicircle,Quartercircle,Parabola,Curve,Curverow,Row, DashedRow,Masspoint,Arctrack,
                Barriertrack,Belt,Track,Ltrack,Spring,Weight,Weight2,Weight3,Bullet,Block,Skateboard,Pallet,Car,Plane};
    Primitive();
    Primitive(QPen pen,Type type, QVector<QPoint> args);
    Primitive(QPen pen,Type type);
    QPoint center() const;
    Type type() const;
    QPen pen();
    QVector<QPoint> args() const;
    QVector<QPoint> points() const;
    bool contain(QPoint p);
    bool containArgs(QPoint p);
    void setArgs(QVector<QPoint> args);
    void setPoints(QVector<QPoint> points);
    void addPoint(const QPoint &point);
    void setPenWidth(int width);
    static QVector<QPoint> BresenhamDrawDashedLine(QVector<QPoint> args,int dashLength);
    static QVector<QPoint> BresenhamDrawLine(QVector<QPoint> args);        // 绘制直线（Bresenham算法）
    static QVector<QPoint> MidpointDrawLine(QVector<QPoint> args);// 绘制直线（中点算法）
    static QVector<QPoint> drawPolygon(QVector<QPoint> args);     // 绘制多边形
    static QVector<QPoint> drawCircle(QVector<QPoint> args);	  // 绘制圆形
    static QVector<QPoint> drawEllipse(QVector<QPoint> args);	  // 绘制椭圆
    static QVector<QPoint> drawBezierCurve(QVector<QPoint> args); // 绘制Bezier曲线
    static QVector<QPoint> drawBSplineCurve(QVector<QPoint> args);// 绘制B样条曲线
    static QVector<QPoint> drawARC(QVector<QPoint> args);         // 绘制圆弧
    QVector<QPoint> translate(QPoint pos);		                   // 平移
    QVector<QPoint> translateArg(QPoint arg,QPoint pos);
    QVector<QPoint> rotate(QPoint center,qreal r);			       // 旋转
    QVector<QPoint> scale(QPoint center,float s);				   // 缩放
    QVector<QPoint> CohenSutherlandClip(QPoint lt, QPoint rb);	   // 裁剪（CohenSuther）
    QVector<QPoint> MidpointSubdivisionClip(QPoint lt, QPoint rb); // 裁剪（中点）
    void setImage(const QImage image);
    QVector<QPoint> fill(QImage& image);

    static QVector<QPoint> drawSurface(QVector<QPoint> args);
    static QVector<QPoint> drawRamp(QVector<QPoint> args);
    static QVector<QPoint> drawSemicircle(QVector<QPoint> args);
    static QVector<QPoint> drawquartercircle(QVector<QPoint> args);
    static QVector<QPoint> drawparabola(QVector<QPoint> args);
    static QVector<QPoint> drawcurve(QVector<QPoint> args);
    static QVector<QPoint> drawcurverow(QVector<QPoint> args);
    static QVector<QPoint> drawrow(QVector<QPoint> args);
    static QVector<QPoint> drawdashedrow(QVector<QPoint> args);
    static QVector<QPoint> drawmasspoint(QVector<QPoint> args);
    static QVector<QPoint> drawarctrack(QVector<QPoint> args);
    static QVector<QPoint> drawbarriertrack(QVector<QPoint> args);
    static QVector<QPoint> drawbelt(QVector<QPoint> args);
    static QVector<QPoint> drawtrack(QVector<QPoint> args);
    static QVector<QPoint> drawLtrack(QVector<QPoint> args);
    static QVector<QPoint> drawspring(QVector<QPoint> args);
    static QVector<QPoint> drawweight(QVector<QPoint> args);
    static QVector<QPoint> drawweight2(QVector<QPoint> args);
    static QVector<QPoint> drawweight3(QVector<QPoint> args);
    static QVector<QPoint> drawbullet(QVector<QPoint> args);
    static QVector<QPoint> drawblock(QVector<QPoint> args);
    static QVector<QPoint> drawskateboard(QVector<QPoint> args);
    static QVector<QPoint> drawpallet(QVector<QPoint> args);
    static QVector<QPoint> drawcar(QVector<QPoint> args);
    static QVector<QPoint> drawplane(QVector<QPoint> args);

protected:
    Type _type;
    QPoint _center;
    QVector<QPoint> _args;
    QVector<QPoint> _points;
    QPen _pen;
    QImage _image;
};


#endif // PRIMITIVE_H
