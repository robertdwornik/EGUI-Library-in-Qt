#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "adddialog.h"
#include <QPalette>
#include <QStandardItemModel>
#include <QTextStream>
#include <QFileDialog>
#include <QFile>
#include <QDir>
#include <QString>
#include <QDebug>
#include <QComboBox>
#include <adddialog.h>
#include <QMessageBox>

#include <QMessageBox>
#include <QModelIndex>
#include <QFileDialog>
#include <QtDebug>
#include <sstream>
#include <adddialog.h>
#include <vector>
#include <algorithm>
#include <QtAlgorithms>
#include <QVector>
#include <iterator>


#include "tablemodel.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
       ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setMenuBarGrey();
    setUpModels();
    openFile();


    connect(ui->editBookButton, &QAbstractButton::clicked,this, &MainWindow::editEntry);
    connect(ui->deleteBookButton, &QAbstractButton::clicked,this, &MainWindow::removeEntry);
    connect(ui->addBookButton, &QAbstractButton::clicked,this, &MainWindow::addEntrySlot);
    connect(ui->filterButton, &QAbstractButton::clicked,this, &MainWindow::filterEntry);
    connect(tableView->selectionModel(),
            &QItemSelectionModel::selectionChanged,
            this,
            &MainWindow::selectionChanged);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateActions(const QItemSelection &selection)
{
    QModelIndexList indexes = selection.indexes();
}

void MainWindow::openFile()
{
    QString filename = QFileDialog::getOpenFileName (nullptr, "Open dat file",
                                                         QDir::currentPath(), "dat (*.csv)");
    QFile file(filename);

    if(!file.open(QIODevice::ReadOnly)){
        qDebug() << file.errorString();
        return;
    }

    QList<Book> books;
    QStringList bookData;

    while (!file.atEnd()) {
      QByteArray line = file.readLine();

      for(QString it : line.split(';')){
          bookData.push_back(it);
          if(bookData.size() == 3){
              Book bookdata;
              bookdata.author = bookData[0];
              bookdata.title = bookData[1];
              bookdata.year = bookData[2];

              books.push_back(bookdata);
              bookData.clear();
          }
      }
    }
    if (books.isEmpty()) {
            QMessageBox::information(this, tr("No contacts in file"),
                                     tr("The file you are attempting to open contains no contacts."));
        } else {
            for (const auto &book: qAsConst(books))
                addEntry(book.author, book.title,book.year);
        }
}


void MainWindow::editEntry()
{
    QItemSelectionModel *selectionModel = tableView->selectionModel();

    QModelIndexList indexes = selectionModel->selectedRows();
    QString author, title, year;
    int row = -1;
    foreach (QModelIndex index, indexes) {
        row = proxyModel->mapToSource(index).row();

        QModelIndex authorIndex = table->index(row, 0, QModelIndex());
        QVariant varAuthor = table->data(authorIndex, Qt::DisplayRole);
        author = varAuthor.toString();

        QModelIndex titleIndex = table->index(row, 1, QModelIndex());
        QVariant varTitle = table->data(titleIndex, Qt::DisplayRole);
        title = varTitle.toString();

        QModelIndex yearIndex = table->index(row, 2, QModelIndex());
        QVariant varYear = table->data(yearIndex, Qt::DisplayRole);
        year = varYear.toString();

    }

        AddDialog aDialog;
        aDialog.setWindowTitle(tr("Edit a book"));

        aDialog.authorText->setText(author);
        aDialog.titleText->setText(title);
        aDialog.yearText->setText(year);

        if (aDialog.exec()) {
            QString newAuthor = aDialog.authorText->toPlainText();
            QString newTitle = aDialog.titleText->toPlainText();
            QString newYear = aDialog.yearText->toPlainText();

            if (newAuthor != author) {
                QModelIndex index = table->index(row, 0, QModelIndex());
                table->setData(index, newAuthor, Qt::EditRole);
            }
            if (newTitle != title) {
                QModelIndex index = table->index(row, 1, QModelIndex());
                table->setData(index, newTitle, Qt::EditRole);
            }
            if (newYear != year) {
                QModelIndex index = table->index(row, 2, QModelIndex());
                table->setData(index, newYear, Qt::EditRole);
            }
        }
}

void MainWindow::removeEntry()
{
    QItemSelectionModel *selectionModel = tableView->selectionModel();
    //QSortFilterProxyModel *proxy = static_cast<QSortFilterProxyModel*>(tableView->model());
    QModelIndexList indexes = selectionModel->selectedRows();

    //int size = indexes.size();
    std::vector<int> reversedIndexes;

    foreach (QModelIndex index, indexes) {
       int row = proxyModel->mapToSource(index).row();
       reversedIndexes.push_back(row);
       //table->removeRows(row,1,QModelIndex());
    }

    std::sort(reversedIndexes.begin(),reversedIndexes.end(),std::greater<int>());

    for(const auto& v : reversedIndexes)
        table->removeRows(v,1,QModelIndex());
}

void MainWindow::addEntrySlot()
{
    AddDialog aDialog;

    if(aDialog.exec()){
        QString author = aDialog.authorText->toPlainText();
        QString title = aDialog.titleText->toPlainText();
        QString year = aDialog.yearText->toPlainText();

        addEntry(author,title,year);
    }
}

void MainWindow::filterEntry()
{
    QString author = ui->authorLineEdit->text();
    QString title = ui->titleLineEdit->text();
    QString year = ui->comboBox->currentText();
}

void MainWindow::setMenuBarGrey()
{
    QPalette palette;
    palette.setColor(ui->menuLibrary->backgroundRole(),Qt::gray);
    ui->menuBar->setPalette(palette);
}

void MainWindow::setUpModels()
{

    table = new TableModel(this);
    tableView = ui->tableBookView;

    proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(table);
    proxyModel->setFilterKeyColumn(2);

    tableView->setModel(proxyModel);

    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableView->setSelectionMode(QAbstractItemView::MultiSelection);
    tableView->setSortingEnabled(true);

}

void MainWindow::addEntry(QString author, QString title, QString year)
{
      if(!table->getBooks().contains({author,title,year})){
        table->insertRows(0,1,QModelIndex());

        QModelIndex index = table->index(0,0,QModelIndex());
        table->setData(index,author, Qt::EditRole);
        index = table->index(0,1,QModelIndex());
        table->setData(index,title,Qt::EditRole);
        index = table->index(0,2, QModelIndex());
        table->setData(index,year, Qt::EditRole);

      }else {
      QMessageBox::information(this, tr("Duplicate title and author"),
                               tr("The author  \"%1\" and title \"%2\" already exists.")
                               .arg(author,title));
  }
}


