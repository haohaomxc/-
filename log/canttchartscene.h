#ifndef CANTTCHARTSCENE_H
#define CANTTCHARTSCENE_H

#include <QGraphicsScene>
#include <QDateTime>
#include <QTime>
#include <QHash>
#include <QPair>

class CanttTimeBarItem;
class CanttChartScene : public QGraphicsScene
{
    Q_OBJECT
public:    
    explicit CanttChartScene(QObject *parent = 0);

    void setAxisRange(const QDateTime &start, const QDateTime &end, const QStringList &platforms);

    CanttTimeBarItem *addPlanTimeBar(const QString &platform, const QDateTime &start, const QDateTime &end);
    CanttTimeBarItem *addRealTimeBar(const QString &platform, const QDateTime &start, const QDateTime &end);

    void setStepTimeValue(const QTime &time);

private:
    void drawGridLines();
    void drawVerticalAxis(const QStringList &platforms);

    int m_rowCount;
    int m_columnCount;

    QDateTime m_startDateTime;
    QDateTime m_endDateTime;
    QTime m_stepTimeValue;
    QStringList m_platforms;

    int m_firstTimeBarStartX;
    int m_firstTimeBarStartY;
    double m_perPixelHMsecs;

    QHash<QString, double> m_platformStartYHash;
    QHash<QString, QPair<QDateTime, QDateTime>> m_planTimeBarTemp;
    QHash<QString, QPair<QDateTime, QDateTime>> m_realTimeBarTemp;
    QMultiHash<QString, QGraphicsItem*> m_plaformTimeBarHash;
};

#endif // CANTTCHARTSCENE_H
