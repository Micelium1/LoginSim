#include "passwordchangewindow.h"
#include "ui_passwordchangewindow.h"

PasswordChangeWindow::PasswordChangeWindow(QString _name, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PasswordChangeWindow)
    , name(_name)
    , userList(filename)

{
    ui->setupUi(this);
    ui->limitationNotify->setText(QString("<i>Минимальная длина вашего пароля: %1</i>").arg(userList.find(name)->toObject()[User::limited].toInt()));
    connect(ui->applyButton,&QPushButton::clicked,this,&PasswordChangeWindow::on_applyButton_press);
    connect(ui->leaveButton,&QPushButton::clicked,this,&QWidget::close);
}

void PasswordChangeWindow::on_applyButton_press()
{


    PasswordChangeCodes isSuccesfull = userList.changePassword(name,ui->oldPasswdInput->text(),ui->newPasswdInput->text());


    if (isSuccesfull == PasswordChangeCodes::Okay) {
        ui->changeStatus->setText("Пароль успешно изменён");
        QPalette pal = ui->changeStatus->palette();
        pal.setColor(QPalette::WindowText,Qt::green);
        ui->changeStatus->setPalette(pal);
        userList.save_changes();
    }
    else if (isSuccesfull == PasswordChangeCodes::WrongPassword) {
        ui->changeStatus->setText("Неверный пароль");
        QPalette pal = ui->changeStatus->palette();
        pal.setColor(QPalette::WindowText,Qt::red);
        ui->changeStatus->setPalette(pal);
    }
    else if (isSuccesfull == PasswordChangeCodes::ViolatedLimitations) {
        ui->changeStatus->setText("Пароль не соответствует минимальной длинне");
        QPalette pal = ui->changeStatus->palette();
        pal.setColor(QPalette::WindowText,Qt::red);
        ui->changeStatus->setPalette(pal);
    }
}

PasswordChangeWindow::~PasswordChangeWindow()
{
    delete ui;
}
