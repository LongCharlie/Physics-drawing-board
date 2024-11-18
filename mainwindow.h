#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QBrush>
#include <QString>
#include <QUrl>
#include <QFileDialog>
#include "Primitive.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private slots:

    void on_Save_clicked();

    void on_Translate_clicked();

    void on_ZoomIn_clicked();

    void on_ZoomOut_clicked();

    void on_Delete_clicked();

    void on_SSurface_clicked();

    void on_Ramp_clicked();

    void on_Belt_clicked();

    void on_pallet_clicked();

    void on_skateboard_clicked();

    void on_arctrack_clicked();

    void on_plane_clicked();

    void on_car_clicked();

    void on_bullet_clicked();

    void on_track_clicked();

    void on_Ltrack_clicked();

    void on_barriertrack_clicked();

    void on_curve_clicked();

    void on_BezierControl_clicked();

    void on_block_clicked();

    void on_masspoint_clicked();

    void on_Rectangle_clicked();

    void on_Semicircle_clicked();

    void on_spring_clicked();

    void on_row_clicked();

    void on_weight_clicked();

    void on_parabola_clicked();

    void on_quartercircle_clicked();

    void on_curverow_clicked();

    void on_Rotate_clicked();

    void on_DashRow_clicked();

    void on_Line_clicked();

    void on_DashedLine_clicked();

    void on_weight2_clicked();

    void on_weight3_clicked();

    void on_Ramp30_clicked();

private:
    Ui::MainWindow *ui;
    enum State {DashedLine, Line, Line2, Triangle, Circle, Ellipse, Polygon, Rectangle, BSplineCurve,
                 BezierCurve ,BezierControl, ARC, Translate, ClipLine, ClipLine2, Fill, ZoomIn, ZoomOut,
                 Delete, Rotate, SetRP, DeleteRP ,Surface, Ramp,Ramp30,Semicircle,Quartercircle,Parabola,Curve,
                 Curverow,Row,DashRow,Masspoint,Arctrack,Barriertrack,Belt,Track,Ltrack,Spring ,Weight,Weight2,Weight3,Bullet,Block,
                 Skateboard,Pallet,Car,Plane} state;//程序状态
    QVector<QPoint> points;          //鼠标点击的位置
    QList<Primitive *> primitives;   //已经绘制的图元列表
    Primitive *primitive;            //当前正在操作的图元
    QImage image;                    //画布
    QPainter painter;                //画笔
    QPen pen;                        //笔的颜色和大小
    bool isBezierInProgress;   //Bezier是否正在绘制
    bool isPolygonInProgress;  //Polygon是否正在绘制
    QPoint selectedControlPoint;     //存储被选中的控制点(用于Bezier曲线控制)
    QPoint referencePoint;           //旋转 平移参考点
    bool isReferencePointSet;
    bool isBSplineInProgress;
};


#endif // MAINWINDOW_H
