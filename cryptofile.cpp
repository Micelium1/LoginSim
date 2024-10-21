#include "cryptofile.h"

CryptoFile::CryptoFile() {}

bool CryptoFile::encryptFile(const QString &inputFilePath, const QString &outputFilePath, const QString &password)
{
    HCRYPTPROV hProv = 0;
    HCRYPTKEY hKey = 0;
    HCRYPTHASH hHash = 0;

    // 1. Инициализация криптографического провайдера
    if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, 0)) {
        if (GetLastError() == NTE_BAD_KEYSET) {
            if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_NEWKEYSET)) {
                qCritical() << "Error during CryptAcquireContext!";
                return false;
            }
        } else {
            qCritical() << "Error during CryptAcquireContext!";
            return false;
        }
    }

    // 2. Создание хэш-объекта с использованием MD2
    if (!CryptCreateHash(hProv, CALG_MD2, 0, 0, &hHash)) {
        qCritical() << "Error during CryptCreateHash!";
        CryptReleaseContext(hProv, 0);
        return false;
    }

    // 3. Хэширование парольной фразы
    if (!CryptHashData(hHash, reinterpret_cast<const BYTE*>(password.toUtf8().constData()), password.size(), 0)) {
        qCritical() << "Error during CryptHashData!";
        CryptDestroyHash(hHash);
        CryptReleaseContext(hProv, 0);
        return false;
    }

    // 4. Генерация ключа с добавлением случайного значения (соли)
    if (!CryptDeriveKey(hProv, CALG_AES_256, hHash, CRYPT_EXPORTABLE | CRYPT_CREATE_SALT, &hKey)) {
        qCritical() << "Error during CryptDeriveKey!";
        CryptDestroyHash(hHash);
        CryptReleaseContext(hProv, 0);
        return false;
    }

    // 5. Установка режима шифрования (ECB)
    DWORD mode = CRYPT_MODE_ECB;
    if (!CryptSetKeyParam(hKey, KP_MODE, reinterpret_cast<BYTE*>(&mode), 0)) {
        qCritical() << "Error during CryptSetKeyParam!";
        CryptDestroyKey(hKey);
        CryptDestroyHash(hHash);
        CryptReleaseContext(hProv, 0);
        return false;
    }

    // 6. Чтение исходного файла
    QFile inputFile(inputFilePath);
    if (!inputFile.open(QIODevice::ReadOnly)) {
        qCritical() << "Error opening input file!";
        CryptDestroyKey(hKey);
        CryptDestroyHash(hHash);
        CryptReleaseContext(hProv, 0);
        return false;
    }

    QByteArray fileData = inputFile.readAll();
    inputFile.close();

    // 7. Шифрование данных
    DWORD dataLen = fileData.size();
    DWORD bufLen = dataLen + 16; // Для блочного шифрования буфер должен быть больше
    QVector<BYTE> encryptedData(bufLen);
    memcpy(encryptedData.data(), fileData.data(), dataLen);

    if (!CryptEncrypt(hKey, 0, TRUE, 0, encryptedData.data(), &dataLen, bufLen)) {
        qCritical() << "Error during CryptEncrypt!";
        CryptDestroyKey(hKey);
        CryptDestroyHash(hHash);
        CryptReleaseContext(hProv, 0);
        return false;
    }

    // 8. Запись зашифрованных данных в файл
    QFile outputFile(outputFilePath);
    if (!outputFile.open(QIODevice::WriteOnly)) {
        qCritical() << "Error opening output file!";
        CryptDestroyKey(hKey);
        CryptDestroyHash(hHash);
        CryptReleaseContext(hProv, 0);
        return false;
    }
    outputFile.write(reinterpret_cast<const char*>(encryptedData.data()), dataLen);
    outputFile.close();

    // 9. Очистка ресурсов
    CryptDestroyKey(hKey);
    CryptDestroyHash(hHash);
    CryptReleaseContext(hProv, 0);

    return true;
}

bool CryptoFile::decryptFile(const QString &inputFilePath, const QString &outputFilePath, const QString &password)
{
    HCRYPTPROV hProv = 0;
    HCRYPTKEY hKey = 0;
    HCRYPTHASH hHash = 0;

    // 1. Инициализация криптографического провайдера
    if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, 0)) {
        if (GetLastError() == NTE_BAD_KEYSET) {
            if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_NEWKEYSET)) {
                qCritical() << "Error during CryptAcquireContext!";
                return false;
            }
        } else {
            qCritical() << "Error during CryptAcquireContext!";
            return false;
        }
    }

    // 2. Создание хэш-объекта с использованием MD2
    if (!CryptCreateHash(hProv, CALG_MD2, 0, 0, &hHash)) {
        qCritical() << "Error during CryptCreateHash!";
        CryptReleaseContext(hProv, 0);
        return false;
    }

    // 3. Хэширование парольной фразы
    if (!CryptHashData(hHash, reinterpret_cast<const BYTE*>(password.toUtf8().constData()), password.size(), 0)) {
        qCritical() << "Error during CryptHashData!";
        CryptDestroyHash(hHash);
        CryptReleaseContext(hProv, 0);
        return false;
    }

    // 4. Генерация ключа с добавлением случайного значения (соли)
    if (!CryptDeriveKey(hProv, CALG_AES_256, hHash, CRYPT_EXPORTABLE, &hKey)) {
        qCritical() << "Error during CryptDeriveKey!";
        CryptDestroyHash(hHash);
        CryptReleaseContext(hProv, 0);
        return false;
    }

    // 5. Установка режима расшифровки (ECB)
    DWORD mode = CRYPT_MODE_ECB;
    if (!CryptSetKeyParam(hKey, KP_MODE, reinterpret_cast<BYTE*>(&mode), 0)) {
        qCritical() << "Error during CryptSetKeyParam!";
        CryptDestroyKey(hKey);
        CryptDestroyHash(hHash);
        CryptReleaseContext(hProv, 0);
        return false;
    }

    // 6. Чтение зашифрованного файла
    QFile inputFile(inputFilePath);
    if (!inputFile.open(QIODevice::ReadOnly)) {
        qCritical() << "Error opening input file!";
        CryptDestroyKey(hKey);
        CryptDestroyHash(hHash);
        CryptReleaseContext(hProv, 0);
        return false;
    }

    QByteArray encryptedData = inputFile.readAll();
    inputFile.close();

    // 7. Расшифровка данных
    DWORD dataLen = encryptedData.size();
    if (!CryptDecrypt(hKey, 0, TRUE, 0, reinterpret_cast<BYTE*>(encryptedData.data()), &dataLen)) {
        qCritical() << "Error during CryptDecrypt!";
        CryptDestroyKey(hKey);
        CryptDestroyHash(hHash);
        CryptReleaseContext(hProv, 0);
        return false;
    }

    // 8. Запись расшифрованных данных в файл
    QFile outputFile(outputFilePath);
    if (!outputFile.open(QIODevice::WriteOnly)) {
        qCritical() << "Error opening output file!";
        CryptDestroyKey(hKey);
        CryptDestroyHash(hHash);
        CryptReleaseContext(hProv, 0);
        return false;
    }
    outputFile.write(encryptedData.left(dataLen));
    outputFile.close();

    // 9. Очистка ресурсов
    CryptDestroyKey(hKey);
    CryptDestroyHash(hHash);
    CryptReleaseContext(hProv, 0);

    return true;
}

