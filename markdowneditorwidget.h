#ifndef MARKDOWNEDITORWIDGET_H
#define MARKDOWNEDITORWIDGET_H

#include <QWidget>
#include <QPlainTextEdit>
#include <QWebEngineView>
#include <QPushButton>

class MarkdownEditorWidget : public QWidget {
    Q_OBJECT

public:
    explicit MarkdownEditorWidget(QWidget *parent = nullptr);

    void setDefaultContent(const QString &content);
    void saveToMarkdown(const QString &filePath);
    void exportToPdf(const QString &pdfPath);

private slots:
    void updatePreview();
    void onSaveClicked();
    void onExportPdfClicked();

private:
    QPlainTextEdit *editor;
    QWebEngineView *preview;
    QPushButton *saveButton;
    QPushButton *exportButton;

    void setupLayout();
    void setupConnections();
};

#endif // MARKDOWNEDITORWIDGET_H
