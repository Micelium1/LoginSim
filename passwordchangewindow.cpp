#include "passwordchangewindow.h"
#include "ui_passwordchangewindow.h"

PasswordChangeWindow::PasswordChangeWindow(QString _name, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PasswordChangeWindow)
    , name(_name)
{
    ui->setupUi(this);
    connect(ui->applyButton,&QPushButton::clicked,this,&PasswordChangeWindow::on_applyButton_press);
    connect(ui->leaveButton,&QPushButton::clicked,this,&QWidget::close);
}

void PasswordChangeWindow::on_applyButton_press()
{
    UserToJson userList(filename);

    bool isSuccesfull = userList.changePassword(name,ui->oldPasswdInput->text(),ui->newPasswdInput->text());

    if (isSuccesfull) {
        ui->changeStatus->setText("Пароль успешно изменён");
        QPalette pal = ui->changeStatus->palette();
        pal.setColor(QPalette::WindowText,Qt::green);
        ui->changeStatus->setPalette(pal);
        userList.save_changes();
    }
    else {
        ui->changeStatus->setText("Неверный пароль");
        QPalette pal = ui->changeStatus->palette();
        pal.setColor(QPalette::WindowText,Qt::red);
        ui->changeStatus->setPalette(pal);
    }
}

PasswordChangeWindow::~PasswordChangeWindow()
{
    delete ui;
}
