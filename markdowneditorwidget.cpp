#include "markdowneditorwidget.h"
#include <QVBoxLayout>
#include <QFileDialog>
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <QWebChannel>
#include <QSplitter>
#include <QMessageBox>

MarkdownEditorWidget::MarkdownEditorWidget(const Diary &diary, QWidget *parent)
    : QWidget(parent), username(diary.getUsername()), diaryType(diary.getDiaryType())
{
    setFixedSize(800,900);
    editor = new QPlainTextEdit(this);
    preview = new QWebEngineView(this);
    saveButton = new QPushButton("保存", this);
    exportButton = new QPushButton("导出为 PDF", this);
    insertImageButton = new QPushButton("插入图片", this);
    QString timestamp = diary.getDateTime();
    currentMarkdownName = timestamp;

    setupLayout();
    setupConnections();
    determineTargetDirectory();
    openMarkdownFile();

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
    split->setStretchFactor(1, 3);

    layout->addWidget(split);
    layout->addWidget(insertImageButton);
    layout->addWidget(saveButton);
    layout->addWidget(exportButton);
    setLayout(layout);
}

void MarkdownEditorWidget::setupConnections()
{
    connect(editor, &QPlainTextEdit::textChanged, this, &MarkdownEditorWidget::updatePreview);
    connect(saveButton, &QPushButton::clicked, this, &MarkdownEditorWidget::onSaveClicked);
    connect(exportButton, &QPushButton::clicked, this, &MarkdownEditorWidget::onExportPdfClicked);
    connect(insertImageButton, &QPushButton::clicked, this, &MarkdownEditorWidget::onInsertImageClicked);
}

void MarkdownEditorWidget::determineTargetDirectory()
{
    QString basePath = QDir(QCoreApplication::applicationDirPath()).absolutePath();
    basePath = QDir(basePath).absoluteFilePath("..");  // 向上一级目录
    QString subFolder;
    if (diaryType == "daily") subFolder = "daily_日记";
    else if (diaryType == "weekly") subFolder = "weekly_周记";
    else if (diaryType == "monthly") subFolder = "monthly_月记";
    else if (diaryType == "yearly") subFolder = "yearly_年记";
    targetDir = basePath + "/" + username + "/diary/" + subFolder;
    imageDirBase = basePath + "/" + username + "/picture/" + subFolder;
}

void MarkdownEditorWidget::openMarkdownFile()
{
    // 构造文件路径
    QString basePath = QCoreApplication::applicationDirPath();
    QDir baseDir(basePath);
    baseDir.cdUp(); // 从 debug 回到上一级（Desktop_Qt_6_9_0_MSVC2022_64bit-Debug）
    QString subFolder;
    if (diaryType == "daily") subFolder = "daily_日记";
    else if (diaryType == "weekly") subFolder = "weekly_周记";
    else if (diaryType == "monthly") subFolder = "monthly_月记";
    else if (diaryType == "yearly") subFolder = "yearly_年记";
    QString filePath = baseDir.absoluteFilePath(username + "/diary/" + subFolder + "/"  + currentMarkdownName + ".md");
    qDebug() << "打开路径: " << filePath;

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "错误", "无法打开文件: " + filePath);
        return;
    }

    QTextStream in(&file);
    QString content = in.readAll();
    editor->setPlainText(content);

    file.close();

    updatePreview();
}


void MarkdownEditorWidget::updatePreview()
{
    // 第一步：设置图片 baseHref

    QString baseHref = QUrl::fromLocalFile(imageDirBase + "/" + currentMarkdownName + "/").toString();
    QString setBaseJs = QString("setBaseHrefFromQt(\"%1\");").arg(baseHref);
    preview->page()->runJavaScript(setBaseJs);

    // 第二步：使用你原有方式转义并调用 updateMarkdown
    QString markdown = editor->toPlainText();
    QString escaped = markdown;
    escaped.replace("\\", "\\\\")
        .replace("\"", "\\\"")
        .replace("\n", "\\n")
        .replace("\r", "");
    QString js = QString("updateMarkdown(\"%1\");").arg(escaped);
    preview->page()->runJavaScript(js);
}

void MarkdownEditorWidget::onInsertImageClicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "选择图片", "", "Images (*.png *.jpg *.jpeg *.bmp *.gif)");
    if (filePath.isEmpty()) return;

    QFileInfo fileInfo(filePath);
    QString imageSubDir = imageDirBase + "/" + currentMarkdownName;
    QDir().mkpath(imageSubDir);
    QString destPath = imageSubDir + "/" + fileInfo.fileName();

    if (QFile::copy(filePath, destPath)) {
        // 插入图片的绝对 file:// 路径
        QString absPath = QUrl::fromLocalFile(destPath).toString();
        QString markdownImage = QString("![%1](%2)").arg(fileInfo.fileName(), absPath);
        editor->insertPlainText("\n" + markdownImage + "\n");
    }
}

void MarkdownEditorWidget::onSaveClicked()
{
    QString fileName = currentMarkdownName + ".md";
    QString filePath = targetDir + "/" + fileName;
    saveToMarkdown(filePath);
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
        emit saved();
    }
}

