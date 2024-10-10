#include "usertojson.h"

QString filename = "userdata.json";

const QString User::name = "Username";
const QString User::passwd = "Password";
const QString User::blocked = "Blocked";
const QString User::limited = "Limited";

UserToJson::UserToJson(QString filename) {

    JsonFile.setFileName(filename);

    if (!JsonFile.exists()) {
        addNewUser("admin",false,0);
        if (JsonFile.open(QFile::WriteOnly)) {
            JsonFile.write(QJsonDocument(JsonArray).toJson());
            JsonFile.close();
        }
        return;
    }
    if (JsonFile.open(QFile::ReadOnly)) {

        QByteArray fileData = JsonFile.readAll();
        JsonFile.close();

        if (fileData.isEmpty()) {
            JsonFile.close();
            addNewUser("admin",false,0);
            if (JsonFile.open(QFile::WriteOnly)) {
                QJsonDocument jsonDoc(JsonArray);
                JsonFile.write(jsonDoc.toJson());
                JsonFile.close();
            } else {
                qDebug() << "Не удалось открыть файл для записи.";
                throw std::system_error(std::make_error_code(std::errc::io_error),"Не удалось открыть файл для записи");
            }
            return;
        }
        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(fileData,&err);
        if (err.error) throw std::runtime_error(err.errorString().toStdString());
        JsonArray = doc.object();

    } else {
        qDebug() << "Не удалось открыть файл для чтения.";
        throw std::system_error(std::make_error_code(std::errc::io_error),"Не удалось открыть файл для чтения");
    }

}

void UserToJson::addNewUser(QString name, bool blocked, int limited)
{
    QJsonObject user;
    user[User::passwd] = "";
    user[User::blocked] = blocked;
    user[User::limited] = limited;
    JsonArray[name] = user;
}

void UserToJson::modifyUser(QString name, bool blocked, int limited)
{

    QJsonObject user = JsonArray[name].toObject();
    if (user.empty()) {
        addNewUser(name,blocked,limited);
        return;
    }
    user[User::blocked] = blocked;
    user[User::limited] = limited;
    JsonArray[name] = user;

}

void UserToJson::removeUser(QString name)
{
    JsonArray.remove(name);
}

QJsonObject UserToJson::getUser(QString name)
{
    QJsonObject user = JsonArray[name].toObject();
    user[User::name] = name;
    return user;
}





PasswordChangeCodes UserToJson::changePassword(QString name, QString old_passwd, QString new_passwd)
{
    if (!JsonArray.contains(name)) return PasswordChangeCodes::WrongPassword;
    QJsonObject user = JsonArray[name].toObject();
    if (!(user[User::passwd] == old_passwd)) {
        return WrongPassword;
    }


    if (user[User::limited].toInt() > new_passwd.size()) return ViolatedLimitations;

    user[User::passwd] = new_passwd;
    JsonArray[name] = user;

    return Okay;
}


void UserToJson::save_changes() {
    JsonFile.open(QFile::WriteOnly);
    JsonFile.write(QJsonDocument(JsonArray).toJson());
    JsonFile.close();
}

QSharedPointer<QJsonArray> UserToJson::getJsonArray() const
{
    QSharedPointer<QJsonArray> protectedArray(new QJsonArray());

    for (const QString& iter_ref:JsonArray.keys()) {
        QJsonObject protectedJsonObject = JsonArray[iter_ref].toObject();
        protectedJsonObject.remove(User::passwd);
        protectedJsonObject[User::name] = iter_ref;
        protectedArray->append(protectedJsonObject);

    }
    return protectedArray;
}


