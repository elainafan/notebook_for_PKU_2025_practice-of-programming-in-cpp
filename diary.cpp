// diary.cpp
#include "diary.h"
#include <QTextDocument>
#include <QRegularExpression>

Diary::Diary(const QString& notebookName,
             const QDateTime& dateTime,
             const QString& title,
             const QString& markdownContent,
             const QVector<QPixmap>& images,
             const QString& username,
             const QString& diaryType)
    : m_notebookName(notebookName),
    m_dateTime(dateTime),
    m_title(title),
    m_markdownContent(markdownContent),
    m_images(images),
    m_username(username),
    m_diaryType(diaryType){}

QString Diary::getNotebookName() const {
    return m_notebookName;
}

QDate Diary::getDate() const {
    return m_dateTime.date();
}

QTime Diary::getTime() const {
    return m_dateTime.time();
}

QString Diary::getTitle() const {
    return m_title;
}

QVector<QPixmap> Diary::getImages() const {
    return m_images;
}

QString Diary::getUsername() const {
    return m_username;
}

QString Diary::getDiaryType() const {
    return m_diaryType;
}

QString Diary::getDateTime() const {
    return m_dateTime.toString("yyyy_MM_dd_HH_mm_ss");
}

QString Diary::getMarkdownHtmlPreview(int lineCount) const {
    QStringList lines = m_markdownContent.split('\n');
    if (lineCount > 0 && lineCount < lines.size()) {
        lines = lines.mid(0, lineCount);
    }
    QString partialMarkdown = lines.join('\n');

    // 移除图片 Markdown 语法，例如 ![描述](路径)
    QRegularExpression imageRegex(R"(!\[[^\]]*\]\([^\)]*\))");
    partialMarkdown.remove(imageRegex);

    QTextDocument doc;
    doc.setMarkdown(partialMarkdown);  // Qt 6 专属
    return doc.toHtml();
}
