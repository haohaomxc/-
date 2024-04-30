#include "renwuwindow.h"
#include "ui_renwuwindow.h"
//数据库
#include <QSqlError>
#include<QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QDebug>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QMainWindow>
#include <QSlider>
#include <QLineEdit>
#include <QString>

RenwuWindow::RenwuWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::RenwuWindow)
{
    ui->setupUi(this);

    // 已经全局连接并打开了数据库
    fillComboBox();


    modell = new QSqlQueryModel(this);
        findRenwu();


        ui->hourSlider->setMinimum(0); // 最小值为0
        ui->hourSlider->setMaximum(23); // 最大值为23

        ui->minuteSlider->setMinimum(0); // 最小值为0
        ui->minuteSlider->setMaximum(59); // 最大值为59

        ui->hourSlider_2->setMinimum(0); // 最小值为0
        ui->hourSlider_2->setMaximum(23); // 最大值为23

        ui->minuteSlider_2->setMinimum(0); // 最小值为0
        ui->minuteSlider_2->setMaximum(59); // 最大值为59

    // 连接信号与槽
        connect(ui->hourSlider, &QSlider::valueChanged, this, &RenwuWindow::onHourValueChanged);
        connect(ui->minuteSlider, &QSlider::valueChanged, this, &RenwuWindow::onMinuteValueChanged);
        connect(ui->hourSlider_2, &QSlider::valueChanged, this, &RenwuWindow::onHourSlider2ValueChanged);
        connect(ui->minuteSlider_2, &QSlider::valueChanged, this, &RenwuWindow::onMinuteSlider2ValueChanged);

}


//此版本代码仅支持实现单段时间：
//void RenwuWindow::onHourValueChanged(int value)
//{
//    int minute = ui->minuteSlider->value();
//    updateTextInput(value, minute);
//}

//void RenwuWindow::onMinuteValueChanged(int value)
//{
//    int hour = ui->hourSlider->value();
//    updateTextInput(hour, value);
//}

//void RenwuWindow::updateTextInput(int hour, int minute)
//{
//    QString timeStr = QStringLiteral("%1:%2").arg(hour, 2, 10, QLatin1Char('0')).arg(minute, 2, 10, QLatin1Char('0'));
//    ui->lineEdit_shijian->setText(timeStr);
//}

void RenwuWindow::onHourValueChanged(int value)
{
    int minute = ui->minuteSlider->value();
    int hour2 = ui->hourSlider_2->value();
    int minute2 = ui->minuteSlider_2->value();
    updateTextInput(value, minute, hour2, minute2);
}

void RenwuWindow::onMinuteValueChanged(int value)
{
    int hour = ui->hourSlider->value();
    int hour2 = ui->hourSlider_2->value();
    int minute2 = ui->minuteSlider_2->value();
    updateTextInput(hour, value, hour2, minute2);
}

// 新增的槽函数声明与实现
void RenwuWindow::onHourSlider2ValueChanged(int value)
{
    int minute1 = ui->minuteSlider->value();
    int minute2 = ui->minuteSlider_2->value();
    int hour1 = ui->hourSlider->value();
    updateTextInput(hour1, minute1, value, minute2);
}

void RenwuWindow::onMinuteSlider2ValueChanged(int value)
{
    int hour1 = ui->hourSlider->value();
    int minute1 = ui->minuteSlider->value();
    int hour2 = ui->hourSlider_2->value();
    updateTextInput(hour1, minute1, hour2, value);
}

void RenwuWindow::updateTextInput(int hour1, int minute1, int hour2, int minute2)
{
    QString timeStr = QStringLiteral("%1:%2-%3:%4")
                     .arg(hour1, 2, 10, QLatin1Char('0'))
                     .arg(minute1, 2, 10, QLatin1Char('0'))
                     .arg(hour2, 2, 10, QLatin1Char('0'))
                     .arg(minute2, 2, 10, QLatin1Char('0'));
    ui->lineEdit_shijian->setText(timeStr);
}






void RenwuWindow::fillComboBox()
{
    // 执行SQL查询
    query.prepare("SELECT DISTINCT yonghu_zhanghu FROM yonghu");
    if (!query.exec()) {
        qDebug() << "Error executing query: " << query.lastError().text();
        return;
    }

    // 获取ComboBox对象并填充选项
    QComboBox *comboBox = ui->comboBox;
    if (comboBox) {
        comboBox->clear();
        while (query.next()) {
            QString zhanghu = query.value(0).toString();
            comboBox->addItem(zhanghu);
        }
    } else {
        qDebug() << "ComboBox not found!";
    }
}


RenwuWindow::~RenwuWindow()
{
    delete ui;
}


//无检查重复表版本
//void RenwuWindow::on_pushButton_xuanding_clicked()
//{
    // 获取comboBox当前显示的内容
//    QString selectedText = ui->comboBox->currentText();

    // 构造新表名
//    QString tableName = selectedText + "_renwu";

    // 创建新表
 //   if (createNewTable(tableName)) {
 //       qDebug() << "成功创建新表：" << tableName;
//    } else {
 //       qDebug() << "创建新表失败";
 //   }
//}

//有的版本
void RenwuWindow::on_pushButton_xuanding_clicked()
{
    // 获取comboBox当前显示的内容
    QString selectedText = ui->comboBox->currentText();

    // 构造新表名
    QString tableName = selectedText + "_renwu";

    // 检查表是否存在
    if (isTableExists(tableName)) {
        // 表已存在，加载表的内容到tableView
        loadTableToTableView(tableName);
    } else {
        // 表不存在，尝试创建新表
        if (createNewTable(tableName)) {
            qDebug() << "成功创建新表：" << tableName;
        } else {
            qDebug() << "创建新表失败";
        }
    }
}

// 新增函数：检查表是否存在
bool RenwuWindow::isTableExists(const QString &tableName)
{
    QSqlQuery query(QSqlDatabase::database("xinxi"));
    query.prepare("SELECT COUNT(*) FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_NAME = ?");
    query.addBindValue(tableName);

    if (!query.exec()) {
        qDebug() << "查询表是否存在时出错：" << query.lastError().text();
        return false;
    }

    query.first();
    int count = query.value(0).toInt();
    return count > 0;
}

// 新增函数：加载表的内容到tableView
void RenwuWindow::loadTableToTableView(const QString &tableName)
{
    // 清空现有的modell（如果有数据）
    modell->clear();

    // 设置查询语句
    modell->setQuery(QString("SELECT 时间, 地点, taskId, 工具, 备注, 预期 FROM %1").arg(tableName));

    // 设置表头数据
    modell->setHeaderData(0, Qt::Horizontal, tr("时间"));
    modell->setHeaderData(1, Qt::Horizontal, tr("地点"));
    modell->setHeaderData(2, Qt::Horizontal, tr("任务"));
    modell->setHeaderData(3, Qt::Horizontal, tr("工具"));
    modell->setHeaderData(4, Qt::Horizontal, tr("备注"));
    modell->setHeaderData(5, Qt::Horizontal, tr("预期"));

    // 设置模型到tableView
    ui->tableView->setModel(modell);
    ui->tableView->resizeColumnsToContents();
}


bool RenwuWindow::createNewTable(const QString &tableName)
{
    // 使用已连接好的数据库对象
    QSqlQuery query;

    // SQL语句创建新表
    QString sql = QStringLiteral(
        "CREATE TABLE IF NOT EXISTS `%1` "
        "(时间 VARCHAR(255) NOT NULL, "
        "地点 VARCHAR(255) NOT NULL, "
        "任务 VARCHAR(255) NOT NULL, "
        "工具 VARCHAR(255) NOT NULL, "
        "备注 VARCHAR(255) NOT NULL);"
        "预期 VARCHAR(255) NOT NULL);"
    ).arg(tableName);

    // 执行SQL语句
    if (!query.exec(sql)) {
        qDebug() << "创建新表SQL执行失败：" << query.lastError().text();
        return false;
    }

    return true;
}


void RenwuWindow::findRenwu()
{
    modell->setQuery(QString("SELECT * FROM your_new_table")); // 将"your_new_table"替换为实际的新表名

    // 设置表头数据
    modell->setHeaderData(0, Qt::Horizontal, tr("时间"));
    modell->setHeaderData(1, Qt::Horizontal, tr("地点"));
    modell->setHeaderData(2, Qt::Horizontal, tr("任务"));
    modell->setHeaderData(3, Qt::Horizontal, tr("工具"));
    modell->setHeaderData(4, Qt::Horizontal, tr("备注"));
    modell->setHeaderData(5, Qt::Horizontal, tr("预期"));

    // 将数据模型设置到tableView
    ui->tableView->setModel(modell);
}



void RenwuWindow::on_pushButton_fabu_clicked()
{
    // 获取当前显示在comboBox中的表名
    QString selectedText = ui->comboBox->currentText();
    QString tableName = selectedText + "_renwu";

    // 重新加载表的内容到tableView
    loadTableToTableView(tableName);
}


//按钮：添加的实现
void RenwuWindow::on_pushButton_tianjia_clicked()
{

        // 获取comboBox当前显示的内容
        QString selectedText = ui->comboBox->currentText();
        QString tableName = selectedText + "_renwu";

        // 获取文本框中的数据
        QString timeStr = ui->lineEdit_shijian->text();
        QString locationStr = ui->lineEdit_didian->text();
        QString taskStr = ui->lineEdit_renwu->text();
        QString toolStr = ui->lineEdit_gongju->text();
        QString noteStr = ui->lineEdit_beizhu->text();
        QString expStr = ui->lineEdit_yuqi->text();

        // 直接执行插入操作，不检查表是否存在
        // 注意：在实际项目中，建议在插入前验证表结构，否则可能会因为表结构不匹配而导致插入失败
        QString insertQuery = "INSERT INTO " + tableName + " (时间, 地点, 任务, 工具, 备注, 预期) VALUES (?, ?, ?, ?, ?, ?)";
        QSqlQuery query;

        // 准备并执行SQL语句，绑定参数
        query.prepare(insertQuery);
        query.bindValue(0, timeStr);
        query.bindValue(1, locationStr);
        query.bindValue(2, taskStr);
        query.bindValue(3, toolStr);
        query.bindValue(4, noteStr);
        query.bindValue(5, expStr);

        if (!query.exec()) {
            QMessageBox::critical(this, "错误", QStringLiteral("插入数据失败: %1").arg(query.lastError().text()));
        } else {
            qDebug() << "成功插入数据到表：" << tableName;
        }
        // 清除文本框内容
        ui->lineEdit_shijian->clear();
        ui->lineEdit_didian->clear();
        ui->lineEdit_renwu->clear();
        ui->lineEdit_gongju->clear();
        ui->lineEdit_beizhu->clear();
        ui->lineEdit_yuqi->clear();
}

