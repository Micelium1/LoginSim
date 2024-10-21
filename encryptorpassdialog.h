#ifndef ENCRYPTORPASSDIALOG_H
#define ENCRYPTORPASSDIALOG_H

#include <QDialog>

namespace Ui {
class EncryptorPassDialog;
}

class EncryptorPassDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EncryptorPassDialog(QWidget *parent = nullptr);
    void on_acceptButton_press();
    QString getKeyPass();
    ~EncryptorPassDialog();

private:
    Ui::EncryptorPassDialog *ui;
    QString keyPass;
};

#endif // ENCRYPTORPASSDIALOG_H
