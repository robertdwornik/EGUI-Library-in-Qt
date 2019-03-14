#ifndef ADDDIALOG_H
#define ADDDIALOG_H

#include<QDialog>
#include<QLabel>
#include<QLineEdit>
#include<QTextEdit>


class AddDialog : public QDialog
{
    Q_OBJECT

public:
    AddDialog(QWidget *parent = 0);
    QTextEdit *authorText;
    QTextEdit *titleText;
    QTextEdit *yearText;

private:
    QLabel *authorLabel;
    QLabel *titleLabel;
    QLabel *yearLabel;

    QPushButton *okButton;
    QPushButton *cancelButton;
};
#endif // ADDDIALOG_H