#ifndef GUANLI_H
#define GUANLI_H

#include <QMainWindow>

namespace Ui {
class guanli;
}

class guanli : public QMainWindow
{
    Q_OBJECT

public:
    explicit guanli(QWidget *parent = nullptr);
    ~guanli();

private:
    Ui::guanli *ui;
};

#endif // GUANLI_H
