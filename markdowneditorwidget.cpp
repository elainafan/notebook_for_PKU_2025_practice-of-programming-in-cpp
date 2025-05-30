#include "markdowneditorwidget.h"
#include <QVBoxLayout>
#include <QFileDialog>
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <QWebChannel>

MarkdownEditorWidget::MarkdownEditorWidget(QWidget *parent)
    : QWidget(parent)
{
    editor = new QPlainTextEdit(this);
    preview = new QWebEngineView(this);
    saveButton = new QPushButton("保存为 .md", this);
    exportButton = new QPushButton("导出为 PDF", this);

    setupLayout();
    setupConnections();

    // 加载本地 html 页面（内含 marked.js 和 webchannel.js）
    preview->load(QUrl::fromLocalFile(QCoreApplication::applicationDirPath() + "/markdown.html"));
}

void MarkdownEditorWidget::setupLayout()
{
    auto *layout = new QVBoxLayout(this);
    auto *split = new QSplitter(Qt::Horizontal, this);
    split->addWidget(editor);
    split->addWidget(preview);
    split->setStretchFactor(0, 1);
    split->setStretchFactor(1, 1);

    layout->addWidget(split);
    layout->addWidget(saveButton);
    layout->addWidget(exportButton);
    setLayout(layout);
}

void MarkdownEditorWidget::setupConnections()
{
    connect(editor, &QPlainTextEdit::textChanged, this, &MarkdownEditorWidget::updatePreview);
    connect(saveButton, &QPushButton::clicked, this, &MarkdownEditorWidget::onSaveClicked);
    connect(exportButton, &QPushButton::clicked, this, &MarkdownEditorWidget::onExportPdfClicked);
}

void MarkdownEditorWidget::updatePreview()
{
    QString markdown = editor->toPlainText();
    QString js = QString("updateMarkdown(%1);")
                     .arg(QJsonDocument::fromVariant(markdown).toJson(QJsonDocument::Compact));
    preview->page()->runJavaScript(js);
}

void MarkdownEditorWidget::onSaveClicked()
{
    QString filePath = QFileDialog::getSaveFileName(this, "保存为 Markdown 文件", "", "*.md");
    if (!filePath.isEmpty()) {
        saveToMarkdown(filePath);
    }
}

void MarkdownEditorWidget::onExportPdfClicked()
{
    QString filePath = QFileDialog::getSaveFileName(this, "导出为 PDF 文件", "", "*.pdf");
    if (!filePath.isEmpty()) {
        preview->page()->printToPdf(filePath);
    }
}

void MarkdownEditorWidget::setDefaultContent(const QString &content)
{
    editor->setPlainText(content);
}

void MarkdownEditorWidget::saveToMarkdown(const QString &filePath)
{
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << editor->toPlainText();
    }
}
