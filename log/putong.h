#ifndef PUTONG_H
#define PUTONG_H

#include <QWidget>
#include <QUdpSocket>
#include <QSqlTableModel> // 新增导入QSqlTableModel头文件
#include <QTableView> // 新增导入QTableView头文件
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsSimpleTextItem>
#include <QToolTip>

namespace Ui {
class putong;
}

class putong : public QWidget
{
    Q_OBJECT

public:
    explicit putong(QWidget *parent = nullptr, QString zhanghao = ""); // 修改构造函数签名以接收参数
    ~putong();

private:
    Ui::putong *ui;
    QTableView *tableView; // 新增QTableView成员变量
    QSqlTableModel* model;
    QGraphicsView* graphicsView;
    QGraphicsScene* graphicsScene;
    void updateGanttChart();


private slots:
    void on_pushButton_chaxun_clicked(); // 新增槽函数声明
    void on_pushButton_tuichu_clicked();
    void on_pushButton_shengcheng_clicked();
};

#endif // PUTONG_H
