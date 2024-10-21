#include "cryptofile.h"
CryptoFile::CryptoFile() {}

bool CryptoFile::encryptFile(const QString &inputFilePath, const QString &outputFilePath, const QString &password)
{
    HCRYPTPROV hProv = 0;
    HCRYPTKEY hKey = 0;
    HCRYPTHASH hHash = 0;

    if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, 0)) {
        if (GetLastError() == NTE_BAD_KEYSET) {
            if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_NEWKEYSET)) {
                qCritical() << "Error during CryptAcquireContext!" << GetLastError();
                return false;
            }
        } else {
            qCritical() << "Error during CryptAcquireContext!" << GetLastError();
            return false;
        }
    }

    if (!CryptCreateHash(hProv, CALG_MD2, 0, 0, &hHash)) {
        qCritical() << "Error during CryptCreateHash!" << GetLastError();
        CryptReleaseContext(hProv, 0);
        return false;
    }

    if (!CryptHashData(hHash, reinterpret_cast<const BYTE*>(password.toUtf8().constData()), password.size(), 0)) {
        qCritical() << "Error during CryptHashData!" << GetLastError();
        CryptDestroyHash(hHash);
        CryptReleaseContext(hProv, 0);
        return false;
    }

    if (!CryptDeriveKey(hProv, CALG_RC2, hHash, CRYPT_EXPORTABLE | CRYPT_CREATE_SALT, &hKey)) {
        DWORD dw = GetLastError();

        qCritical() << "Error during CryptDeriveKey!" ;
        if (dw == ERROR_INVALID_HANDLE) qCritical() << "ERROR_INVALID_HANDLE";
        if (dw == ERROR_INVALID_PARAMETER) qCritical() << "ERROR_INVALID_PARAMETER";
        if (dw == NTE_BAD_ALGID) qCritical() << "NTE_BAD_ALGID";
        if (dw == NTE_BAD_FLAGS) qCritical() << "NTE_BAD_FLAGS";
        if (dw == NTE_BAD_HASH) qCritical() << "NTE_BAD_HASH";
        if (dw == NTE_BAD_HASH_STATE) qCritical() << "NTE_BAD_HASH_STATE";
        if (dw == NTE_BAD_UID) qCritical() << "NTE_BAD_UID";
        if (dw == NTE_FAIL) qCritical() << "NTE_FAIL";
        if (dw == NTE_SILENT_CONTEXT) qCritical() << "NTE_SILENT_CONTEXT";
        CryptDestroyHash(hHash);
        CryptReleaseContext(hProv, 0);
        return false;
    }

    DWORD mode = CRYPT_MODE_ECB;
    if (!CryptSetKeyParam(hKey, KP_MODE, reinterpret_cast<BYTE*>(&mode), 0)) {
        qCritical() << "Error during CryptSetKeyParam!" << GetLastError();
        CryptDestroyKey(hKey);
        CryptDestroyHash(hHash);
        CryptReleaseContext(hProv, 0);
        return false;
    }


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


    DWORD dataLen = fileData.size();
    DWORD bufLen = dataLen + 16;
    QByteArray encryptedData(bufLen, 0);
    memcpy(encryptedData.data(), fileData.data(), dataLen);

    if (!CryptEncrypt(hKey, 0, TRUE, 0, reinterpret_cast<BYTE*>(encryptedData.data()), &dataLen, bufLen)) {
        qCritical() << "Error during CryptEncrypt!" << GetLastError();
        CryptDestroyKey(hKey);
        CryptDestroyHash(hHash);
        CryptReleaseContext(hProv, 0);
        return false;
    }


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


    if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, 0)) {
        if (GetLastError() == NTE_BAD_KEYSET) {
            if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_NEWKEYSET)) {
                qCritical() << "Error during CryptAcquireContext!" << GetLastError();
                return false;
            }
        } else {
            qCritical() << "Error during CryptAcquireContext!" << GetLastError();
            return false;
        }
    }


    if (!CryptCreateHash(hProv, CALG_MD2, 0, 0, &hHash)) {
        qCritical() << "Error during CryptCreateHash!" << GetLastError();
        CryptReleaseContext(hProv, 0);
        return false;
    }


    if (!CryptHashData(hHash, reinterpret_cast<const BYTE*>(password.toUtf8().constData()), password.size(), 0)) {
        qCritical() << "Error during CryptHashData!" << GetLastError();
        CryptDestroyHash(hHash);
        CryptReleaseContext(hProv, 0);
        return false;
    }


    if (!CryptDeriveKey(hProv, CALG_RC2, hHash, CRYPT_EXPORTABLE | CRYPT_CREATE_SALT, &hKey)) {
        DWORD dw = GetLastError();

        qCritical() << "Error during CryptDeriveKey!";
        if (dw == ERROR_INVALID_HANDLE) qCritical() << "ERROR_INVALID_HANDLE";
        if (dw == ERROR_INVALID_PARAMETER) qCritical() << "ERROR_INVALID_PARAMETER";
        if (dw == NTE_BAD_ALGID) qCritical() << "NTE_BAD_ALGID";
        if (dw == NTE_BAD_FLAGS) qCritical() << "NTE_BAD_FLAGS";
        if (dw == NTE_BAD_HASH) qCritical() << "NTE_BAD_HASH";
        if (dw == NTE_BAD_HASH_STATE) qCritical() << "NTE_BAD_HASH_STATE";
        if (dw == NTE_BAD_UID) qCritical() << "NTE_BAD_UID";
        if (dw == NTE_FAIL) qCritical() << "NTE_FAIL";
        if (dw == NTE_SILENT_CONTEXT) qCritical() << "NTE_SILENT_CONTEXT";
        CryptDestroyHash(hHash);
        CryptReleaseContext(hProv, 0);
        return false;
    }


    DWORD mode = CRYPT_MODE_ECB;
    if (!CryptSetKeyParam(hKey, KP_MODE, reinterpret_cast<BYTE*>(&mode), 0)) {
        qCritical() << "Error during CryptSetKeyParam!" << GetLastError();
        CryptDestroyKey(hKey);
        CryptDestroyHash(hHash);
        CryptReleaseContext(hProv, 0);
        return false;
    }


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


    DWORD dataLen = encryptedData.size();
    QByteArray decryptedData(dataLen, 0);
    memcpy(decryptedData.data(), encryptedData.data(), dataLen);

    if (!CryptDecrypt(hKey, 0, TRUE, 0, reinterpret_cast<BYTE*>(decryptedData.data()), &dataLen)) {
        qCritical() << "Error during CryptDecrypt!" << GetLastError();
        CryptDestroyKey(hKey);
        CryptDestroyHash(hHash);
        CryptReleaseContext(hProv, 0);
        return false;
    }


    QFile outputFile(outputFilePath);
    if (!outputFile.open(QIODevice::WriteOnly)) {
        qCritical() << "Error opening output file!";
        CryptDestroyKey(hKey);
        CryptDestroyHash(hHash);
        CryptReleaseContext(hProv, 0);
        return false;
    }
    outputFile.write(decryptedData.left(dataLen));
    outputFile.close();


    CryptDestroyKey(hKey);
    CryptDestroyHash(hHash);
    CryptReleaseContext(hProv, 0);

    return true;
}

