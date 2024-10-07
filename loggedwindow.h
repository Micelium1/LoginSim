#ifndef LOGGEDWINDOW_H
#define LOGGEDWINDOW_H

#include "ui_loggedwindow.h"
#include <QMainWindow>

class LoggedWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit LoggedWindow(QString name,QWidget *parent = nullptr);
    void on_userListButton_press();
    void on_changePasswordButton_press(QString name);
private:
    Ui::LoggedWindow *ui;
    QWidget* parentWindow;
protected:
    void closeEvent(QCloseEvent *event) override;
};

#endif // LOGGEDWINDOW_H
