#include "adddialog.h"
#include <QPushButton>
#include <QGridLayout>
#include <QLineEdit>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QDebug>
AddDialog::AddDialog(QWidget *parent)
    : QDialog (parent)
{

        okButton = new QPushButton("OK");
        cancelButton = new QPushButton("Cancel");

        authorText = new QLineEdit;
        titleText = new QLineEdit;
        yearText = new QLineEdit;
        errorMessage = new QErrorMessage(this);
        errorMessage->topLevelWidget();


        formGroupBox = new QGroupBox(tr("Data"));
        QFormLayout *layout = new QFormLayout;
        layout->addRow(new QLabel(tr("Author")), authorText);
        layout->addRow(new QLabel(tr("Title")), titleText);
        layout->addRow(new QLabel(tr("Year")), yearText);
        formGroupBox->setLayout(layout);


        QVBoxLayout *mainLayout = new QVBoxLayout;


        buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                         | QDialogButtonBox::Cancel);


        connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
        connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));


        mainLayout->addWidget(formGroupBox);
        mainLayout->addWidget(buttonBox);

        setLayout(mainLayout);
        setWindowTitle(tr("Add a book"));

}

void AddDialog::done(int r)
{
    if(r == QDialog::Accepted){   
        if(!isNumber() && isEmpty()){
            errorMessage->showMessage("No such a year and title and author label can't be empty");
            return;
        }else if (isEmpty()) {
            errorMessage->showMessage("title and author label can't be empty");
            return;
        }else if (checkSemi()) {
            errorMessage->showMessage("can't use semicolon either in title or author");
            return;
        }
        else if (!isNumber()) {
            errorMessage->showMessage("No such a year");
            return;
        }
    }

    QDialog::done(r);
}

bool AddDialog::isNumber()
{
        return yearText->text().toInt() && yearText->text().size() && std::find_if(yearText->text().begin(),yearText->text().end(),[](QChar c){
            return c.isDigit();
        });
}

bool AddDialog::isEmpty()
{

    return (authorText->text().isEmpty() && titleText->text().isEmpty());
}

bool AddDialog::checkSemi()
{
    return std::find_if(authorText->text().begin(),authorText->text().end(),[](QChar c){
        return (c == ";");
    }) && std::find_if(authorText->text().begin(),authorText->text().end(),[](QChar c){
    return (c == ";");
    });
}

