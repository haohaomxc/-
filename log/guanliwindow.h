#ifndef GUANLIWINDOW_H
#define GUANLIWINDOW_H
#include <QSqlError>
#include<QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QMainWindow>

namespace Ui {
class GuanliWindow;
}

class GuanliWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GuanliWindow(QWidget *parent = nullptr);
    ~GuanliWindow();

    void findall();

private slots:
    void on_find_clicked();

    void on_add_clicked();

    void on_del_clicked();

    void on_update_clicked();

private:
    Ui::GuanliWindow *ui;
    QSqlQueryModel *model;
};

#endif // GUANLIWINDOW_H
