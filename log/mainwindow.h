#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QtSql/QSqlDatabase>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool zhanghucunzai(QString zhanghu);
    void check(QString linezhanghao,QString linemima);
    QString yanzheng();
    bool yanzhengmayanzheng();

public:
    static QSqlDatabase db;

      QString yzm ;
      QTimer *yan =  new QTimer(this);

private slots:
      void on_pushButton_tuichu_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H

