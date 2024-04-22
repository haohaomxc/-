#ifndef MANAGER_H
#define MANAGER_H

#include <QWidget>

namespace Ui {
class manager;
}

class manager : public QWidget
{
    Q_OBJECT

public:
    explicit manager(QWidget *parent = nullptr);
    ~manager();

signals:
    void fanhui();
signals:
    void guanli(); // 新增声明
signals:
    void fabu();
private slots:
    void on_pushButton_guanli_clicked();

    void on_pushButton_renwufabu_clicked();

private:
    Ui::manager *ui;
};

#endif // MANAGER_H
