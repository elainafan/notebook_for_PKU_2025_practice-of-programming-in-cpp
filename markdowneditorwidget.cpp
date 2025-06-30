#include "markdowneditorwidget.h"
#include <QVBoxLayout>
#include <QFileDialog>
#include <QDateTime>
#include <QFile>
#include <QTextStream>
//#include <QWebChannel>
#include <QSplitter>
#include <QMessageBox>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QRegularExpressionMatchIterator>

MarkdownEditorWidget::MarkdownEditorWidget(const Diary &diary, QWidget *parent)
    : QWidget(parent), username(diary.getUsername()), diaryType(diary.getDiaryType()), NotebookName(diary.getNotebookName())
{
    setFixedSize(800,900);
    editor = new QPlainTextEdit(this);
    //preview = new QWebEngineView(this);
    saveButton = new QPushButton("保存", this);
    exportButton = new QPushButton("导出为 PDF", this);
    insertImageButton = new QPushButton("插入图片", this);
    deleteButton = new QPushButton("删除日记", this);
    QString timestamp = diary.getDateTime();
    currentMarkdownName = timestamp;

    setupLayout();
    setupConnections();
    determineTargetDirectory();
    openMarkdownFile();

    // 加载本地 html 页面（内含 marked.js 和 webchannel.js）
    //preview->load(QUrl::fromLocalFile(QCoreApplication::applicationDirPath() + "/markdown.html"));

    applyStyle();
}

void MarkdownEditorWidget::applyStyle()
{
    // 设置整体背景色
    this->setStyleSheet("background-color: #f7f7fa;");

    // 编辑器美化
    editor->setStyleSheet(
        "QPlainTextEdit {"
        "  background: #ffffff;"
        "  border: 1px solid #d0d0d0;"
        "  font-size: 24px;"
        "  font-family: 'Consolas', '微软雅黑', monospace;"
        "  padding: 8px;"
        "  border-radius: 6px;"
        "}"
    );

    // 按钮统一样式
    QString btnStyle =
        "QPushButton {"
        "  background-color: #7030A0;"
        "  color: white;"
        "  border: none;"
        "  border-radius: 5px;"
        "  padding: 8px 18px;"
        "  font-size: 15px;"
        "  margin: 4px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #602090;"
        "}"
        "QPushButton:pressed {"
        "  background-color: #501080;"
        "}";

    saveButton->setStyleSheet(btnStyle);
    exportButton->setStyleSheet(btnStyle);
    insertImageButton->setStyleSheet(btnStyle);
    deleteButton->setStyleSheet(btnStyle);

    // 预览区边框
    /*preview->setStyleSheet(
        "QWebEngineView {"
        "  border: 1px solid #d0d0d0;"
        "  border-radius: 6px;"
        "  background: #fcfcfc;"
        "}"
    );*/
}

void MarkdownEditorWidget::setupLayout()
{
    auto *layout = new QVBoxLayout(this);
    auto *split = new QSplitter(Qt::Horizontal, this);
    split->addWidget(editor);
    //split->addWidget(preview);
    split->setStretchFactor(0, 1);
    split->setStretchFactor(1, 3);

    layout->addWidget(split);
    layout->addWidget(insertImageButton);
    layout->addWidget(saveButton);
    layout->addWidget(exportButton);
    layout->addWidget(deleteButton);
    setLayout(layout);
}

void MarkdownEditorWidget::setupConnections()
{
    connect(editor, &QPlainTextEdit::textChanged, this, &MarkdownEditorWidget::updatePreview);
    connect(saveButton, &QPushButton::clicked, this, &MarkdownEditorWidget::onSaveClicked);
    connect(exportButton, &QPushButton::clicked, this, &MarkdownEditorWidget::onExportPdfClicked);
    connect(insertImageButton, &QPushButton::clicked, this, &MarkdownEditorWidget::onInsertImageClicked);
    connect(deleteButton, &QPushButton::clicked, this, &MarkdownEditorWidget::onDeleteDiaryClicked);
}

void MarkdownEditorWidget::determineTargetDirectory()
{
    QString basePath = QDir(QCoreApplication::applicationDirPath()).absolutePath();
    QString subFolder;
    if (diaryType == "daily") subFolder = "daily";
    else if (diaryType == "weekly") subFolder = "weekly";
    else if (diaryType == "monthly") subFolder = "monthly";
    else if (diaryType == "yearly") subFolder = "yearly";
    subFolder+="_"+NotebookName;
    targetDir = basePath + "/" + username + "/diary/" + subFolder;
    imageDirBase = basePath + "/" + username + "/picture/" + subFolder;
}

void MarkdownEditorWidget::openMarkdownFile()
{
    // 构造文件路径
    QString basePath = QCoreApplication::applicationDirPath();
    QDir baseDir(basePath);
    QString subFolder;
    if (diaryType == "daily") subFolder = "daily";
    else if (diaryType == "weekly") subFolder = "weekly";
    else if (diaryType == "monthly") subFolder = "monthly";
    else if (diaryType == "yearly") subFolder = "yearly";
    subFolder+="_"+NotebookName;
    QString filePath = baseDir.absoluteFilePath(username + "/diary/" + subFolder + "/"  + currentMarkdownName + ".md");
    qDebug() << "打开路径: " << filePath;

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QTextStream in(&file);
    QString content = in.readAll();
    editor->setPlainText(content);

    // ✅ 这里初始化图片路径
    oldImagePaths = extractImagePaths(content);
    qDebug() << "[初始化 oldImagePaths]";
    for (const QString &p : oldImagePaths)
        qDebug() << "  - " << p;

    file.close();

    updatePreview();
}


void MarkdownEditorWidget::updatePreview()
{
    // 第一步：设置图片 baseHref

    QString baseHref = QUrl::fromLocalFile(QCoreApplication::applicationDirPath()).toString();  // 指向 debug/
    if (!baseHref.endsWith("/")) {
        baseHref += "/";
    }
    QString setBaseJs = QString("setBaseHrefFromQt(\"%1\");").arg(baseHref);
    //preview->page()->runJavaScript(setBaseJs);

    // 第二步：使用你原有方式转义并调用 updateMarkdown
    QString markdown = editor->toPlainText();
    QString escaped = markdown;
    escaped.replace("\\", "\\\\")
        .replace("\"", "\\\"")
        .replace("\n", "\\n")
        .replace("\r", "");
    QString js = QString("updateMarkdown(\"%1\");").arg(escaped);
    //preview->page()->runJavaScript(js);
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
        // 构造相对路径：以 debug 为起点
        QString relativePath = QString("%1/picture/%2/%3/%4")
                                   .arg(username)
                                   .arg(diaryType + "_" + NotebookName)
                                   .arg(currentMarkdownName)
                                   .arg(fileInfo.fileName());

        QString markdownImage = QString("![%1](%2)").arg(fileInfo.fileName(), relativePath);
        editor->insertPlainText("\n" + markdownImage + "\n");
    }
}

QStringList MarkdownEditorWidget::extractImagePaths(const QString &markdownText)
{
    QStringList paths;
    QRegularExpression re("!\\[[^\\]]*\\]\\(([^)]+)\\)");
    QRegularExpressionMatchIterator i = re.globalMatch(markdownText);
    while (i.hasNext()) {
        QRegularExpressionMatch match = i.next();
        QString path = match.captured(1);
        paths.append(path);
    }
    return paths;
}

void MarkdownEditorWidget::deleteImageFile(const QString &relativePath)
{
    // 转换相对路径为绝对路径，以debug目录为基准
    QString basePath = QCoreApplication::applicationDirPath()+"/";
    qDebug()<<basePath<<Qt::endl;
    QString absPath =basePath+relativePath;
    qDebug()<<absPath<<Qt::endl;
    if (QFile::exists(absPath)) {
        bool ok = QFile::remove(absPath);
        qDebug() << "删除图片文件:" << absPath << (ok ? "成功" : "失败");
    }
}

void MarkdownEditorWidget::onSaveClicked()
{   
    QString markdownText = editor->toPlainText();
    QStringList currentImagePaths = extractImagePaths(markdownText);

    qDebug() << "[调试] 提取出的图片路径:";
    for (const QString &p : currentImagePaths) {
        qDebug() << "  - " << p;
    }
    // 删除不再引用的图片
    for (const QString &oldPath : oldImagePaths) {
        if (!currentImagePaths.contains(oldPath)) {
            deleteImageFile(oldPath);
        }
    }

    oldImagePaths = currentImagePaths;  // 更新记录

    QString fileName = currentMarkdownName + ".md";
    QString filePath = targetDir + "/" + fileName;
    saveToMarkdown(filePath);
}

void MarkdownEditorWidget::onExportPdfClicked()
{
    QString filePath = QFileDialog::getSaveFileName(this, "导出为 PDF 文件", "", "*.pdf");
    if (!filePath.isEmpty()) {
        //preview->page()->printToPdf(filePath);
    }
}

void MarkdownEditorWidget::setDefaultContent(const QString &content)
{
    editor->setPlainText(content);
}

void MarkdownEditorWidget::saveToMarkdown(const QString &filePath)
{
    qDebug()<<"try to save at "<<filePath;
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << editor->toPlainText();
        emit saved();
        qDebug()<<"saved";
    }
}
void MarkdownEditorWidget::onDeleteDiaryClicked()
{
    // 确认是否删除
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "删除确认", "确定要删除本篇日记及相关图片吗？",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply != QMessageBox::Yes)
        return;

    // 构造路径
    QString mdFilePath = targetDir + "/" + currentMarkdownName + ".md";
    QString imageFolderPath = imageDirBase + "/" + currentMarkdownName;

    // 删除 .md 文件
    bool mdDeleted = QFile::remove(mdFilePath);

    // 删除图片文件夹（含内容）
    QDir imgDir(imageFolderPath);
    bool imgDeleted = imgDir.removeRecursively();

    // 给出结果提示
    if (mdDeleted || imgDeleted) {
        QMessageBox::information(this, "删除成功", "日记文件和图片已删除。");
        close();  // 可选：关闭当前窗口
        emit deleted();
    } else {
        QMessageBox::warning(this, "删除失败", "未能删除部分或全部内容。");
    }
}
