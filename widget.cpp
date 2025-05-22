#include "widget.h"

#include <QTextEdit>
#include <QWebEngineView>
#include <QSplitter>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>

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
    splitter->setStretchFactor(1, 1);

    layout->addWidget(splitter);

    QPushButton *saveButton = new QPushButton("保存为 .md 文件", this);
    layout->addWidget(saveButton);

    connect(saveButton, &QPushButton::clicked, this, &Widget::saveToFile);
    connect(textEdit, &QTextEdit::textChanged, this, &Widget::updatePreview);

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
