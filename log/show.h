#ifndef SHOW_H
#define SHOW_H

#include "mainwindow.h"

namespace Ui {

class Show;
}


class Show : public MainWindow
{
    Q_OBJECT

public:
    explicit Show(QWidget *parent = nullptr);
    ~Show();

private:
    Ui::Show *ui; // 如果Show仍然有自己的UI布局，则保留此部分
};

#endif // SHOW_H
