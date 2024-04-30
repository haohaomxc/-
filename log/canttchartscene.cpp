#include "canttchartscene.h"
#include "definition.h"
#include "cantttimebaritem.h"

#include <QBrush>
#include <QPen>
#include <QGraphicsLineItem>
#include <QGraphicsTextItem>
#include <QDebug>
#include <QCheckBox>
#include <QGraphicsProxyWidget>
#include <QCursor>

const int firstHorizantalGridWidth = 140;
const int horizontalGridWidth = 40;
const int verticalGridHeight = 40;
const int horizontalAxisTextHeight = 21;
const int horizontalAxisTextOffset = 5;
const QPoint axisStartPoint = QPoint(20, 40);
const QPoint platformHeaderOffset = QPoint(6, 10);
const QColor gridLineColor = QColor(48, 85, 93);
const QColor scaleDateColor = QColor(253, 201, 115);
const QColor scaleTimeColor = QColor(208, 216, 237);

CanttChartScene::CanttChartScene(QObject *parent) : QGraphicsScene(parent),
    m_rowCount(0), m_columnCount(0), m_stepTimeValue(0, 30)//时间间隔
{
    setBackgroundBrush(QBrush(QColor(43, 48, 54)));//背景颜色设为灰色

    m_perPixelHMsecs =  m_stepTimeValue.msecsSinceStartOfDay() / (double)horizontalGridWidth;
}

void CanttChartScene::setAxisRange(const QDateTime &start, const QDateTime &end, const QStringList &platforms)
{
    if (start >= end || 0 == platforms.count())
    {
        return;
    }

    // 根据提供的平台列表更新场景中的行数（代表不同平台的数量）
      m_rowCount = platforms.count();

      // 设置图表的时间范围起始点与终止点
      m_startDateTime = start;
      m_endDateTime = end;

      // 存储传入的平台列表
      m_platforms = platforms;

      // 计算第一条时间条（时间轴的第一个时间单元）的起始坐标：
      // x 轴坐标从 axisStartPoint 的 x 坐标值开始，加上第一个水平网格的宽度
      m_firstTimeBarStartX = axisStartPoint.x() + firstHorizantalGridWidth;

      // y 轴坐标则保持与 axisStartPoint 的 y 坐标相同
      m_firstTimeBarStartY = axisStartPoint.y();
    //清空现有图形项
      clear();

      // 根据时间范围和平台数量计算场景所需的最小宽度和高度
      double sceneMiniWidth = m_firstTimeBarStartX + horizontalGridWidth +
                             (end.toMSecsSinceEpoch() - start.toMSecsSinceEpoch()) / m_perPixelHMsecs;
      double sceneMiniHeight = m_firstTimeBarStartY + platforms.count() * verticalGridHeight;

      // 设置场景区域大小
      setSceneRect(0, 0, sceneMiniWidth, sceneMiniHeight + 800);

      // 绘制垂直轴（可能包括平台标签等）
      drawVerticalAxis(platforms);

      // 初始化绘图时的时间变量
      QDateTime startDateTime = start;
      QDate startDate = start.date();

      // 初始化水平方向上的绘图位置
      double x = m_firstTimeBarStartX;

      // 遍历整个时间范围，在每个水平网格的位置上添加时间标签
      for (; x <= sceneMiniWidth; x += horizontalGridWidth)
      {
          // 创建一个表示小时和分钟的时间标签项
          QGraphicsTextItem *timeItem = new QGraphicsTextItem(startDateTime.toString("HH:mm"));
          timeItem->setDefaultTextColor(scaleTimeColor);
          timeItem->setZValue(std::numeric_limits<int>::min());
          timeItem->setPos(x - horizontalAxisTextOffset, axisStartPoint.y() - horizontalAxisTextHeight);
          addItem(timeItem);

          // 如果在第一个时间单位处，添加日期标签项
          if (x == axisStartPoint.x() + firstHorizantalGridWidth)
          {
              QGraphicsTextItem *dateItem = new QGraphicsTextItem(startDateTime.date().toString("yyyy-MM-dd"));
              dateItem->setDefaultTextColor(scaleDateColor);
              dateItem->setZValue(std::numeric_limits<int>::min());
              addItem(dateItem);
              dateItem->setPos(x - horizontalAxisTextOffset, axisStartPoint.y() - horizontalAxisTextHeight * 2);
          }
          // 其他时间单位，仅当日期变化时才添加新的日期标签项
          else
          {
              if (startDateTime.date() > startDate)
              {
                  QGraphicsTextItem *dateItem = new QGraphicsTextItem(startDateTime.date().toString("yyyy-MM-dd"));
                  dateItem->setDefaultTextColor(scaleDateColor);
                  dateItem->setZValue(std::numeric_limits<int>::min());
                  addItem(dateItem);
                  dateItem->setPos(x - horizontalAxisTextOffset, axisStartPoint.y() - horizontalAxisTextHeight * 2);
                  startDate = startDateTime.date();
              }
          }

          // 移动到下一个时间单位
          startDateTime = startDateTime.addMSecs(m_stepTimeValue.msecsSinceStartOfDay());

          // 增加列计数器
          m_columnCount++;

          // 若已超过当前日期和时间，则停止绘制
          if (startDateTime > QDateTime::currentDateTime())
          {
              break;
          }
      }

      // 绘制网格线
      drawGridLines();

      // 扩展场景矩形尺寸，为额外空间留出余地
      QRectF rect = this->sceneRect();
      setSceneRect(0, 0, rect.width() + 200, rect.height() + 200);
}

// 定义CanttChartScene类的成员函数drawVerticalAxis，该函数接收一个平台名称列表（QStringList类型）作为参数
void CanttChartScene::drawVerticalAxis(const QStringList &platforms)
{
    // 如果平台列表为空，则不执行任何操作并直接返回
    if (platforms.count() == 0)
    {
        return;
    }

    // 获取当前场景的最大垂直坐标值（对应于场景高度）
    const double maxY = this->height();

    // 遍历垂直坐标轴，从axisStartPoint的y坐标开始，每次增加verticalGridHeight的距离
    int index = 0;
    for (double y = axisStartPoint.y(); y <= maxY; y += verticalGridHeight)
    {
        // 如果当前索引超过了平台列表的最后一个元素，跳出循环
        if (index > platforms.count() - 1)
        {
            break;
        }

        // 创建一个新的QCheckBox对象，并设置其对象名及样式表
        QCheckBox *box = new QCheckBox;
        box->setObjectName("PlatformCheckBox");
        box->setStyleSheet(
            "#PlatformCheckBox {"
            "color: rgb(205, 218, 235);"
            "background-color: rgb(43, 48, 54);"
            "}"
            "#PlatformCheckBox::indicator:unchecked {"
            "border-image: url(:/img/checkbox/timebar-show.png) 0 0 0 0 stretch;"
            "}"
            "#PlatformCheckBox::indicator:checked {"
            "border-image: url(:/img/checkbox/timebar-hide.png) 0 0 0 0 stretch;"
            "}"
        );

        // 连接复选框的点击信号到槽函数，处理显示或隐藏与平台相关的图表项
        connect(box, &QCheckBox::clicked, [=](bool checked) {
            // 获取与当前平台对应的图形项列表
            auto list = m_plaformTimeBarHash.values(box->text());

            // 如果复选框被选中，则隐藏对应的图形项
            if (checked)
            {
                for (QGraphicsItem *item : list)
                {
                    item->hide();
                }
            }
            // 否则，当复选框未被选中时，显示对应的图形项
            else
            {
                for (QGraphicsItem *item : list)
                {
                    item->show();
                }
            }
        });

        // 设置复选框的文本为当前索引指向的平台名称
        box->setText(platforms.at(index));

        // 使用addWidget方法将复选框作为一个图形代理小部件加入到场景中
        QGraphicsProxyWidget *proxy = addWidget(box);

        // 设置代理小部件的光标为手型
        proxy->setCursor(QCursor(Qt::PointingHandCursor));

        // 设置代理小部件的位置，使其位于垂直轴上的特定位置，加上偏移量platformHeaderOffset
        proxy->setPos(QPoint(axisStartPoint.x(), y) + platformHeaderOffset);

        // 将平台名称及其在场景中的起始Y坐标存储到哈希表m_platformStartYHash中
        m_platformStartYHash.insert(platforms.at(index), y);

        // 索引递增，准备处理下一个平台
        index++;
    }
}




// 定义CanttChartScene类的成员函数addPlanTimeBar，此函数接收三个参数：
// 1. QString类型的platform（平台名称）
// 2. QDateTime类型的start（时间段开始时间）
// 3. QDateTime类型的end（时间段结束时间）

CanttTimeBarItem *CanttChartScene::addPlanTimeBar(const QString &platform, const QDateTime &start, const QDateTime &end)
{
    // 检查传入的平台名称是否存在于已知平台起始Y坐标的哈希表m_platformStartYHash中
    if (!m_platformStartYHash.keys().contains(platform))
    {
        // 若不存在，则返回空指针nullptr，表示无法为该平台添加时间条
        return nullptr;
    }

    // 将时间段（start-end）以键值对的形式插入临时缓存m_planTimeBarTemp，关联到指定平台
    auto pair = qMakePair(start, end);
    m_planTimeBarTemp.insert(platform, pair);

    // 创建一个新的CanttTimeBarItem实例，使用给定的开始时间和结束时间，以及类型CanttTimeBarItem::PlanTime和每像素代表的时间（毫秒）
    CanttTimeBarItem *item = new CanttTimeBarItem(start, end, CanttTimeBarItem::PlanTime, m_perPixelHMsecs);

    // 计算时间条在场景中的X坐标，基于时间刻度转换为像素坐标
    double x = m_firstTimeBarStartX + (start.toMSecsSinceEpoch() - m_startDateTime.toMSecsSinceEpoch()) / m_perPixelHMsecs;

    // 计算时间条在场景中的Y坐标，使用平台起始Y坐标加3个单位（留出一定的空间）
    double y = m_platformStartYHash.value(platform) + 3;

    // 将新创建的时间条图形项添加到场景中
    addItem(item);

    // 设置时间条在场景中的具体位置
    item->setPos(x, y);

    // 将创建好的时间条图形项与其对应的平台名称关联，并插入到m_plaformTimeBarHash哈希表中
    m_plaformTimeBarHash.insert(platform, item);

    // 返回创建成功的时间条图形项指针
    return item;
}



// 定义CanttChartScene类的成员函数addRealTimeBar，该函数接收三个参数：
// 1. QString类型的platform（平台名称）
// 2. QDateTime类型的start（实时时间段开始时间）
// 3. QDateTime类型的end（实时时间段结束时间）

CanttTimeBarItem *CanttChartScene::addRealTimeBar(const QString &platform, const QDateTime &start, const QDateTime &end)
{
    // 检查传入的平台名称是否存在于已知平台起始Y坐标的哈希表m_platformStartYHash中
    if (!m_platformStartYHash.keys().contains(platform))
    {
        // 若不存在，则返回空指针nullptr，表示无法为该平台添加实时时间条
        return nullptr;
    }

    // 将实时时间段（start-end）以键值对的形式插入临时缓存m_realTimeBarTemp，关联到指定平台
    auto pair = qMakePair(start, end);
    m_realTimeBarTemp.insert(platform, pair);

    // 创建一个新的CanttTimeBarItem实例，用于表示实时时间条，使用给定的开始时间和结束时间，以及类型CanttTimeBarItem::RealTime和每像素代表的时间（毫秒）
    CanttTimeBarItem *item = new CanttTimeBarItem(start, end, CanttTimeBarItem::RealTime, m_perPixelHMsecs);

    // 计算时间条在场景中的X坐标，基于时间刻度转换为像素坐标
    double x = m_firstTimeBarStartX + (start.toMSecsSinceEpoch() - m_startDateTime.toMSecsSinceEpoch()) / m_perPixelHMsecs;

    // 计算时间条在场景中的Y坐标，这里相对于平台起始Y坐标增加了时间条高度（canttTimeBarHeight）及额外的6个单位（可能用于间隔或装饰）
    double y = m_platformStartYHash.value(platform) + canttTimeBarHeight + 6;

    // 将新创建的实时时间条图形项添加到场景中
    addItem(item);

    // 设置时间条在场景中的具体位置
    item->setPos(x, y);

    // 将创建好的实时时间条图形项与其对应的平台名称关联，并插入到m_plaformTimeBarHash哈希表中
    m_plaformTimeBarHash.insert(platform, item);

    // 返回创建成功的实时时间条图形项指针
    return item;
}


void CanttChartScene::setStepTimeValue(const QTime &time)
{
    // 设置时间步长值为给定的QTime对象
    m_stepTimeValue = time;

    // 根据时间步长（一天中从午夜开始的毫秒数）和水平网格宽度来计算每像素所代表的时间（毫秒）
    m_perPixelHMsecs = m_stepTimeValue.msecsSinceStartOfDay() / (double)horizontalGridWidth;

#if 0
    // 如果起始日期时间为空、结束日期时间为空或者没有平台数据，则无需更新坐标轴范围
    if (m_startDateTime.isNull() || m_endDateTime.isNull() || 0 == m_platforms.count())
    {
        return;
    }

    // 否则，根据新的时间步长，重新计算图表的坐标轴范围，并更新图表布局
    setAxisRange(m_startDateTime, m_endDateTime, m_platforms);
#endif
}

void CanttChartScene::drawGridLines()
{
    const double maxY = this->height();
    const double maxX = m_firstTimeBarStartX + m_columnCount * horizontalGridWidth;

    //绘制第一条水平网格线
    QGraphicsLineItem *item = new QGraphicsLineItem(axisStartPoint.x(), axisStartPoint.y(), axisStartPoint.x(), maxY);
    item->setPen(QPen(gridLineColor));
    item->setZValue(std::numeric_limits<int>::min());
    addItem(item);

    //绘制水平网格线
    for (double x = axisStartPoint.x() + firstHorizantalGridWidth; x <= maxX; x += horizontalGridWidth)
    {
        QGraphicsLineItem *item = new QGraphicsLineItem(x, axisStartPoint.y(), x, maxY);
        item->setPen(QPen(gridLineColor));
        item->setZValue(std::numeric_limits<int>::min());
        addItem(item);
    }

    //绘制垂直网格线
    for (double y = axisStartPoint.y(); y <= maxY; y += verticalGridHeight)
    {
        QGraphicsLineItem *item = new QGraphicsLineItem(axisStartPoint.x(), y, maxX, y);
        item->setPen(QPen(gridLineColor));
        item->setZValue(std::numeric_limits<int>::min());
        addItem(item);
    }
}

