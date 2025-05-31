#ifndef MARKDOWNEDITORWIDGET_H
#define MARKDOWNEDITORWIDGET_H

#include <QWidget>
#include <QPlainTextEdit>
//#include <QWebEngineView>
#include <QPushButton>
#include "diary.h"

class MarkdownEditorWidget : public QWidget {
    Q_OBJECT

public:
    explicit MarkdownEditorWidget(const Diary &diary,QWidget *parent = nullptr);

    void setDefaultContent(const QString &content);
    void saveToMarkdown(const QString &filePath);
    void exportToPdf(const QString &pdfPath);

signals:
    void saved();

private slots:
    void updatePreview();
    void onSaveClicked();
    void onExportPdfClicked();
    void onInsertImageClicked();

private:
    QPlainTextEdit *editor;
    //QWebEngineView *preview;
    QPushButton *saveButton;
    QPushButton *exportButton;
    QPushButton *insertImageButton;

    QString username;
    QString diaryType;
    QString targetDir;
    QString currentTimestamp;
    QString currentMarkdownName;  // 对应 markdown 文件名（无扩展名），用于图片子目录命名
    QString imageDirBase;         // 图片存储的 base 路径（不含最后的图片子文件夹）

    void setupLayout();
    void setupConnections();
    void determineTargetDirectory();
};

#endif // MARKDOWNEDITORWIDGET_H
