#include "putong.h"
#include "ui_putong.h"
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsProxyWidget>
#include <QToolTip>


// 在putong类的私有部分声明变量
QGraphicsView* graphicsView;
QGraphicsScene* graphicsScene;


void putong::on_pushButton_chaxun_clicked()
{
    // 获取当前窗口标题并添加"_renwu"
    QString tableName = windowTitle() + "_renwu";
      model = new QSqlTableModel(this, QSqlDatabase::database());
    model->setTable(tableName);

    model->setEditStrategy(QSqlTableModel::OnManualSubmit);

    // 同步模型与数据库结构
    if (!model->select()) {
        qDebug() << "无法选择表：" << model->lastError().text();
        return;
    }

    // 设置模型到tableView
    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents(); // 自动调整列宽以适应内容
}




void putong::on_pushButton_shengcheng_clicked()
{
    updateGanttChart();
}


putong::~putong()
{
    delete ui;
}

void putong::on_pushButton_tuichu_clicked()
{
    close();
}

void putong::updateGanttChart()
{
    graphicsScene->clear();

    // Fix: No Data Here
    // QModelIndexList selectedRows = ui->tableView->selectionModel()->selectedRows();
    QModelIndexList selectedRows = ui->tableView->selectionModel()->selectedIndexes();


    // for (const QModelIndex& index : selectedRows) {

    // Fix: Need More
    for (int i=0;i < 4;i++){


        //4.23
        // 获取表格中的数据
     //   QVariant startTimeVariant = model->data(model->index(i, 0));
        // QVariant endTimeVariant = model->data(model->index(i, 1));
      //  QString task = model->data(model->index(i, 1)).toString();
       // QString tool = model->data(model->index(i, 2)).toString();
       // QString note = model->data(model->index(i, 3)).toString();

        // 获取表格中的数据
              QVariant startTimeVariant = model->data(model->index(i, 0));
              QVariant endTimeVariant = model->data(model->index(i, 1));
              QString task = model->data(model->index(i, 2)).toString();
              QString tool = model->data(model->index(i, 3)).toString();
              QString note = model->data(model->index(i, 4)).toString();



        // 获取表格中的数据
//        QVariant startTimeVariant = model->data(index.sibling(index.row(), 0));
//        QVariant endTimeVariant = model->data(index.sibling(index.row(), 1));
//        QString task = model->data(index.sibling(index.row(), 2)).toString();
//        QString tool = model->data(index.sibling(index.row(), 3)).toString();
//        QString note = model->data(index.sibling(index.row(), 4)).toString();

        // 解析时间区间
        bool ok;
        int startH, startM, endH, endM;
        QStringList timeRangeStart = startTimeVariant.toString().split("-");
        startH = timeRangeStart[0].mid(0, 2).toInt(&ok, 10);
        startM = timeRangeStart[0].mid(3, 2).toInt(&ok, 10);

        // QStringList timeRangeEnd = endTimeVariant.toString().split("-");
        QStringList timeRangeEnd = startTimeVariant.toString().split("-");


        endH = timeRangeEnd[1].mid(0, 2).toInt(&ok, 10);
        endM = timeRangeEnd[1].mid(3, 2).toInt(&ok, 10);

        // 计算矩形条形图的位置和大小
        qreal xStart = startH * 100 / 23 + startM * 100 / (23 * 60);
        qreal xEnd = endH * 100 / 23 + endM * 100 / (23 * 60);
        qreal rectWidth = xEnd - xStart;
        qreal yBase = 10; // 纵轴起点位置
        qreal rectHeight = 20; // 条形图高度，可根据实际情况调整

        // 创建条形图
        QGraphicsRectItem* bar = new QGraphicsRectItem(xStart, yBase, rectWidth, rectHeight);
        bar->setPen(Qt::NoPen);
        bar->setBrush(Qt::blue);

        // 添加任务标签
        QGraphicsSimpleTextItem* taskLabel = new QGraphicsSimpleTextItem(task, bar);
        taskLabel->setPos(xStart, yBase - 30);

        // 设置条形图的tooltip
        bar->setToolTip(QString("工具：%1\n备注：%2").arg(tool, note));
        // 将条形图和任务标签添加到甘特图场景中
        graphicsScene->addItem(bar);
        graphicsScene->addItem(taskLabel);
        // 重新调整场景大小以适应所有条形图并触发滚动条（如果场景内容超出视图）
        graphicsScene->setSceneRect(0, 0, ui->graphicsView->width(), ui->graphicsView->height());

        // graphicsScene->invalidate(graphicsScene->sceneRect()); // 更新整个场景区域
    }

    // just for test
//    QBrush greenbrush(Qt::green);
//    QBrush bluebrush(Qt::blue);
//    QPen outlinePen(Qt::black);
//    graphicsScene->addEllipse(0, 0, 300, 60, outlinePen, greenbrush);
}

putong::putong(QWidget *parent, QString zhanghao) :
    QWidget(parent),
    ui(new Ui::putong)
{
    ui->setupUi(this);
    setWindowTitle(zhanghao);

    // Update Here
//    graphicsView = new QGraphicsView(this);
//    graphicsScene = new QGraphicsScene();
//    graphicsView->setScene(graphicsScene);
    graphicsScene = new QGraphicsScene(this);
    ui->graphicsView->setScene(graphicsScene);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded); // 根据需要显示水平滚动条
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded); // 根据需要显示垂直滚动条

    // Update Here:
    tableView = new QTableView();

    // 初始化甘特图相关的槽函数触发事件
    connect(ui->tableView->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &putong::updateGanttChart);
}
