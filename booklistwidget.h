#ifndef BOOKLISTWIDGET_H
#define BOOKLISTWIDGET_H


#include<QTableWidget>
#include<QSortFilterProxyModel>
#include"tablemodel.h"
class BookListWidget : public QTableWidget
{

    Q_OBJECT

public:
    BookListWidget(QWidget *parent = 0, QTableView* table = NULL);
    void readFromFile();

public slots:
    void showAddEntryDialog();
    void addEntry(QString author,QString title, QString year);
    void editEntry();
    void removeEntry();

signals:
    void selectionChanged(const QItemSelection &selected);
private:
    TableModel *table;
    QTableView *tableView;
    QSortFilterProxyModel *proxyModel;

};

#endif // BOOKLISTWIDGET_H
