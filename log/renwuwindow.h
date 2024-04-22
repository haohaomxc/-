#ifndef RENWUWINDOW_H
#define RENWUWINDOW_H
#include <QSqlError>
#include<QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QMainWindow>
#include <QStringList>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtWidgets>
#include <QSlider>
#include <QLineEdit>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui {
class RenwuWindow;
}
QT_END_NAMESPACE

class RenwuWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit RenwuWindow(QWidget *parent = nullptr);
    ~RenwuWindow();

private:
    Ui::RenwuWindow *ui;
    QSqlDatabase db;
    QSqlQuery query;
    QSqlQueryModel *modell;
    QTableView *tableView;
    QSlider *hourSlider;
    QSlider *minuteSlider;
    QLineEdit *lineEdit_shijian;

public slots:
    void updateTextInput(int hour1, int minute1, int hour2, int minute2);

private slots:
    void fillComboBox();
    void on_pushButton_xuanding_clicked();
    void on_pushButton_fabu_clicked();
    bool createNewTable(const QString &tableName);
    void findRenwu();
    bool isTableExists(const QString &tableName);
    void loadTableToTableView(const QString &tableName);
    void on_pushButton_tianjia_clicked();

public slots:
    void onHourValueChanged(int value);
    void onMinuteValueChanged(int value);
    void onHourSlider2ValueChanged(int value);
    void onMinuteSlider2ValueChanged(int value);


};
#endif // RENWUWINDOW_H

