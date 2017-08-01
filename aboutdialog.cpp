#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    ui->appNameLabel->setText(qApp->applicationDisplayName());
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
