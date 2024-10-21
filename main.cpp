#include "mainwindow.h"
#include "usertojson.h"
#include "encryptorpassdialog.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "RegistrationAndLogin_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    QString keyPass;

    {
        EncryptorPassDialog dial;
        dial.exec();
        keyPass = dial.getKeyPass();
    }

    QSharedPointer<UserToJson> userList = QSharedPointer<UserToJson>(new UserToJson(filename,keyPass));
    MainWindow w(userList);
    w.show();
    return a.exec();
}
