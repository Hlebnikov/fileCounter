#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>
#include <QFileDialog>
#include <QDebug>
#include <QRegExp>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    void getSubfolders(QString path);
    QStringList getFiles(QString path, QRegExp re);
};

#endif // MAINWINDOW_H
