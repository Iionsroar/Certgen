#include "certgen.h"
#include "ui_certgen.h"

#include <QDir>
#include <QFileDialog>
#include <QGuiApplication>
#include <QImageReader>
#include <QImage>
#include <QMessageBox>
#include <QPixmap>

Certgen::Certgen(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Certgen)
{
    ui->setupUi(this);
//    this->setCentralWidget(ui->certificatePreview);

//    connect(ui->actionOpenTemp, &QAction::triggered, this, &Certgen::openTemplate);

// Disable menu actions for unavailable features
//#if !defined(QT_PRINTSUPPORT_LIB) || !QT_CONFIG(printer)
//    ui->actionPrint->setEnabled(false);
//#endif

//#if !QT_CONFIG(clipboard)
//    ui->actionCut->setEnabled(false);
//    ui->actionCopy->setEnabled(false);
//    ui->actionPaste->setEnabled(false);
//#endif
}

Certgen::~Certgen()
{
    delete ui;
}

void Certgen::on_actionLoadTemp_triggered()
{
    QString templateFileName = QFileDialog::getOpenFileName(this, "Open a template");

    QPixmap pm(templateFileName);
    ui->certificatePreview->setPixmap(pm);
    ui->certificatePreview->setScaledContents(true);

    QImageReader reader(templateFileName);
    reader.setAutoTransform(true);
    const QImage newImage = reader.read();
    if (newImage.isNull()) {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Cannot load %1: %2")
                                 .arg(QDir::toNativeSeparators(templateFileName), reader.errorString()));
    }
}
