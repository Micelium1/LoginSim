#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "usertojson.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QSharedPointer<UserToJson> userList,QWidget *parent = nullptr);
    ~MainWindow();

    void on_EnterButton_press(QSharedPointer<UserToJson> userList);
    void on_ExitButton_press();
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
