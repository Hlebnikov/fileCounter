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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this,"Папка со съемками");
    ui->tableWidget->clear();
    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "Папка" << "Портреты" << "Репортаж" << "Учителя" <<"Стоимость");
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
    int summ=0;
    foreach (QString subfolder, subfolders) {
        QString subdir(path+"/"+subfolder);
        ui->tableWidget->setItem(i,0,new QTableWidgetItem(subfolder));
        qDebug() << subdir.count()-2;
        QStringList portraits = getFiles(path+"/"+subfolder,portrait);
        QStringList teachers = getFiles(path+"/"+subfolder,teacher);
        QStringList reports = getFiles(path+"/"+subfolder,report);
        int p = portraits.count();
        int r = reports.count();
        int t = teachers.count();
        ui->tableWidget->setItem(i,1,new QTableWidgetItem(QString::number(p)));
        ui->tableWidget->setItem(i,2,new QTableWidgetItem(QString::number(r)));
        ui->tableWidget->setItem(i,3,new QTableWidgetItem(QString::number(t)));
        int oneDayCost = p*50+r*30+t*25;
        ui->tableWidget->setItem(i,4,new QTableWidgetItem(QString::number(oneDayCost)));
        summ+=oneDayCost;
        i++;
    }
    ui->label->setText(QString::number(summ));

}

QStringList MainWindow::getFiles(QString path, QRegExp re)
{
    QStringList out;
    QDir dir(path);
    QStringList subDirs = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    QStringList files= dir.entryList(QDir::Files);
    if (subDirs.count() > 0){
        foreach (QString subDir, subDirs) {
            QString subPath = path+"/"+subDir;
            files += getFiles(subPath,re);
        }
    }

    foreach (QString file, files) {
        if (re.exactMatch(file)) out+=file;
    }

    return out;
}
