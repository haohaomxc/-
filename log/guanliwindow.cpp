#include "guanliwindow.h"
#include "ui_guanliwindow.h"
//数据库
#include <QSqlError>
#include<QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QMessageBox>

//显示中文
//#pragma execution_character_set("utf-8")


GuanliWindow::GuanliWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GuanliWindow)
{

    ui->setupUi(this);

    model =new  QSqlQueryModel(this);
    findall();
}


void GuanliWindow::findall()
{
    model->setQuery(QString("select * FROM yonghu"));
    //列名
 // model->setHeaderData(0, Qt::Horizontal, tr("序号"));
    model->setHeaderData(0, Qt::Horizontal, tr("账户名"));
    model->setHeaderData(1, Qt::Horizontal, tr("密码"));
    model->setHeaderData(2, Qt::Horizontal, tr("用户类型"));
    ui->tableView->setModel(model);//数据放置进去
}


GuanliWindow::~GuanliWindow()
{
    delete ui;
}
//查找功能
void GuanliWindow::on_find_clicked()
{
    QString yonghu_zhanghu = ui->find_name->text();
        model->setQuery(QString("select * FROM yonghu Where yonghu_zhanghu = '%1'").arg(yonghu_zhanghu));
        ui->tableView->setModel(model);//数据放置进去
}
//添加用户功能
void GuanliWindow::on_add_clicked()
{
    QString yonghu_zhanghu = ui->yonghu_zhanghu->text();
    QString yonghu_mima = ui->yonghu_mima->text();
    QString yonghu_leixing = ui->yonghu_leixing->text();

    QSqlQuery query;
    QString str = QString("INSERT INTO yonghu (yonghu_zhanghu,yonghu_mima,yonghu_leixing) values('%1','%2','%3')").arg(yonghu_zhanghu).arg(yonghu_mima).arg(yonghu_leixing);//删除表中符合id的所有数据
    if (query.exec(str)==false)
    {
        QMessageBox::information(this, "infor", "添加失败");
    }
    else
    {
        QMessageBox::information(this, "infor", "添加成功");
        //再执行一边查询全部的操作
        findall();
    }
}
//删除功能
void GuanliWindow::on_del_clicked()
{
    QString yonghu_zhanghu = ui->yonghu_zhanghu->text();
       QSqlQuery query;
       QString str = QString("DELETE FROM yonghu WHERE yonghu_zhanghu = '%1'").arg(yonghu_zhanghu);//删除表中符合yonghu_zhanghu的所有数据
       if (query.exec(str)==false)
       {
           QMessageBox::information(this, "infor", "删除失败");
       }
       else
       {
           QMessageBox::information(this, "infor", "删除成功");
           //再执行一边查询全部的操作
           findall();
       }
}
//修改功能
void GuanliWindow::on_update_clicked()
{
    QString yonghu_zhanghu = ui->yonghu_zhanghu->text();
    QString yonghu_mima = ui->yonghu_mima->text();
    QString yonghu_leixing = ui->yonghu_leixing->text();

    QSqlQuery query;
    QString str = QString("UPDATE yonghu SET yonghu_mima='%1' WHERE yonghu_zhanghu = '%2'").arg(yonghu_mima).arg(yonghu_zhanghu);//删除表中符合id的所有数据
    if (query.exec(str)==false)
    {
        QMessageBox::information(this, "infor", "修改失败");
    }
    else
    {
        QMessageBox::information(this, "infor", "修改成功");
        //再执行一边查询全部的操作
        findall();
    }
}
