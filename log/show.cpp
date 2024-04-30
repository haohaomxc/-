#include "show.h"
#include "ui_show.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>
#include <QVariantList>
#include "canttchartview.h"
#include "cantttimebaritem.h"
#include "mainwindow.h"

// 确保MainWindow::db在主线程中已经正确初始化并可用

Show::Show(QWidget *parent) :
    MainWindow(parent),
    ui(new Ui::Show)
{
    ui->setupUi(this);

    // 确保MainWindow类的数据库连接已打开
    if (!MainWindow::db.isOpen()) {
        QMessageBox::critical(nullptr, "错误", "数据库连接尚未建立或已关闭，请确保在创建Show实例前已经成功连接数据库！");
        return;
    }

    CanttChartView *view = new CanttChartView(this);

    // 初始化任务ID列表
    QStringList tasksList;

    // 使用MainWindow类的静态数据库连接
    QSqlQuery query(MainWindow::db);

    // 修复SQL查询语句，确保列名正确无误
    query.prepare("SELECT 时间, 地点, taskId, 工具, 备注, 预期 FROM 1111111_renwu");

    if (!query.exec()) {
        QMessageBox::critical(this, "错误", "执行SQL查询失败：" + query.lastError().text());
        return;
    }

    while (query.next()) {
        QString timeRange = query.value(0).toString();
        QString taskId = query.value(2).toString();
        tasksList << taskId;
        QString expectation = query.value(5).toString();

        QStringList timeParts = timeRange.split("-");
        if (timeParts.size() != 2) {
            qDebug() << "无效的时间范围：" << timeRange;
            continue;
        }

        QDateTime startTime = QDateTime(QDate(2024, 4, 16), QTime::fromString(timeParts[0], "hh:mm"));
        QDateTime endTime = QDateTime(QDate(2024, 4, 16), QTime::fromString(timeParts[1], "hh:mm"));

        CanttTimeBarItem::EventType eventType;
        if (expectation == "预期完成") {
            eventType = CanttTimeBarItem::AnshiEvent;
        } else if (expectation == "预期推迟") {
            eventType = CanttTimeBarItem::TuichiEvent;
        } else if (expectation == "预期提前") {
            eventType = CanttTimeBarItem::TiqianEvent;
        } else if (expectation == "预期未完成") {
            eventType = CanttTimeBarItem::NonEvent;
        } else {
            qDebug() << "未知的预期状态：" << expectation;
            continue;
        }

        QDateTime midTime = startTime.addSecs((endTime.toMSecsSinceEpoch() - startTime.toMSecsSinceEpoch()) / 2);

        CanttTimeBarItem *item = view->addPlanTimeBar(taskId, startTime, endTime);
        if (item != nullptr) {
            item->addEvent(midTime, eventType, taskId);
        }
    }

    view->setAxisRange(QDateTime::fromString("2024-04-16 00:00", "yyyy-MM-dd HH:mm"),
                       QDateTime::fromString("2024-04-17 00:00", "yyyy-MM-dd HH:mm"),
                       tasksList);

    setCentralWidget(view);
    showMaximized();
}

Show::~Show()
{
    delete ui;
}
