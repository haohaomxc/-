#include "canttchartview.h"
#include "canttchartscene.h"

#include <QWheelEvent>

CanttChartView::CanttChartView(QWidget *parent) : QGraphicsView(parent)
{
    // 构造函数
    // 创建一个CanttChartScene对象并将其赋值给成员变量m_pScene
    m_pScene = new CanttChartScene(this);

    // 设置视图关联的场景为刚创建的CanttChartScene
    setScene(m_pScene);

    // 设置视图内部内容对齐方式，左上角对齐
    setAlignment(Qt::AlignLeft | Qt::AlignTop);

    // 设置拖动模式，用户可通过鼠标拖拽视图
    setDragMode(QGraphicsView::ScrollHandDrag);

    // 设置渲染提示，包括抗锯齿和文本抗锯齿
    setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    // 将视图中心点定位到原点(0, 0)
    centerOn(0, 0);
}

void CanttChartView::setAxisRange(const QDateTime &start, const QDateTime &end, const QStringList &platforms)
{
    m_pScene->setAxisRange(start, end, platforms);
}

CanttTimeBarItem *CanttChartView::addPlanTimeBar(const QString &platform, const QDateTime &start, const QDateTime &end)
{
    return m_pScene->addPlanTimeBar(platform, start, end);
}

CanttTimeBarItem *CanttChartView::addRealTimeBar(const QString &platform, const QDateTime &start, const QDateTime &end)
{
    return m_pScene->addRealTimeBar(platform, start, end);
}

void CanttChartView::setStepTimeValue(const QTime &time)
{
    m_pScene->setStepTimeValue(time);
}

void CanttChartView::wheelEvent(QWheelEvent *event)
{
    if (event->delta() > 0)
    {
        zoomOut();
    }
    else
    {
        zoomIn();
    };
}

void CanttChartView::zoomIn()
{
    scaleBy(1.1);
}

void CanttChartView::zoomOut()
{
    scaleBy(1.0 / 1.1);
}

void CanttChartView::scaleBy(double factor)
{
    scale(factor, factor);
}
