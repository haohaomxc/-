#include "cantttimebaritem.h"
#include "definition.h"

#include <QBrush>
#include <QPen>
#include <QCursor>
#include <QPoint>
#include <QLabel>
#include <QGraphicsProxyWidget>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>
#include <QSqlError>
#include <QSqlQueryModel>
#include "mainwindow.h"

const int eventItemYOffset = 2;//设置偏移量

// CanttTimeBarItem 类的构造函数，接受四个参数：
// - start: 表示时间条起始时间的 QDateTime 对象
// - end: 表示时间条结束时间的 QDateTime 对象
// - type: 表示时间条类型的枚举值 TimeType
// - factor: 控制时间与图形宽度比例的浮点数
CanttTimeBarItem::CanttTimeBarItem(const QDateTime &start, const QDateTime &end, TimeType type, double factor)
    : QGraphicsRectItem(nullptr), // 继承自 QGraphicsRectItem，并将其父项设置为 nullptr（即顶级项）
      m_pFactor(factor), // 将传入的 factor 存储到成员变量 m_pFactor 中
      m_startDateTime(start), // 将传入的起始时间存储到成员变量 m_startDateTime 中
      m_endDateTime(end) // 将传入的结束时间存储到成员变量 m_endDateTime 中
{
    // 计算时间间隔的毫秒数并除以 factor 得到矩形宽度
    double width = (end.toMSecsSinceEpoch() - start.toMSecsSinceEpoch()) / m_pFactor;

    // 设置矩形区域的位置和大小：左下角坐标为 (0, 0)，宽度为计算得到的宽度，高度为canttTimeBarHeight
    setRect(0, 0, width, canttTimeBarHeight);

    // 设置鼠标悬停在该矩形上方时显示的手型光标
    setCursor(QCursor(Qt::PointingHandCursor));

    // 根据时间条类型来设置填充颜色
    if (CanttTimeBarItem::PlanTime == type) // 如果类型为 PlanTime
    {
        setBrush(QBrush(QColor(92, 201, 221))); // 设置背景色为浅蓝色
    }
    else // 否则（类型为其他）
    {
        setBrush(QBrush(QColor(233, 252, 74))); // 设置背景色为亮绿色
    }

    // 创建一个无边框的画笔
    QPen pen;
    pen.setStyle(Qt::NoPen);

    // 应用该无边框画笔给当前的CanttTimeBarItem对象
    setPen(pen);
}


// CanttTimeBarItem 类的一个成员函数，用于在时间条上添加一个事件标记
void CanttTimeBarItem::addEvent(const QDateTime &dateTime, CanttTimeBarItem::EventType type, const QString &taskId)
{
    // 检查待添加事件的时间是否在当前时间条的范围内
    if (dateTime < m_startDateTime || dateTime > m_endDateTime)
    {
        // 若不在范围内，则不执行任何操作并直接返回
        return;
    }

    // 创建一个与事件类型对应的 QGraphicsItem 子对象，作为事件标记
    QGraphicsItem *item = createEventItem(type, taskId);

    // 计算事件时间点在时间条上的横坐标位置
    double x = (dateTime.toMSecsSinceEpoch() - m_startDateTime.toMSecsSinceEpoch()) / m_pFactor;

    // 设置事件标记的二维坐标位置
    item->setPos(x, eventItemYOffset);
}

// 修改createEventItem函数签名，接受QString类型的taskId
QGraphicsItem *CanttTimeBarItem::createEventItem(CanttTimeBarItem::EventType type, const QString &taskId)
{
    QSqlQuery query(MainWindow::db);
    QString sql = QStringLiteral("SELECT 地点, 工具, 备注 FROM 1111111_renwu WHERE taskId = ?");
    query.prepare(sql);
    query.addBindValue(taskId);

    if (!query.exec()) {
        qDebug() << "Error executing SQL query: " << query.lastError().text();
        return nullptr;
    }

    QLabel *label = new QLabel;
    // 初始化样式表和尺寸属性...

    QString tooltipText;
    if (query.next()) {
        QString location = query.value(0).toString();
        QString tools = query.value(1).toString();
        QString notes = query.value(2).toString();

        // 设置动态生成的工具提示文本
        tooltipText = QStringLiteral("地点: %1\n工具: %2\n备注: %3").arg(location, tools, notes);
        label->setToolTip(tooltipText);
    } else {
        qDebug() << "No record found for the given task ID: " << taskId;
        // 可选：设置默认工具提示或其他默认行为
    }

    // 根据事件类型设置文本内容
    switch (type)
    {
    case CanttTimeBarItem::AnshiEvent:
        label->setText(QStringLiteral("预期按时"));
        break;
    case CanttTimeBarItem::TiqianEvent:
        label->setText(QStringLiteral("预期提前"));
        break;
    case CanttTimeBarItem::TuichiEvent:
        label->setText(QStringLiteral("预期推迟"));
        break;
    case CanttTimeBarItem::NonEvent:
        label->setText(QStringLiteral("预期未完成"));
        break;
    default:
        break;
    }

    // 确保当没有查询结果时，也至少有默认文本或为空
    if (tooltipText.isEmpty()) {
        // 可选：设置默认文本或删除已有的工具提示
    }

    // 创建并返回 QGraphicsProxyWidget
    QGraphicsProxyWidget *proxy = new QGraphicsProxyWidget(this);
    proxy->setWidget(label);
    return proxy;
}


























// CanttTimeBarItem 类的一个成员函数，用于创建一个表示特定事件类型的图形项
//QGraphicsItem *CanttTimeBarItem::createEventItem(CanttTimeBarItem::EventType type)
//{
//    // 创建一个 QLabel 对象作为基础事件标记
//    QLabel *label = new QLabel;

//    // 设置 QLabel 的样式表，以定义其外观特征
//    label->setStyleSheet("QLabel {"
//                         "background-color: transparent;"
//                         "min-height: 12px;"
//                         "max-height: 12px;"
//                         "font-size: 11px;"
//                         "padding-left: -2px;"
//                         "border-width: 0 0 0 12;"
//                         "border-image: url(:/img/event/takeoff.png) 0 0 0 64;}");

//    // 设置 QLabel 的工具提示文本，此处以起飞事件为例
//    //label->setToolTip(QStringLiteral("开始起飞\n人员：张三\n地点：xxx根据地"));

//    QSqlQuery query;

//    QString location = query.value(1).toString();
//    QString tools = query.value(3).toString();
//    QString notes = query.value(4).toString();

//    // 设置动态生成的工具提示文本
//    QString tooltipText = QStringLiteral("地点: %1\n工具: %2\n备注: %3").arg(location, notes);
//    label->setToolTip(tooltipText);

//    // 根据传入的事件类型设置 QLabel 的文本内容
//    switch (type)
//    {
//    case CanttTimeBarItem::AnshiEvent:
//        label->setText(QStringLiteral("预期按时"));
//        break;
//    case CanttTimeBarItem::TiqianEvent:
//        label->setText(QStringLiteral("预期提前"));
//        break;
//    case CanttTimeBarItem::TuichiEvent:
//        label->setText(QStringLiteral("预期推迟"));
//        break;
//    case CanttTimeBarItem::NonEvent:
//        label->setText(QStringLiteral("预期未完成"));
//        break;
//    default:
//        break;
//    }

//    // 创建一个 QGraphicsProxyWidget 对象，将 QLabel 包装成可添加到 QGraphicsScene 中的图形项
//    QGraphicsProxyWidget *proxy = new QGraphicsProxyWidget(this);

//    // 将之前创建的 QLabel 设置为 QGraphicsProxyWidget 的小部件
//    proxy->setWidget(label);

//    // 返回封装后的 QGraphicsProxyWidget 图形项
//    return proxy;
//}
