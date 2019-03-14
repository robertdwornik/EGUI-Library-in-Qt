#include "booklistwidget.h"
#include <QMessageBox>
#include <QModelIndex>
#include <QFileDialog>
#include <QtDebug>
#include <sstream>
#include <adddialog.h>
BookListWidget::BookListWidget(QWidget *parent,QTableView* View)
{
    table = new TableModel(this);

    proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(table);
    proxyModel->setFilterKeyColumn(2);

    tableView = View;
    tableView->setModel(proxyModel);

    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    tableView->setSortingEnabled(true);

    connect(tableView->selectionModel(),
            &QItemSelectionModel::selectionChanged,
            this,
            &BookListWidget::selectionChanged);

}

void BookListWidget::readFromFile()
{
    QString filename = QFileDialog::getOpenFileName (nullptr, "Open dat file",
                                                         QDir::currentPath(), "dat (*.dat)");
    QFile file(filename);

    if(!file.open(QIODevice::ReadOnly)){
        qDebug() << file.errorString();
        return;
    }

    QList<Book> books;
    QStringList bookData;

    while (!file.atEnd()) {
      QByteArray line = file.readLine();

      for(QString it : line.split(',')){
          bookData.push_back(it);
          if(bookData.size() == 3){
              Book bookdata(bookData);
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

void BookListWidget::showAddEntryDialog()
{
    AddDialog aDialog;

        if (aDialog.exec()) {
            QString author = aDialog.authorText->toPlainText();
            QString title = aDialog.titleText->toPlainText();
            QString year = aDialog.yearText->toPlainText();

            addEntry(author, title,year);
        }
}

void BookListWidget::addEntry(QString author, QString title, QString year)
{
  //  if(!table->getBooks().contains({author,title,year})){
        table->insertRows(0,1,QModelIndex());

        QModelIndex index = table->index(0,0,QModelIndex());
        table->setData(index,author, Qt::EditRole);
        index = table->index(0,1,QModelIndex());
        table->setData(index,title,Qt::EditRole);
        index = table->index(0,2, QModelIndex());
        table->setData(index,year, Qt::EditRole);

//    }else {
//    QMessageBox::information(this, tr("Duplicate title and author"),
//                             tr("The author  \"%1\" and title \"%2\" already exists.")
//                             .arg(author,title));
//}
}

void BookListWidget::editEntry()
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

void BookListWidget::removeEntry()
{
    QItemSelectionModel *selectionModel = tableView->selectionModel();
    QModelIndexList indexes = selectionModel->selectedRows();

    foreach (QModelIndex index, indexes) {
       int row = proxyModel->mapToSource(index).row();
       table->removeRows(row,1,QModelIndex());
    }
}