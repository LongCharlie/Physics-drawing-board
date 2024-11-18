#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , state(Line)
    , primitive(nullptr)
    , pen(Qt::black, 2)
    , isBezierInProgress(false)
    , isPolygonInProgress(false)
    , isReferencePointSet(false)
    , isBSplineInProgress(false)
{
    ui->setupUi(this);
    image = QImage(this->size(), QImage::Format_RGBA64);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event)
{

    Q_UNUSED(event);
    painter.begin(&image);
    image.fill(Qt::white);
    foreach (Primitive *p, primitives)
    {
        painter.setPen(p->pen());
        painter.drawPoints(p->points());
        if (state == BezierControl && p->type() == Primitive::BezierCurve)
        {
            QPen controlPointPen(Qt::red, 5);
            painter.setPen(controlPointPen);
            painter.drawPoints(p->args());
        }
        if(p->type() == Primitive::Fill){
            painter.setPen(p->pen());
            p->setPoints(p->fill(image));
            painter.drawPoints(p->points());
        }

    }
    if (isReferencePointSet)
    {
        QPen refPointPen(Qt::blue, 5);
        painter.setPen(refPointPen);
        painter.drawPoint(referencePoint);
    }
    if (state == ClipLine && primitive)
    {
        painter.setPen(primitive->pen());
        painter.drawPoints(primitive->points());
    }
    if (state == ClipLine2 && primitive)
    {
        painter.setPen(primitive->pen());
        painter.drawPoints(primitive->points());
    }
    if (state == Polygon)
    {
        painter.setBrush(QBrush(Qt::black));
        foreach (QPoint p, points)
            painter.drawEllipse(p, 3, 3);
    }
    painter.end();
    ui->label->setPixmap(QPixmap::fromImage(image));
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    QPoint pos = event->pos();
    pos.rx() -= 100;
    pos.ry() -= 75;
    int x=pos.x();
    int y=pos.y();
    points.append(pos);
    switch (state)
    {
    case DashedLine:
        primitive = new Primitive(pen, Primitive::DashedLine,{pos, pos});
        primitives.append(primitive);
        break;
    case Line:
        primitive = new Primitive(pen, Primitive::Line,{pos, pos});
        primitives.append(primitive);
        break;
    case Line2:
        primitive = new Primitive(pen, Primitive::Line2,{pos, pos});
        primitives.append(primitive);
        break;
    case Triangle:
        primitive = new Primitive(pen, Primitive::Polygon,{pos, pos, pos});
        primitives.append(primitive);
        break;
    case Circle:
        primitive = new Primitive(pen, Primitive::Circle,{pos, QPoint(0, 0)});
        primitives.append(primitive);
        break;
    case Ellipse:
        primitive = new Primitive(pen, Primitive::Ellipse,{pos, QPoint(0, 0)});
        primitives.append(primitive);
        break;
    case Polygon:
        if (!isPolygonInProgress)
        {
            primitive = new Primitive(pen, Primitive::Polygon, {pos});
            primitives.append(primitive);
            isPolygonInProgress = true;
        }
        else
        {
            primitive->addPoint(pos);
        }
        break;
    case Rectangle:
        primitive = new Primitive(pen, Primitive::Polygon,{pos, pos, pos, pos});
        primitives.append(primitive);
        break;
    case BezierCurve:
        if (!isBezierInProgress)
        {
            primitive = new Primitive(pen, Primitive::BezierCurve, {pos});
            primitives.append(primitive);
            isBezierInProgress = true;
        }
        else
        {
            primitive->addPoint(pos);
        }
        break;
    case BezierControl:
        foreach (Primitive *p, primitives)
        {
            if (p->type() == Primitive::BezierCurve && p->containArgs(points[0]))
            {
                const QVector<QPoint>& args = p->args();
                for (int i = 0; i < args.size(); ++i)
                {
                    if (qAbs(pos.x() - args[i].x()) < 5 &&
                        qAbs(pos.y() - args[i].y()) < 5)
                    {
                        selectedControlPoint = args[i];
                        primitive = p;
                        break;
                    }
                }
            }
        }
        break;
    case BSplineCurve:
        if (!isBezierInProgress)
        {
            primitive = new Primitive(pen, Primitive::BSplineCurve, {pos});
            primitives.append(primitive);
            isBSplineInProgress = true;
        }
        else
        {
            primitive->addPoint(pos);
        }
        break;
    case ARC:
        break;
    case Translate:
        primitive = nullptr;
        foreach (Primitive *p, primitives)
            if (p->contain(points[0]))
            {
                primitive = p;
                break;
            }
        break;
    case ClipLine:
        primitive = new Primitive(QPen(Qt::black, 1), Primitive::Polygon,{pos, pos, pos, pos});
        break;
    case ClipLine2:
        primitive = new Primitive(QPen(Qt::black, 1), Primitive::Polygon,{pos, pos, pos, pos});
        break;
    case Fill:
        primitive = new Primitive(QPen(Qt::green),Primitive::Fill);
        primitives.append(primitive);
        primitive->setImage(image);
        primitive->setArgs({pos});
        break;
    case ZoomIn:
        primitive = nullptr;
        foreach (Primitive *p, primitives)
            if (p->contain(points[0]))
            {
                primitive = p;
                break;
            }
        break;
    case ZoomOut:
        primitive = nullptr;
        foreach (Primitive *p, primitives)
            if (p->contain(points[0]))
            {
                primitive = p;
                break;
            }
        break;
    case Delete:
        primitive = nullptr;
        foreach (Primitive *p, primitives)
            if (p->contain(points[0]))
            {
                primitive = p;
                break;
            }
        break;
    case SetRP:
        isReferencePointSet = true;
        referencePoint = pos;
        break;
    case DeleteRP:
        break;
    case Rotate:
        primitive = nullptr;
        foreach (Primitive *p, primitives)
            if (p->contain(points[0]))
            {
                primitive = p;
                break;
            }
        break;
    case Surface:
        primitive = new Primitive(pen, Primitive::Surface,{pos, {x+=200,y}});
        primitives.append(primitive);
        break;
    case Ramp:
        primitive = new Primitive(pen, Primitive::Ramp,{pos, {x-=150,y},{x,y-=150}});
        primitives.append(primitive);
        break;
    case Ramp30:
        primitive = new Primitive(pen, Primitive::Polygon,{pos, {x-=150,y},{x,y-=87}});
        primitives.append(primitive);
        break;
    case Semicircle:
        primitive = new Primitive(pen, Primitive::Semicircle,{{x-=150,y},{x+=150,y},{x=75,y}});
        primitives.append(primitive);
        break;
    case Quartercircle:
        primitive = new Primitive(pen, Primitive::Quartercircle,{{pos,{x+=150,y+=150},{x=150,y}}});
        primitives.append(primitive);
        break;
    case Parabola:
        primitive = new Primitive(pen, Primitive::BezierCurve,{pos,{x,y-300},{x+150,y-300},{x+150,y}});
        primitives.append(primitive);
        break;
    case Curve:
        primitive = new Primitive(pen, Primitive::Curve,{pos,{x,y+=100},{x+=70,y-10}});
        primitives.append(primitive);
        break;
    case Curverow:
        primitive = new Primitive(pen, Primitive::Curverow,{pos,{x-=25,y+=20},{x+=70,y},{x-=25,y-=20}});
        primitives.append(primitive);
        break;
    case Row:
        primitive = new Primitive(pen, Primitive::Row,{pos,{x+70,y},{x+60,y-5},{x+60,y+5}});
        primitives.append(primitive);
        break;
    case DashRow:
        primitive = new Primitive(pen, Primitive::DashedRow,{pos,{x+70,y},{x+60,y-5},{x+60,y+5}});
        primitives.append(primitive);
        break;
    case Masspoint:
        primitive = new Primitive(pen, Primitive::Masspoint,{pos,{x=10,y}});
        primitives.append(primitive);
        break;
    case Arctrack:
        primitive = new Primitive(pen, Primitive::Arctrack,{pos,{x,y+30},{x+60,y+30},{x+60,y},{x+75,y},{x+75,y+35},{x-15,y+35},{x-15,y}});
        primitives.append(primitive);
        break;
    case Barriertrack:
        primitive = new Primitive(pen, Primitive::Barriertrack,{pos,{x+=60,y}});
        primitives.append(primitive);
        break;
    case Belt:
        primitive = new Primitive(pen, Primitive::Belt,{pos,{x+=80,y},{x=10,y}});
        primitives.append(primitive);
        break;
    case Track:
        primitive = new Primitive(pen, Primitive::Track,{pos,{x+=100,y}});
        primitives.append(primitive);
        break;
    case Ltrack:
        primitive = new Primitive(pen, Primitive::Ltrack,{pos,{x+=100,y}});
        primitives.append(primitive);
        break;
    case Spring:
        primitive = new Primitive(pen, Primitive::Spring,{pos,{x+15,y},{x+20,y-5},{x+25,y+5},{x+30,y-5},{x+35,y+5},{x+40,y-5},{x+45,y+5},
                                                           {x+50,y-5},{x+55,y+5},{x+60,y-5},{x+65,y+5},{x+70,y-5},{x+75,y+5},{x+80,y},{x+95,y}});
        primitives.append(primitive);
        break;
    case Weight:
        primitive = new Primitive(pen, Primitive::Weight,{pos,{x-=10,y-=10},{x+=15,y-=10},{x+=15,y+=10},{x-=10,y+=10},{x-=5,y},{x,y+=5},{x,y+=5}});
        primitives.append(primitive);
        break;
    case Weight2:
        primitive = new Primitive(pen, Primitive::Weight2,{pos,{x+=10,y+=10},{x-=15,y+=10},{x-=15,y-=10},{x+=10,y-=10},{x+=5,y},{x,y-=5},{x,y-=5}});
        primitives.append(primitive);
        break;
    case Weight3:
        primitive = new Primitive(pen, Primitive::Weight3,{pos,{x-=10,y-=10},{x+=15,y-=10},{x+=15,y+=10},{x-=10,y+=10},{x-=5,y},{x,y+=5},{x,y+=5},
                                                          {x+=5,y+=40},{x+=10,y+=10},{x-=15,y+=10},{x-=15,y-=10},{x+=10,y-=10},{x+=5,y},{x,y-=5},{x,y-=5}});
        primitives.append(primitive);
        break;
    case Bullet:
        primitive = new Primitive(pen, Primitive::Bullet,{pos,{x+=15,y+=5},{x-=15,y+=5}});
        primitives.append(primitive);
        break;
    case Block:
        primitive = new Primitive(pen, Primitive::Block,{pos,{x+=25,y},{x,y+=25},{x-=25,y},{x,y-=25}});
        primitives.append(primitive);
        break;
    case Skateboard:
        primitive = new Primitive(pen, Primitive::Skateboard,{{pos,{x+=80,y},{x=10,y}}});
        primitives.append(primitive);
        break;
    case Pallet:
        primitive = new Primitive(pen, Primitive::Pallet,{{pos,{x+=70,y},{x-=20,y+=10},{x-=30,y}}});
        primitives.append(primitive);
        break;
    case Car:
        primitive = new Primitive(pen, Primitive::Car,{{pos,{x+=60,y},{x=10,y},{x=5,y}}});
        primitives.append(primitive);
        break;
    case Plane:
        primitive = new Primitive(pen, Primitive::Plane,{{pos,{x-=10,y+=15},{x,y+=50},{x+=20,y},{x,y-=50},
                                                           {x,y+=10},{x+=30,y+=20},{x,y+=15},{x-=30,y-=20},
                                                           {x-=20,y-=15},{x-=30,y+=20},{x,y+=15},{x+=30,y-=20},
                                                           {x,y+=25},{x-=10,y+=10},{x+=40,y},{x-=10,y-=10}  }});
        primitives.append(primitive);
        break;
    }
    update();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    QPoint pos = event->pos();
    pos.rx() -= 100;
    pos.ry() -= 75;
    QVector<QPoint> args;
    switch (state)
    {
    case DashedLine:
        primitive->setArgs({points[0], pos});
        break;
    case Line:
        primitive->setArgs({points[0], pos});
        break;
    case Line2:
        primitive->setArgs({points[0], pos});
        break;
    case Triangle:
        primitive->setArgs({pos,{(points[0].x() + pos.x()) / 2, points[0].y()},{points[0].x(), pos.y()}});
        break;
    case Circle:
        if (qAbs(pos.x() - points[0].x()) < qAbs(pos.y() - points[0].y()))
            if (pos.ry() > points[0].y())
                pos.ry() = points[0].y() + qAbs(pos.rx() - points[0].x());
            else
                pos.ry() = points[0].y() - qAbs(pos.rx() - points[0].x());
        else
            if (pos.rx() > points[0].x())
                pos.rx() = points[0].x() + qAbs(pos.ry() - points[0].y());
            else
                pos.rx() = points[0].x() - qAbs(pos.ry() - points[0].y());
        primitive->setArgs({(pos + points[0]) / 2,(pos - points[0]) / 2});
        break;
    case Ellipse:
        primitive->setArgs({(pos + points[0]) / 2,(pos - points[0]) / 2});
        break;
    case Polygon:
        break;
    case Rectangle:
        primitive->setArgs({points[0],{points[0].x(), pos.y()},pos,{pos.x(), points[0].y()}});
        break;
    case BezierCurve:
        break;
    case BezierControl:
        if (!primitive)
            break;
        args = primitive->translateArg(selectedControlPoint,pos - points[0]);
        primitive->setPoints(args);
        break;
    case BSplineCurve:
        break;
    case ARC:
        break;
    case Translate:
        if (!primitive)
            break;
        args = primitive->translate(pos - points[0]);
        primitive->setPoints(args);
        break;
    case ClipLine:
        primitive->setArgs({points[0],{points[0].x(), pos.y()},pos,{pos.x(), points[0].y()}});
        foreach (Primitive *p, primitives)
        {
            args = primitive->args();
            args = p->CohenSutherlandClip(args[0], args[2]);
            p->setPoints(args);
        }
        break;
    case ClipLine2:
        primitive->setArgs({points[0],{points[0].x(), pos.y()},pos,{pos.x(), points[0].y()}});
        break;
    case Fill:
        break;
    case ZoomIn:
        break;
    case ZoomOut:
        break;
    case Delete:
        break;
    case SetRP:
        break;
    case DeleteRP:
        break;
    case Rotate:
        if (!primitive)
            break;
        if(isReferencePointSet){
            QPoint a = points[0] - referencePoint, b = pos - referencePoint;
            qreal product = a.x() * b.y() - a.y() * b.x();
            qreal aNorm = qSqrt(qreal(a.x() * a.x() + a.y() * a.y()));
            qreal bNorm = qSqrt(qreal(b.x() * b.x() + b.y() * b.y()));
            args = primitive->rotate(referencePoint,qAsin(product / aNorm / bNorm));
        }
        else{
            QPoint a = points[0] - primitive->center(), b = pos - primitive->center();
            qreal product = a.x() * b.y() - a.y() * b.x();
            qreal aNorm = qSqrt(qreal(a.x() * a.x() + a.y() * a.y()));
            qreal bNorm = qSqrt(qreal(b.x() * b.x() + b.y() * b.y()));
            args = primitive->rotate(primitive->center(),qAsin(product / aNorm / bNorm));
        }
        primitive->setPoints(args);
        break;
    case Surface:
        break;
    }

    update();
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    QPoint pos = event->pos();
    pos.rx() -= 100;
    pos.ry() -= 75;
    QVector<QPoint> args;
    switch (state)
    {
    case DashedLine:
        primitive->setArgs({points[0], pos});
        break;
    case Line:
        primitive->setArgs({points[0], pos});
        break;
    case Line2:
        primitive->setArgs({points[0], pos});
        break;
    case Triangle:
        primitive->setArgs({pos,{(points[0].x() + pos.x()) / 2, points[0].y()},{points[0].x(), pos.y()}});
        break;
    case Circle:
        if (qAbs(pos.x() - points[0].x()) < qAbs(pos.y() - points[0].y()))
            if (pos.ry() > points[0].y())
                pos.ry() = points[0].y() + qAbs(pos.rx() - points[0].x());
            else
                pos.ry() = points[0].y() - qAbs(pos.rx() - points[0].x());
        else
            if (pos.rx() > points[0].x())
                pos.rx() = points[0].x() + qAbs(pos.ry() - points[0].y());
            else
                pos.rx() = points[0].x() - qAbs(pos.ry() - points[0].y());
        primitive->setArgs({(pos + points[0]) / 2,(pos - points[0]) / 2});
        break;
    case Ellipse:
        primitive->setArgs({(pos + points[0]) / 2,(pos - points[0]) / 2});
        break;
    case Polygon:
        primitive->setArgs(points);
        break;
    case Rectangle:
        primitive->setArgs({points[0],{points[0].x(), pos.y()},pos,{pos.x(), points[0].y()}});
        break;
    case BezierCurve:
        primitive->setArgs(points);
        break;
    case BezierControl:
        if (!primitive)
            break;
        args = primitive->translateArg(selectedControlPoint,pos - points[0]);
        primitive->setArgs(args);
        break;
    case BSplineCurve:
        primitive->setArgs(points);
        break;
    case ARC:
        break;
    case Translate:
        if (!primitive)
            break;
        args = primitive->translate(pos - points[0]);
        primitive->setArgs(args);
        break;
    case ClipLine:
        primitive->setArgs({points[0],{points[0].x(), pos.y()},pos,{pos.x(), points[0].y()}});
        foreach (Primitive *p, primitives)
        {
            args = primitive->args();
            args = p->CohenSutherlandClip(args[0], args[2]);
            p->setArgs(args);
        }
        delete primitive;
        primitive = nullptr;
        break;
    case Fill:
        break;
    case ClipLine2:
        points.append(pos);
        for (int i = 0; i < primitives.size(); ) {
            Primitive *p = primitives[i];
            if (p->type() == Primitive::Line || p->type() == Primitive::Line2) {
                QVector<QPoint> clippedPoints = p->MidpointSubdivisionClip(points.first(), points.back());
                if (!clippedPoints.empty()) {
                    p->setPoints(clippedPoints);
                    ++i;
                }
                else {
                    delete p;
                    primitives.removeAt(i);
                }
            } else {
                ++i;
            }
        }
        delete primitive;
        primitive = nullptr;
        break;
    case ZoomIn:
        if (!primitive) break;
        if(isReferencePointSet)
        {
            args = primitive->scale(referencePoint,1.2);
        }
        else
        {
            args = primitive->scale(primitive->center(),1.2);
        }
        primitive->setArgs(args);
        break;
    case ZoomOut:
        if (!primitive) break;
        if(isReferencePointSet)
        {
            args = primitive->scale(referencePoint,0.9);
        }
        else
        {
            args = primitive->scale(primitive->center(),0.9);
        }
        primitive->setArgs(args);
        break;
    case Delete:
        primitives.removeAll(primitive);
        delete primitive;
        break;
    case SetRP:
        break;
    case DeleteRP:
        break;
    case Rotate:
        if (!primitive)
            break;
        if(isReferencePointSet){
            QPoint a = points[0] - referencePoint, b = pos - referencePoint;
            qreal product = a.x() * b.y() - a.y() * b.x();
            qreal aNorm = qSqrt(qreal(a.x() * a.x() + a.y() * a.y()));
            qreal bNorm = qSqrt(qreal(b.x() * b.x() + b.y() * b.y()));
            args = primitive->rotate(referencePoint,qAsin(product / aNorm / bNorm));
        }
        else{
            QPoint a = points[0] - primitive->center(), b = pos - primitive->center();
            qreal product = a.x() * b.y() - a.y() * b.x();
            qreal aNorm = qSqrt(qreal(a.x() * a.x() + a.y() * a.y()));
            qreal bNorm = qSqrt(qreal(b.x() * b.x() + b.y() * b.y()));
            args = primitive->rotate(primitive->center(),qAsin(product / aNorm / bNorm));
        }
        primitive->setArgs(args);
        break;
    case Surface:
        break;
    }
    update();
    if (state != BezierCurve && state != Polygon)
        points.clear();
}

void MainWindow::on_Save_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Image"), "", tr("Images (*.png *.xpm *.jpg)"));

    if (fileName.isEmpty())
        return;

    QImage outputImage = image;
    outputImage.save(fileName);
}


void MainWindow::on_SSurface_clicked()
{
    state = Surface;
    isBezierInProgress=false;
    isPolygonInProgress =false;
    points.clear();
}


void MainWindow::on_Translate_clicked()
{
    state = Translate;
    isBezierInProgress=false;
    isPolygonInProgress =false;
    points.clear();
}


void MainWindow::on_ZoomIn_clicked()
{
    state = ZoomIn;
    isBezierInProgress=false;
    isPolygonInProgress =false;
    points.clear();
}


void MainWindow::on_ZoomOut_clicked()
{
    state = ZoomOut;
    isBezierInProgress=false;
    isPolygonInProgress =false;
    points.clear();
}


void MainWindow::on_Ramp_clicked()
{
    state = Ramp;
    isBezierInProgress=false;
    isPolygonInProgress =false;
    points.clear();
}


void MainWindow::on_Semicircle_clicked()
{
    state =  Semicircle;
    isBezierInProgress=false;
    isPolygonInProgress =false;
    points.clear();
}

void MainWindow::on_quartercircle_clicked()
{
    state = Quartercircle;
    isBezierInProgress=false;
    isPolygonInProgress =false;
    points.clear();
}

void MainWindow::on_parabola_clicked()
{
    state =  Parabola;
    isPolygonInProgress =false;
    points.clear();
}




void MainWindow::on_curve_clicked()
{
    state =  BezierCurve;
    isBezierInProgress=false;
    isPolygonInProgress =false;
    points.clear();
}


void MainWindow::on_curverow_clicked()
{
    state =  Curverow;
    isBezierInProgress=false;
    isPolygonInProgress =false;
        points.clear();
}


void MainWindow::on_row_clicked()
{
    state = Row;
    isBezierInProgress=false;
    isPolygonInProgress =false;
    points.clear();
}

void MainWindow::on_masspoint_clicked()
{
    state = Masspoint;
    isBezierInProgress=false;
    isPolygonInProgress =false;
    points.clear();
}


void MainWindow::on_arctrack_clicked()
{
    state = Arctrack;
    isBezierInProgress=false;
    isPolygonInProgress =false;
    points.clear();
}


void MainWindow::on_barriertrack_clicked()
{
    state = Barriertrack;
    isBezierInProgress=false;
    isPolygonInProgress =false;
    points.clear();
}


void MainWindow::on_Belt_clicked()
{
    state = Belt;
    isBezierInProgress=false;
    isPolygonInProgress =false;
    points.clear();
}



void MainWindow::on_track_clicked()
{
    state = Track;
    isBezierInProgress=false;
    isPolygonInProgress =false;
    points.clear();
}


void MainWindow::on_Ltrack_clicked()
{
    state = Ltrack;
    isBezierInProgress=false;
    isPolygonInProgress =false;
    points.clear();
}


void MainWindow::on_spring_clicked()
{
    state = Spring;
    isBezierInProgress=false;
    isPolygonInProgress =false;
    points.clear();
}


void MainWindow::on_weight_clicked()
{
    state = Weight;
    isBezierInProgress=false;
    isPolygonInProgress =false;
    points.clear();
}


void MainWindow::on_bullet_clicked()
{
    state = Bullet;
    isBezierInProgress=false;
    isPolygonInProgress =false;
    points.clear();
}


void MainWindow::on_block_clicked()
{
    state = Block;
    isBezierInProgress=false;
    isPolygonInProgress =false;
    points.clear();
}


void MainWindow::on_skateboard_clicked()
{
    state = Skateboard;
    isBezierInProgress=false;
    isPolygonInProgress =false;
    points.clear();
}


void MainWindow::on_pallet_clicked()
{
    state = Pallet;
    isBezierInProgress=false;
    isPolygonInProgress =false;
    points.clear();
}



void MainWindow::on_car_clicked()
{
    state = Car;
    isBezierInProgress=false;
    isPolygonInProgress =false;
    points.clear();
}


void MainWindow::on_plane_clicked()
{
    state = Plane;
    isBezierInProgress=false;
    isPolygonInProgress =false;
    points.clear();
}


void MainWindow::on_Delete_clicked()
{
    state = Delete;
    isBezierInProgress=false;
    isPolygonInProgress =false;
    points.clear();
}


void MainWindow::on_BezierControl_clicked()
{
    state = BezierControl;
    isBezierInProgress=false;
    isPolygonInProgress =false;
    points.clear();
}


void MainWindow::on_Rectangle_clicked()
{
    state = Rectangle;
    isBezierInProgress=false;
    isPolygonInProgress =false;
    points.clear();
}


void MainWindow::on_Rotate_clicked()
{
    state = Rotate;
    isBezierInProgress=false;
    isPolygonInProgress =false;
    points.clear();
}


void MainWindow::on_DashRow_clicked()
{
    state = DashRow;
    isBezierInProgress=false;
    isPolygonInProgress =false;
    points.clear();
}


void MainWindow::on_Line_clicked()
{
    state = Line;
    isBezierInProgress=false;
    isPolygonInProgress =false;
    points.clear();
}


void MainWindow::on_DashedLine_clicked()
{
    state = DashedLine;
    isBezierInProgress=false;
    isPolygonInProgress =false;
    points.clear();
}


void MainWindow::on_weight2_clicked()
{
    state = Weight2;
    isBezierInProgress=false;
    isPolygonInProgress =false;
    points.clear();
}


void MainWindow::on_weight3_clicked()
{
    state = Weight3;
    isBezierInProgress=false;
    isPolygonInProgress =false;
    points.clear();
}


void MainWindow::on_Ramp30_clicked()
{
    state = Ramp30;
    isBezierInProgress=false;
    isPolygonInProgress =false;
    points.clear();
}

