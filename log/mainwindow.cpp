#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QSqlQuery>//对sql语句操作
#include <QMessageBox>
#include <QDebug>
#include <QVariantList>//
#include <QCryptographicHash>//加密类
#include <QTime>//随机数种子
#include <QTimer>
#include "manager.h"
#include "putong.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)

{
    ui->setupUi(this);
    setFixedSize(760,450);
    //打印QT支持的数据库
//    qDebug()<<QSqlDatabase::drivers();
//    QSqlDatabase sjk=QSqlDatabase::addDatabase("QMYSQL");
//    sjk.setHostName("localhost");
//    sjk.setPort(3306);
//    sjk.setUserName("root");
//    sjk.setPassword("1330099414qwe");
//    sjk.setDatabaseName("xinxi");
//    if(!sjk.open())
//        QMessageBox::warning(this,"警告",sjk.lastError().text());
//    else
//        qDebug()<<"成功";


//第二种
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
        db.setHostName("127.0.0.1");  //连接本地主机
        db.setPort(3306);
        db.setDatabaseName("xinxi");
        db.setUserName("root");
        db.setPassword("1330099414qwe");
        bool ok = db.open();
        if (ok){
            QMessageBox::information(this, "infor", "link success");
        }
        else {
            QMessageBox::information(this, "infor", "link failed");
            qDebug()<<"error open database because"<<db.lastError().text();
}










//设置定时器，目的是在验证码输入错误后出发更新验证码 [34-43]
    yan->setInterval(100);
    yan->start();
    connect(yan,&QTimer::timeout,[=](){
        //验证功能
        ui->yanzhengma->setText(yanzheng());
       yzm = QString("%1").arg(ui->yanzhengma->text());
       yan->stop();
    });

//    //验证功能
//    ui->yanzhengma->setText(yanzheng());
//   yzm = QString("%1").arg(ui->yanzhengma->text());

//注册账号
    connect(ui->pushButton_zhuce,&QPushButton::clicked,[=](){
        //这里是验证码验证模块
        if(!yanzhengmayanzheng()){
             QMessageBox::warning(this,"警告","验证码错误");
            return ;
        }

        QString linezhanghao = ui->lineEditzhanghao->text();
        QString linemima = ui->lineEdit_mima->text();
        if(linemima == NULL||linezhanghao==NULL){
            QMessageBox::warning(this,"警告","账户或密码输入为空");
            return;
        }

        qDebug()<<linezhanghao;

        QSqlQuery qq;
//        //插入(自定义占位符?)
//        qq.exec("insert into yonghu(yonghu_zhanghao,yonghu_mima) values (?,?)");
//        //给字段绑定相应的值
//        QVariantList zh;
//        zh<<linezhanghao;
//        QVariantList mm;
//        mm<<linemima;
//        //按顺序绑定【一个一个按顺序绑定】
//        qq.addBindValue(zh);
//        qq.addBindValue(mm);
//        //执行预处理命令
//        qq.execBatch();
        QString caozuo =QString("insert into yonghu(yonghu_zhanghu,yonghu_mima)values('%1','%2')").arg(linezhanghao).arg(linemima);
//上面用了查找账户数量的方法判断是否重名在MYSQL中可行但由于QT中MYSQL只返回该语句是否执行成功，至于执行的查询数量等结果不反馈所以此处用主键的方式来设置账号是恰当的
        if(!qq.exec(caozuo) ){
            QMessageBox::warning(this,"警告","该账号存在");
            return ;
}
        else{
        QMessageBox::information(this,"通知","注册成功\n");
}
    });
//登录账号
    connect(ui->pushButton_denglu,&QPushButton::clicked,[=](){
        //这里是验证码验证模块
        if(!yanzhengmayanzheng()){
             QMessageBox::warning(this,"警告","验证码错误");
            return ;
        }

        QSqlQuery qq;
        QString linezhanghao = ui->lineEditzhanghao->text();
        QString linemima = ui->lineEdit_mima->text();
        check(linezhanghao,linemima);
        QString caozuo =QString("select * from yonghu where yonghu_zhanghu='%1'and yonghu_mima='%2'").arg(linezhanghao).arg(linemima);


        if(!qq.exec(caozuo) )
             QMessageBox::warning(this,"警告","密码错误");

                if(qq.next()){
                    //QMessageBox::information(this,"提示","登陆成功");
                    if(linezhanghao == "2020112159"){
                    manager *my = new manager;
                    this->hide();
                    my->show();
                    //用户点返回进入登陆界面
                    connect(my,&manager::fanhui,[=](){
                        my->close();
                        this->show();
                    });
                    }
                    else{
//                        QMessageBox::information(this,"提示","暂不支持非管理员帐号登录");
                        putong *yonghu= new putong(0,linezhanghao);
                        yonghu->show();
                    }

                }
                else
                    QMessageBox::warning(this,"警告","密码错误");
    });


//修改密码
    connect(ui->pushButton_wangjimima,&QPushButton::clicked,[=](){
        //这里是验证码验证模块
        if(!yanzhengmayanzheng()){
             QMessageBox::warning(this,"警告","验证码错误");
            return ;
        }

        QSqlQuery qq;
        QString linezhanghao = ui->lineEditzhanghao->text();
        QString linemima = ui->lineEdit_mima->text();
        check(linezhanghao,linemima);

        QString caozuo =QString("update yonghu set yonghu_mima = '%1'where yonghu_zhanghu = '%2'").arg(linemima).arg(linezhanghao);
        if(qq.exec(caozuo))
            QMessageBox::information(this,"通知","修改密码完成:"+linemima);
        else
            QMessageBox::warning(this,"警告","密码修改失败:请检查用户名是否正确");

    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::zhanghucunzai(QString zhanghu)
{
    QSqlQuery qq;
    QString caozuo = QString("select * from yonghu where yonghu_zhanghu = '%1'").arg(zhanghu);
    qq.exec(caozuo);
    if(qq.next())
        return true;
    return false;
}

void MainWindow::check(QString linezhanghao,QString linemima)
{
    if(linemima == NULL||linezhanghao==NULL){
        QMessageBox::warning(this,"警告","账户或密码输入为空");
        return;
    }
    if(!zhanghucunzai(linezhanghao)){
        QMessageBox::warning(this,"警告","账户不存在");
        return ;
    }
}




//生成验证码
QString MainWindow::yanzheng()
{
    a:
    int ret=0;
    for(int i=0 ; i<4;i++){
    //qsrand(QTime::currentTime().second()*1000);
    ret  = ret*10+qrand()%10;
    }
    if(ret<1000)
        goto a;
   return  QString::number( ret);

}

bool MainWindow::yanzhengmayanzheng()
{
    //验证码验证
    if(yzm != ui->inputyanzhengma->text()){
         yanzheng();
         yan->start();//启动定时器（不必让定时器一直工作，只需让其更改验证码时工作即可）
         return 0;
    }
    return 1;
}



void MainWindow::on_pushButton_tuichu_clicked()
{
    close ();
}
