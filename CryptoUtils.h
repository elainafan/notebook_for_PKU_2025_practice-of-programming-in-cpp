#ifndef CRYPTOUTILS_H
#define CRYPTOUTILS_H

#include <QObject>
#include <QByteArray>
#include <QFile>
#include <QDebug>
#include "qaesencryption.h"

class CryptoUtils : public QObject
{
    Q_OBJECT
public:
    explicit CryptoUtils(QObject *parent = nullptr);

    bool encryptFile(const QString &inputPath, const QString &outputPath, const QString &password);
    bool decryptFile(const QString &inputPath, const QString &outputPath, const QString &password);


private:
    struct FileHeader{
        char magic[4] = {'F','M','T','@'}; // 4字节
        char format[8];                    // 8字节
        uint32_t original_size;            // 4字节
    };// 总长度 16 字节

    QByteArray generateSalt(int length = 16);
    QByteArray deriveKey(const QString &password, const QByteArray &salt);

    QAESEncryption m_aes;
};

#endif // CRYPTOUTILS_H
