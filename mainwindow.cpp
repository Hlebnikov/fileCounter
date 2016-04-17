#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "xlsxdocument.h"

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
    path = QFileDialog::getExistingDirectory(this,"Папка со съемками");
//    path = "D:/Рабочая/Февраль";
    ui->tableWidget->clear();
    ui->tableWidget->setColumnCount(7);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "Дата" << "Папка" << "Развороты" << "Портреты" << "Репортаж" << "Учителя" <<"Стоимость");
    getSubfolders(path);

}

void MainWindow::on_action_Excel_triggered()
{
    qDebug() << "Save to Excel";
    QXlsx::Document xlsx;
    xlsx.write("A1", "Александр - Февраль");
    xlsx.write("A2", "Дата");
    xlsx.setColumnWidth(3,7,10);
    xlsx.setColumnWidth(2,2,24);
    xlsx.setColumnWidth(1,1,12);
    xlsx.write("B2", "Съемка");
    xlsx.write("C2", "Развороты");
    xlsx.write("D2", "Портреты");
    xlsx.write("E2", "Репортаж");
    xlsx.write("F2", "Учителя");
    xlsx.write("G2", "Стоимость");
    int rowsCount = ui->tableWidget->rowCount();
    for (int row=0; row<rowsCount-1; row++){
        for (int column=0; column<7; column++){
            if (ui->tableWidget->item(row,column) != NULL)
//                qDebug() << ui->tableWidget->item(row,column)->text();
            xlsx.write(row+3,column+1,ui->tableWidget->item(row,column)->text());
        }
    }
    xlsx.write(rowsCount+3,6,"Всего:");
    xlsx.write(rowsCount+3,7,ui->label->text());
    xlsx.saveAs(path+"/"+path.split("/").back()+".xlsx");
}

void MainWindow::getSubfolders(QString path)
{
    ui->pushButton->setText(path);
    QDir dir(path);
    int subfoldersCount = dir.count()-2;


//    portrait.setPatternSyntax(QRegExp::Wildcard);
//    qDebug() << subfoldersCount;
    QStringList subfolders = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    ui->tableWidget->setRowCount(dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot).count()+1);
    int i=0;
    int summ=0;

    QSet<QDate> dates;

    foreach (QString subfolder, subfolders) {
        QString subdir(path+"/"+subfolder);
        int pages = getPageCount(subdir);
        QFileInfo fileInfo(subdir);
        QDate createDate = fileInfo.created().date();
//        qDebug() << createDate.toString();
        dates << createDate;
        ui->tableWidget->setItem(i,0, new QTableWidgetItem(createDate.toString(Qt::SystemLocaleShortDate)));
//        qDebug() << pages;
        ui->tableWidget->setItem(i,1,new QTableWidgetItem(subfolder));
        ui->tableWidget->setItem(i,2,new QTableWidgetItem(QString::number(pages)));
//        qDebug() << subdir.count()-2;
        QStringList portraits = getFiles(path+"/"+subfolder,portrait);
        QStringList teachers = getFiles(path+"/"+subfolder,teacher);
        QStringList reports = getFiles(path+"/"+subfolder,report);
        int p = portraits.count();
        int r = reports.count();
        int t = teachers.count();
        ui->tableWidget->setItem(i,3,new QTableWidgetItem(QString::number(p)));
        ui->tableWidget->setItem(i,4,new QTableWidgetItem(QString::number(r)));
        ui->tableWidget->setItem(i,5,new QTableWidgetItem(QString::number(t)));
        int oneDayCost = p*55+r*17+t*15;
        ui->tableWidget->setItem(i,6,new QTableWidgetItem(QString::number(oneDayCost)));
        summ+=oneDayCost;
        i++;
    }
    qDebug() << "Dates" << dates;
    ui->label_4->setText(QString::number(dates.count()));
    ui->label_6->setText(QString::number(ui->tableWidget->rowCount()-1));
    ui->label->setText(QString::number(summ));

}

QStringList MainWindow::getFiles(QString path, QRegExp re)
{
    QStringList out;
    QDir dir(path);
    QStringList subDirs = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    QStringList files= dir.entryList(QDir::Files | QDir::NoDotAndDotDot);
    if (subDirs.count() > 0){
        foreach (QString subDir, subDirs) {
            if (subDir != "корзина") {
                QString subPath = path+"/"+subDir;
                files += getFiles(subPath,re);
            }
        }
    }

    foreach (QString file, files) {
        QFileInfo fileInfo(file);
//        qDebug() << fileInfo.canonicalFilePath();
        if (re.exactMatch(file) /*&& created.date().month() == 2*/) out+=file;
    }

    return out;
}

int MainWindow::getPageCount(QString path)
{
    int count = 0;
    QRegExp razvorot("(разворот)");
    QDir dir(path);
    QStringList folders = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    foreach (QString folder, folders) {
        if (folder.contains("разворот")) count++;
    }
    return count>0 ? count-1 : count;
}
