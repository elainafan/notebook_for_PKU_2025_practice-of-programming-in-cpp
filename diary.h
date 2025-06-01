 #ifndef DIARY_H
#define DIARY_H

#include <QString>
#include <QDateTime>
#include <QPixmap>
#include <QVector>

class Diary {
public:
    Diary(const QString& notebookName,
          const QDateTime& dateTime,
          const QString& title,
          const QString& markdownContent,
          const QVector<QPixmap>& images = {},
          const QString& username="",
          const QString& diaryType="");

    QString getNotebookName() const;
    QDate getDate() const;
    QTime getTime() const;
    QString getTitle() const;
    QVector<QPixmap> getImages() const;
    QString getMarkdownHtmlPreview(int lineCount = -1) const;
    QString getUsername() const;
    QString getDiaryType() const;
    QString getDateTime() const;

private:
    QString m_notebookName;
    QDateTime m_dateTime;
    QString m_title;
    QString m_markdownContent;
    QVector<QPixmap> m_images;
    QString m_username;
    QString m_diaryType;
};

#endif // DIARY_H
