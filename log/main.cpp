#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include<QSqlDatabase>
#include <QFile>
#include <QTextStream>
#include <QApplication>



int main(int argc, char *argv[])
{
    //查看有哪些可用数据库
    QStringList type=QSqlDatabase::drivers();
    qDebug()<<type;
    QApplication a(argc, argv);
    // 如果QSS文件已经被添加到资源文件中
    QFile styleSheet("D:/qtproject/1.1.1/log/res/qss/style.qss");
    if (styleSheet.open(QFile::ReadOnly))
    {
        QTextStream stream(&styleSheet);
        QString styleSheetContent = stream.readAll();
        a.setStyleSheet(styleSheetContent);
        styleSheet.close();
    }
    else
    {
        // 处理文件未能打开的情况，例如打印错误信息
        qDebug() << "Failed to load stylesheet from file!";
    }

    MainWindow w;
    w.show();
    return a.exec();
}

