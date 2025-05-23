#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

class QTextEdit;
class QWebEngineView;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);

private slots:
    void updatePreview();
    void saveToFile();
    void exportToPdf();
    void insertImage();
    void openMdFile();

private:
    QTextEdit *textEdit;
    QWebEngineView *preview;
    bool previewLoaded = false;
};

#endif // WIDGET_H
