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
    QString html = doc.toHtml();
    qDebug()<<Qt::endl<<html;

    int scaleRate = 2;
    // 1. 放大 px/pt 单位
    QRegularExpression rePxPt(R"(font-size\s*:\s*(\d+)(px|pt))");
    QString newHtml;
    int lastPos = 0;
    auto it = rePxPt.globalMatch(html);
    while (it.hasNext()) {
        auto match = it.next();
        int oldSize = match.captured(1).toInt();
        QString unit = match.captured(2);
        int newSize = int(oldSize * scaleRate);
        newHtml += html.mid(lastPos, match.capturedStart() - lastPos);
        newHtml += QString("font-size:%1%2").arg(newSize).arg(unit);
        lastPos = match.capturedEnd();
    }
    newHtml += html.mid(lastPos);
    html = newHtml;

    // 2. 替换 font-size: 关键字为具体 pt 值
    QMap<QString, double> sizeMap = {
        {"xx-small", 0.578}, {"x-small", 0.694}, {"small", 0.833}, {"medium", 1.0},
        {"large", 1.2}, {"x-large", 1.44}, {"xx-large", 1.728}
    };
    double basePt = 13.0 * scaleRate; // body已放大3倍
    QRegularExpression reKeyword(R"(font-size\s*:\s*(xx-large|x-large|large|medium|small|x-small|xx-small))", QRegularExpression::CaseInsensitiveOption);

    QString result;
    lastPos = 0;
    auto it2 = reKeyword.globalMatch(html);
    while (it2.hasNext()) {
        auto match = it2.next();
        QString key = match.captured(1).toLower();
        double pt = basePt * sizeMap.value(key, 1.0);
        result += html.mid(lastPos, match.capturedStart() - lastPos);
        result += QString("font-size:%1pt").arg(int(pt));
        lastPos = match.capturedEnd();
    }
    result += html.mid(lastPos);
    html = result;

    QStringList tags = {"h1", "h2", "h3", "h4", "h5", "h6", "pre", "blockquote"};
    for (const QString& tag : tags) {
        // 开始标签，保留属性
        html.replace(QRegularExpression(QString(R"(<%1([^>]*)>)").arg(tag), QRegularExpression::CaseInsensitiveOption), "<p\\1>");
        // 结束标签
        html.replace(QRegularExpression(QString(R"(</%1>)").arg(tag), QRegularExpression::CaseInsensitiveOption), "</p>");
    }

    qDebug()<<Qt::endl<<html;
    return html;
}
