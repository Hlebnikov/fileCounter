#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>
#include <QFileDialog>
#include <QDebug>
#include <QRegExp>
#include <QDate>
#include <QFile>
#include "qexifimageheader_original.h"

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
    void on_action_Excel_triggered();

private:
    QString path;

    Ui::MainWindow *ui;
    void getSubfolders(QString path);
    int countMatchedFiles(QString path, QRegExp re);
    int getPageCount(QString path);
    int mainMonth;
};

#endif // MAINWINDOW_H
