#include "usertojson.h"

QString filename = "userdata.json";

const QString User::name = "Username";
const QString User::passwd = "Password";
const QString User::blocked = "Blocked";
const QString User::limited = "Limited";

UserToJson::UserToJson(QString filename) {

    JsonFile.setFileName(filename);

    if (!JsonFile.exists()) {
        addNewUser("admin",false,true);
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
            addNewUser("admin",false,true);
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

        JsonArray = QJsonDocument::fromJson(fileData).array();

    } else {
        qDebug() << "Не удалось открыть файл для чтения.";
        throw std::system_error(std::make_error_code(std::errc::io_error),"Не удалось открыть файл для чтения");
    }

}

void UserToJson::addNewUser(QString name, bool blocked, bool limited)
{
    QJsonObject object;
    object[User::name] = name;
    object[User::passwd] = "";
    object[User::blocked] = blocked;
    object[User::limited] = limited;
    JsonArray.append(object);
}

void UserToJson::modifyUser(QString name, bool blocked, bool limited)
{
    for (QJsonValueRef object:(JsonArray))
    {
        if (object.toObject()[User::name] == name) {
            QJsonObject obj = object.toObject();

            obj[User::blocked] = blocked;
            obj[User::limited] = limited;

            object = QJsonValue(obj);
            return;
        }

    }
    addNewUser(name,blocked,limited);
}

void UserToJson::removeUser(QString name)
{
    JsonArray.removeAt(indexFind(name));
}

int UserToJson::indexFind(QString name) const  {
    for (int i = 0; i < JsonArray.size();++i) {
        if (JsonArray[i][User::name] == name) return i;
    }
    return -1;
}

bool UserToJson::changePassword(QString name, QString old_passwd, QString new_passwd)
{
    std::optional<QJsonValueRef> user = find(name);
    QJsonValueRef ref = user.value();
    if (ref.toObject()[User::passwd] == old_passwd) {
        QJsonObject obj = ref.toObject();
        obj[User::passwd] = new_passwd;
        ref = QJsonValue(obj);
        return true;
    }
    return false;

}

std::optional<QJsonValueRef> UserToJson::find(QString name)
{
    for (QJsonValueRef iter_ref:JsonArray) {
        if (iter_ref.toObject()[User::name] == name) {
            return iter_ref;
        }
    }
    return std::nullopt;
}
void UserToJson::save_changes() {
    JsonFile.open(QFile::WriteOnly);
    JsonFile.write(QJsonDocument(JsonArray).toJson());
    JsonFile.close();
}

QSharedPointer<QJsonArray> UserToJson::getJsonArray() const
{
    QSharedPointer<QJsonArray> protectedArray(new QJsonArray());
    for (QJsonValueConstRef iter_ref:JsonArray) {
        QJsonObject protectedJsonObject = iter_ref.toObject();
        protectedJsonObject.remove(User::passwd);
        protectedArray->append(protectedJsonObject);

    }
    return protectedArray;
}


