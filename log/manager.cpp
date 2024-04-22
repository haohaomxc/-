#include "manager.h"
#include "ui_manager.h"
#include <QStandardItemModel>
#include "guanli.h"
#include "renwuwindow.h"

manager::manager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::manager)
{
    //main中采用的是sql语句修改数据库
    //manager中采用数据模型修改数据库
    ui->setupUi(this);
    connect(ui->pushButton_tuichu,&QPushButton::clicked,[=](){
        emit this->fanhui();
    }

    );
}



manager::~manager()
{
    delete ui;
}


// 假设GuanliWindow是你要打开的管理窗口类
#include "guanliwindow.h"

void manager::on_pushButton_guanli_clicked()
{
    GuanliWindow *guanliWindow = new GuanliWindow;

    // 显示新的GuanliWindow窗口
    guanliWindow->show();

    // 如果需要在打开GuanliWindow后关闭当前manager窗口
  //  this->hide(); // 隐藏当前manager窗口，但不释放资源
}



void manager::on_pushButton_renwufabu_clicked()
{
    RenwuWindow *renwuliWindow = new RenwuWindow;

    // 显示新的RenwuWindow窗口
    renwuliWindow->show();

    // 如果需要在打开RenwuWindow后关闭当前manager窗口
  //  this->hide(); // 隐藏当前manager窗口，但不释放资源
}
