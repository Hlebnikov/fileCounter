#include "mainwindow.h"
#include "ui_mainwindow.h"

const QRegExp teacher("[^A-Z0-9]*[,][^A-Z0-9]*(jpg|jpeg)",Qt::CaseInsensitive);
const QRegExp portrait("[^A-Z0-9,]*(jpg|jpeg)",Qt::CaseInsensitive);
const QRegExp report("[0-9.]*(jpg|jpeg)",Qt::CaseInsensitive);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "Папка" << "Портреты" << "Репортаж" << "Учителя" <<"Стоимость");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this,"Папка со съмками");
    getSubfolders(path);
}

void MainWindow::getSubfolders(QString path)
{
    ui->pushButton->setText(path);
    QDir dir(path);
    int subfoldersCount = dir.count()-2;
    ui->tableWidget->setRowCount(100);

//    portrait.setPatternSyntax(QRegExp::Wildcard);
    qDebug() << subfoldersCount;
    QStringList subfolders = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    int i=0;
    foreach (QString subfolder, subfolders) {
        QDir subdir(path+"/"+subfolder);
        ui->tableWidget->setItem(i,0,new QTableWidgetItem(subfolder));
        qDebug() << subdir.count()-2;
        QStringList files = subdir.entryList(QDir::Files | QDir::NoDotAndDotDot);
        int p=0,t=0,r=0;
        foreach (QString file, files) {
            if (report.exactMatch(file))r++;
            else if (portrait.exactMatch(file))p++;
            else if (teacher.exactMatch(file))t++;
        }
        ui->tableWidget->setItem(i,1,new QTableWidgetItem(QString::number(p)));
        ui->tableWidget->setItem(i,2,new QTableWidgetItem(QString::number(r)));
        ui->tableWidget->setItem(i++,3,new QTableWidgetItem(QString::number(t)));
    }
    qDebug() << subfolders;
}
