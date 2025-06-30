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
    void deleted();

private slots:
    void updatePreview();
    void onSaveClicked();
    void onExportPdfClicked();
    void onInsertImageClicked();
    void onDeleteDiaryClicked();
    void applyStyle();

private:
    QPlainTextEdit *editor;
//    QWebEngineView *preview;
    QPushButton *saveButton;
    QPushButton *exportButton;
    QPushButton *insertImageButton;
    QPushButton *deleteButton;

    QStringList oldImagePaths;
    QString username;
    QString diaryType;
    QString targetDir;
    QString currentTimestamp;
    QString currentMarkdownName;  // 对应 markdown 文件名（无扩展名），用于图片子目录命名
    QString imageDirBase;         // 图片存储的 base 路径（不含最后的图片子文件夹）
    QString NotebookName;

    void setupLayout();
    void setupConnections();
    void determineTargetDirectory();
    void openMarkdownFile();
    QStringList extractImagePaths(const QString &markdownText);
    void deleteImageFile(const QString &relativePath);
};

#endif // MARKDOWNEDITORWIDGET_H
