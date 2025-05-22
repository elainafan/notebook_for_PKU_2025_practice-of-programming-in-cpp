#include "CryptoUtils.h"
#include <QCryptographicHash>
#include <QRandomGenerator>
#include <QFileInfo>

CryptoUtils::CryptoUtils(QObject *parent) : QObject(parent),
    m_aes(QAESEncryption::AES_256, QAESEncryption::CBC)
{
}

QByteArray CryptoUtils::generateSalt(int length)
{
    QByteArray salt;
    salt.resize(length);

    // 使用Qt的加密安全随机数生成器
    QRandomGenerator *generator = QRandomGenerator::system();
    for(int i = 0; i < length; ++i) {
        salt[i] = static_cast<char>(generator->generate());
    }

    return salt;
}

QByteArray CryptoUtils::deriveKey(const QString &password, const QByteArray &salt)
{
    const int iterations = 10000;
    QByteArray key = password.toUtf8() + salt;

    // 使用SHA-256进行密钥派生
    for(int i = 0; i < iterations; ++i) {
        key = QCryptographicHash::hash(key, QCryptographicHash::Sha256);
    }

    return key;
}

bool CryptoUtils::encryptFile(const QString &inputPath, const QString &outputPath, const QString &password)
{
    QFile inputFile(inputPath);
    if (!inputFile.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open input file:" << inputPath;
        return false;
    }

    QByteArray plaintext = inputFile.readAll();
    inputFile.close();

    FileHeader header;
    QFileInfo fi(inputPath);
    strncpy(header.format, fi.suffix().toLatin1().constData(), sizeof(header.format));
    header.original_size = plaintext.size();

    QByteArray headerBytes(reinterpret_cast<char*>(&header), sizeof(FileHeader));
    QByteArray salt = generateSalt();
    QByteArray key = deriveKey(password, salt);
    QByteArray iv = generateSalt(16); // AES块大小

    // 加密数据
    QByteArray encrypted = m_aes.encode(plaintext, key, iv);

    // 将头、盐值、key、IV和加密数据一起保存
    QByteArray outputData = headerBytes + salt + key + iv + encrypted;

    QFile outputFile(outputPath);
    if (!outputFile.open(QIODevice::WriteOnly)) {
        qWarning() << "Failed to open output file:" << outputPath;
        return false;
    }

    outputFile.write(outputData);
    outputFile.close();

    return true;
}

bool CryptoUtils::decryptFile(const QString &inputPath, const QString &outputPath, const QString &password)
{
    QFile inputFile(inputPath);
    if (!inputFile.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open input file:" << inputPath;
        return false;
    }

    QByteArray encryptedData = inputFile.readAll();
    inputFile.close();

    if (encryptedData.size() < 64 + sizeof(FileHeader)) { // FileHeader + 盐值(16) + key(32) + IV(16)
        qWarning() << "Invalid encrypted file format";
        return false;
    }

    FileHeader header;
    memcpy(&header, encryptedData.constData(), sizeof(FileHeader));
    QByteArray salt = encryptedData.mid(sizeof(FileHeader),16);
    QByteArray realkey = encryptedData.mid(sizeof(FileHeader)+16,32);
    QByteArray iv = encryptedData.mid(sizeof(FileHeader)+48, 16);
    QByteArray ciphertext = encryptedData.mid(sizeof(FileHeader)+64);

    QByteArray key = deriveKey(password, salt);
    if (key.isEmpty()||key!=realkey) {
        return false;
    }

    // 验证魔数
    if(memcmp(header.magic, "FMT@", 4) != 0) {
        qWarning() << "Magic number mismatch";
        return false;
    }

    // 解密数据
    QByteArray decrypted = m_aes.decode(ciphertext, key, iv);

    // 移除可能的填充
    decrypted = m_aes.removePadding(decrypted);

    // 恢复原始文件名和格式
    QString format = QString::fromLatin1(header.format).trimmed();
    QString finalPath = outputPath + "." + format;

    QFile outputFile(finalPath);
    if (!outputFile.open(QIODevice::WriteOnly)) {
        qWarning() << "Failed to open output file:" << outputPath;
        return false;
    }

    outputFile.write(decrypted);
    outputFile.close();

    return true;
}
