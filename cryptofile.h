#ifndef CRYPTOFILE_H
#define CRYPTOFILE_H

#include <QFile>
#include <QCryptographicHash>
#include <QDebug>
#include <QByteArray>
#include <QDataStream>
#include <QCoreApplication>
#include <QVector>

#include <Windows.h>
#include <wincrypt.h>

#pragma comment(lib, "advapi32.lib")
#define MD2_DIGEST_LENGTH 16

class CryptoFile
{
public:
    CryptoFile();
    bool encryptFile(const QString& inputFilePath, const QString& outputFilePath, const QString& password);
    bool decryptFile(const QString& inputFilePath, const QString& outputFilePath, const QString& password);
};

#endif // CRYPTOFILE_H
