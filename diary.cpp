// diary.cpp
#include "diary.h"
#include <QTextDocument>

Diary::Diary(const QString& notebookName,
             const QDateTime& dateTime,
             const QString& title,
             const QString& markdownContent,
             const QVector<QPixmap>& images)
    : m_notebookName(notebookName),
    m_dateTime(dateTime),
    m_title(title),
    m_markdownContent(markdownContent),
    m_images(images) {}

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

QString Diary::getMarkdownHtmlPreview(int lineCount) const {
    QStringList lines = m_markdownContent.split('\n');
    if (lineCount > 0 && lineCount < lines.size()) {
        lines = lines.mid(0, lineCount);
    }
    QString partialMarkdown = lines.join('\n');

    QTextDocument doc;
    doc.setMarkdown(partialMarkdown);  // Qt 6 专属
    return doc.toHtml();
}
