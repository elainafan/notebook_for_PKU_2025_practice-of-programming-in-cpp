#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QLineEdit;
class QPushButton;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void browseFile();
    void encryptFile();
    void decryptFile();

private:
    void setupUI();

    QLineEdit *passwordInput;
    QLineEdit *filePathInput;
    QPushButton *browseButton;
    QPushButton *encryptButton;
    QPushButton *decryptButton;
};

#endif // MAINWINDOW_H
