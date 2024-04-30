#ifndef CANTTCHARTVIEW_H
#define CANTTCHARTVIEW_H

#include <QGraphicsView>
#include <QDateTime>

class CanttChartScene;
class CanttTimeBarItem;
class CanttChartView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit CanttChartView(QWidget *parent = 0);

    void setAxisRange(const QDateTime &start, const QDateTime &end, const QStringList &platforms);

    CanttTimeBarItem *addPlanTimeBar(const QString &platform, const QDateTime &start, const QDateTime &end);
    CanttTimeBarItem *addRealTimeBar(const QString &platform, const QDateTime &start, const QDateTime &end);

    void setStepTimeValue(const QTime &time);

protected:
    virtual void wheelEvent(QWheelEvent *) override;

private slots:
    void zoomIn();
    void zoomOut();

private:
    void scaleBy(double factor);

private:
    CanttChartScene *m_pScene;
};

#endif // CANTTCHARTVIEW_H
