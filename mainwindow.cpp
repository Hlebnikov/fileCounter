#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "xlsxdocument.h"

const QRegExp teacher("[^A-Z0-9]*[,][^A-Z0-9]*(jpg|jpeg)",Qt::CaseInsensitive);
const QRegExp portrait("([^A-Z0-9,]*[()]*)(jpg|jpeg)",Qt::CaseInsensitive);
const QRegExp report("[0-9.]*(jpg|jpeg)",Qt::CaseInsensitive);

const int portraitCost = 55;
const int reportCost = 17;
const int teacherCost = 15;

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

//    MonthDialog *monthDialog  = new MonthDialog();
//    monthDialog->show();
//    path = "D:/Рабочая/Февраль";
    ui->tableWidget->clear();
    ui->tableWidget->setColumnCount(7);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "Дата" << "Папка" << "Развороты" << "Портреты" << "Репортаж" << "Учителя" <<"Стоимость");
//    QFileInfo mainDirecroryInfo(path);
//    mainMonth = mainDirecroryInfo.created().date().month();
//    qDebug() << mainMonth;
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
    QStringList subfolders = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot); //, QDir::Time
    ui->tableWidget->setRowCount(dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot).count()+1);
    int i=subfolders.count()-1;
    int summ=0;

    qDebug() << countMatchedFiles(path,portrait);

    QSet<QDate> dates;

    foreach (QString subfolder, subfolders) {
        QString subdir(path+"/"+subfolder);
        int pages = getPageCount(subdir);
        QFileInfo fileInfo(subdir);
        QDate createDate = fileInfo.created().date();
        dates << createDate;
        ui->tableWidget->setItem(i,0, new QTableWidgetItem(createDate.toString(Qt::SystemLocaleShortDate)));
        ui->tableWidget->setItem(i,1,new QTableWidgetItem(subfolder));
        ui->tableWidget->setItem(i,2,new QTableWidgetItem(QString::number(pages)));

        int portraitCount = countMatchedFiles(path+"/"+subfolder,portrait);
        int reportCount = countMatchedFiles(path+"/"+subfolder,report);
        int teacherCount = countMatchedFiles(path+"/"+subfolder,teacher);
        ui->tableWidget->setItem(i,3,new QTableWidgetItem(QString::number(portraitCount)));
        ui->tableWidget->setItem(i,4,new QTableWidgetItem(QString::number(reportCount)));
        ui->tableWidget->setItem(i,5,new QTableWidgetItem(QString::number(teacherCount)));
        int oneDayCost = portraitCount*portraitCost+reportCount*reportCost+teacherCount*teacherCost;
        ui->tableWidget->setItem(i,6,new QTableWidgetItem(QString::number(oneDayCost)));
        summ+=oneDayCost;
        i--;
    }
//    qDebug() << "Dates" << dates;
    ui->label_4->setText(QString::number(dates.count()));
    ui->label_6->setText(QString::number(ui->tableWidget->rowCount()-1));
    ui->label->setText(QString::number(summ));
}

int MainWindow::countMatchedFiles(QString path, QRegExp re)
{
    int out = 0;
    QDir dir(path);
    QStringList subDirs = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    QFileInfoList infos = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);

    if (subDirs.count() > 0){
        foreach (QString subDir, subDirs) {
            if (subDir != "корзина") {
                QString subPath = path+"/"+subDir;
                out += countMatchedFiles(subPath,re);
            }
        }
    }

    foreach (QFileInfo info, infos) {
        if (re.exactMatch(info.fileName()) && info.lastModified().date().month() == 4) {
            QExifImageHeader header = QExifImageHeader(info.absoluteFilePath());
            QString copyright = header.value(QExifImageHeader::Copyright).toString();
            QDateTime date = header.value(QExifImageHeader::DateTimeDigitized).toDateTime();
            if ((copyright == "hlebnikov-a.ru") && (date.date().month()==4)) out++;
        }
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
    return count > 0 ? count-1 : count;
}
