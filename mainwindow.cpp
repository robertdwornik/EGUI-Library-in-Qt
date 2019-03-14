#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPalette>
#include <QStandardItemModel>
#include <QTextStream>
#include <QFileDialog>
#include <QFile>
#include <QDir>
#include <QString>
#include <QDebug>
#include <QComboBox>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
       ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setMenuBarGrey();

    QTableView *table1 = ui->tableBookView;
    bookListWidget = new BookListWidget((table1));
    setCentralWidget(bookListWidget);
    createMenus();
    setWindowTitle(tr("Book List"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateActions(const QItemSelection &selection)
{
    QModelIndexList indexes = selection.indexes();

       if (!indexes.isEmpty()) {
           removeAct->setEnabled(true);
           editAct->setEnabled(true);
       } else {
           removeAct->setEnabled(false);
           editAct->setEnabled(false);
       }
}

void MainWindow::openFile()
{
    bookListWidget->readFromFile();
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));

       openAct = new QAction(tr("&Open..."), this);
       fileMenu->addAction(openAct);
       connect(openAct, &QAction::triggered, this, &MainWindow::openFile);


       editAct = new QAction(tr("&Edit Entry..."), this);
       editAct->setEnabled(false);
       //toolMenu->addAction(editAct);
       connect(editAct, &QAction::triggered, bookListWidget, &BookListWidget::editEntry);

       //toolMenu->addSeparator();

       removeAct = new QAction(tr("&Remove Entry"), this);
       removeAct->setEnabled(false);
       //toolMenu->addAction(removeAct);
       connect(removeAct, &QAction::triggered, bookListWidget, &BookListWidget::removeEntry);

       connect(bookListWidget, &BookListWidget::selectionChanged,
           this, &MainWindow::updateActions);
}



void MainWindow::setMenuBarGrey()
{
    QPalette palette;
    palette.setColor(ui->menuLibrary->backgroundRole(),Qt::gray);
    ui->menuBar->setPalette(palette);
}



//void MainWindow::on_addBookButton_clicked()
//{
//    int row = model->rowCount();
//    model->insertRows(row,1);

//    QModelIndex index = model->index(row);

//    ui->bookListView->setCurrentIndex(index);
//    ui->bookListView->edit(index);
//}

//void MainWindow::on_editBookButton_clicked()
//{

//}

//void MainWindow::on_deleteBookButton_clicked()
//{
//    model->removeRows(ui->bookListView->currentIndex().row(),1);

//}
