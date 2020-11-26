#include "certgen.h"
#include "ui_certgen.h"
#include "globals.h"

#include <QDir>
#include <QFileDialog>
#include <QGuiApplication>
#include <QImageReader>
#include <QImage>
#include <QMessageBox>
#include <QPainter>
#include <QPixmap>

#include "xlsxdocument.h"
#include "xlsxchartsheet.h"
#include "xlsxcellrange.h"
#include "xlsxchart.h"
#include "xlsxrichstring.h"
#include "xlsxworkbook.h"
using namespace QXlsx;

Certgen::Certgen(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Certgen)
{
    ui->setupUi(this);
//    connect(ui->names, SIGNAL(itemClicked(QListWidgetItem*)),
//                this, SLOT(name_clicked(QListWidgetItem*)));

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

bool Certgen::generateHQ()
{
    QImage templateImg(templateFile);
    QPainter p;
    QRect rectangle = QRect(xPos/100*templateWidth, yPos/100*templateHeight,
                            templateWidth, templateHeight);

    if (!p.begin(&templateImg)) {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Cannot load %1 for preview")
                                 .arg(QDir::toNativeSeparators(templateFile)));
        return false;
    }

    p.setPen(QPen(Qt::black));
    p.setFont(QFont(fontFamily, fontSize, QFont::Bold));
    p.drawText(rectangle, Qt::AlignCenter, previewName);
    ui->certificatePreview->setPixmap(QPixmap::fromImage(templateImg));

    return true;
}

bool Certgen::loadTemplate()
{
    QString templateFileName = QFileDialog::getOpenFileName(this, "Open a template");
    templateFile = templateFileName;

    QImageReader reader(templateFile);
    QSize sizeOfImg = reader.size();
    templateHeight = sizeOfImg.height();
    templateWidth = sizeOfImg.width();

    // MISSING if statement for handling errors and returning false

    QPixmap pm(templateFileName);
    ui->certificatePreview->setPixmap(pm);
    ui->certificatePreview->setScaledContents(true);
    return true;
}

void Certgen::on_actionLoadTemp_triggered() {loadTemplate();}
void Certgen::on_loadTemp_clicked() {loadTemplate();}

void Certgen::on_loadNames_clicked()
{
    QString namesFile = QFileDialog::getOpenFileName(this, "Open a spreadsheet file containing names (.xlsx, .xls)");

    // reading excel files
    Document xlsxR(namesFile);

    int sheetIndexNumber = 0;
    foreach( QString currentSheetName, xlsxR.sheetNames() )
    {
        // get current sheet
        AbstractSheet* currentSheet = xlsxR.sheet( currentSheetName );
        if ( NULL == currentSheet )
            continue;

        // get full cells of current sheet
        int maxRow = -1;
        int maxCol = -1;
        currentSheet->workbook()->setActiveSheet( sheetIndexNumber );
        Worksheet* wsheet = (Worksheet*) currentSheet->workbook()->activeSheet();
        if ( NULL == wsheet )
            continue;

        QString strSheetName = wsheet->sheetName(); // sheet name

        QVector<CellLocation> clList = wsheet->getFullCells( &maxRow, &maxCol );

        QVector< QVector<QString> > cellValues;
        for (int rc = 0; rc < maxRow; rc++)
        {
            QVector<QString> tempValue;
            for (int cc = 0; cc < maxCol; cc++)
            {
                tempValue.push_back(QString(""));
            }
            cellValues.push_back(tempValue);
        }

        for ( int ic = 0; ic < clList.size(); ++ic )
        {
            CellLocation cl = clList.at(ic); // cell location
            int row = cl.row - 1;
            int col = cl.col - 1;

            QSharedPointer<Cell> ptrCell = cl.cell; // cell pointer

            // value of cell
            QVariant var = cl.cell.data()->value();
            QString str = var.toString();

            cellValues[row][col] = str;
        }

        for (int rc = 0; rc < maxRow; rc++)
        {
            for (int cc = 0; cc < maxCol; cc++)
            {
                QString strCell = cellValues[rc][cc];
                QListWidgetItem * item = new QListWidgetItem();
                QCheckBox *chkbox = new QCheckBox(strCell);
                chkbox->setChecked(true);
                ui->names->addItem(item);
                ui->names->setItemWidget(item, chkbox);

                if (rc == 0 && cc == 0) { // set preview certificate using first name
                    previewName = strCell;
                    generateHQ();
                }
            }
        }
        sheetIndexNumber++;
    }

    // TODO search default enabled false
    ui->generate->setEnabled(true);

}

void Certgen::on_names_itemClicked(QListWidgetItem *item)
{
    QCheckBox *chkbox = qobject_cast<QCheckBox *> (ui->names->itemWidget(item));
    previewName = chkbox->text();
    generateHQ();
}
