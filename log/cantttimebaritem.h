#ifndef CANTTTIMEBARITEM_H
#define CANTTTIMEBARITEM_H

#include <QGraphicsRectItem>
#include <QDateTime>
#include "definition.h"
#include "mainwindow.h"

class CanttTimeBarItem : public QGraphicsRectItem
{
public:
    enum {Type = canttTimeBarType};
    enum TimeType {
        PlanTime,
        RealTime
    };
    enum EventType {
        AnshiEvent,
        TiqianEvent,
        TuichiEvent,
        NonEvent
    };

    explicit CanttTimeBarItem(const QDateTime &start, const QDateTime &end, TimeType type, double factor);

    void addEvent(const QDateTime &dateTime, CanttTimeBarItem::EventType type, const QString &taskId);

private:
//    QGraphicsItem *createEventItem(EventType type);
//    QGraphicsItem *createEventItem(EventType type, quint32 eventId);
    QGraphicsItem *createEventItem(CanttTimeBarItem::EventType type, const QString &taskId);
private:
    double m_pFactor;

    QDateTime m_startDateTime;
    QDateTime m_endDateTime;
};

#endif // CANTTTIMEBARITEM_H
