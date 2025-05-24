#include "widget.h"

#include <QTextEdit>
//#include <QWebEngineView>
#include <QSplitter>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>
#include <QHBoxLayout>
#include <QDir>
#include <QFileInfo>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);
    textEdit = new QTextEdit(this);
    preview = new QWebEngineView(this);

    splitter->addWidget(textEdit);
    splitter->addWidget(preview);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 3);

    layout->addWidget(splitter);

    QPushButton *openButton = new QPushButton("打开 .md 文件", this);
    QPushButton *saveButton = new QPushButton("保存为 .md 文件", this);
    QPushButton *pdfButton = new QPushButton("导出为 PDF", this);
    QPushButton *imageButton = new QPushButton("插入图片", this);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(openButton);
    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(pdfButton);
    buttonLayout->addWidget(imageButton);
    layout->addLayout(buttonLayout);

    connect(openButton, &QPushButton::clicked, this, &Widget::openMdFile);
    connect(saveButton, &QPushButton::clicked, this, &Widget::saveToFile);
    connect(pdfButton, &QPushButton::clicked, this, &Widget::exportToPdf);
    connect(textEdit, &QTextEdit::textChanged, this, &Widget::updatePreview);
    connect(imageButton, &QPushButton::clicked, this, &Widget::insertImage);

    updatePreview();
}

void Widget::saveToFile()
{
    QString fileName = QFileDialog::getSaveFileName(this, "保存为 Markdown 文件", "", "Markdown 文件 (*.md)");
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << textEdit->toPlainText();
            file.close();
        }
    }
}

void Widget::exportToPdf()
{
    QString fileName = QFileDialog::getSaveFileName(this, "导出为 PDF 文件", "", "PDF 文件 (*.pdf)");
    if (!fileName.isEmpty()) {
        preview->page()->printToPdf(fileName);
    }
}

void Widget::insertImage()
{
    QString sourcePath = QFileDialog::getOpenFileName(this, "选择图片", "", "Images (*.png *.jpg *.jpeg *.bmp *.gif)");
    if (sourcePath.isEmpty())
        return;

    QString exeDir = QCoreApplication::applicationDirPath();
    QDir dir(exeDir);

    if (!dir.cdUp() || !dir.cdUp() || !dir.cdUp()) {
        return;
    }
    QString projectRootPath = dir.absolutePath();

    QString imagesDirPath = dir.filePath("images");

    QDir imagesDir(imagesDirPath);
    if (!imagesDir.exists()) {
        imagesDir.mkpath(".");
    }

    QFileInfo fileInfo(sourcePath);
    QString baseName = fileInfo.fileName();

    QString targetPath = imagesDirPath + "/" + baseName;

    if (QFile::exists(targetPath)) {
        QFile::remove(targetPath);
    }

    if (!QFile::copy(sourcePath, targetPath)) {
        return;
    }

    QString markdownHtmlPath = exeDir + "/markdown.html";
    QFileInfo mdHtmlFileInfo(markdownHtmlPath);
    QString mdHtmlDir = mdHtmlFileInfo.absolutePath();

    QDir mdHtmlDirPath(mdHtmlDir);
    QString relativeImagePath = mdHtmlDirPath.relativeFilePath(targetPath);

    QString markdownImage = QString("\n\n![图片](%1)\n\n").arg(relativeImagePath);

    textEdit->insertPlainText(markdownImage);
    updatePreview();
}



void Widget::updatePreview()
{
    QString markdown = textEdit->toPlainText();
    QString escaped = markdown;
    escaped.replace("\\", "\\\\").replace("'", "\\'").replace("\n", "\\n");

    QString js = QString("updateMarkdown('%1');").arg(escaped);

    if (!previewLoaded) {
        preview->load(QUrl::fromLocalFile(QCoreApplication::applicationDirPath() + "/markdown.html"));
        connect(preview, &QWebEngineView::loadFinished, this, [=](bool){
            preview->page()->runJavaScript(js);
            previewLoaded = true;
        });
    } else {
        preview->page()->runJavaScript(js);
    }
}

void Widget::openMdFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, "打开 Markdown 文件", "", "Markdown 文件 (*.md)");
    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    QString content = in.readAll();
    file.close();

    textEdit->setPlainText(content);
    updatePreview();
}
