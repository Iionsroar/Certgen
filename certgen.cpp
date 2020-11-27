#include "certgen.h"
#include "ui_certgen.h"
#include "globals.h"

#include <QDesktopServices>
#include <QDir>
#include <QFileDialog>
#include <QGuiApplication>
#include <QImageReader>
#include <QImage>
#include <QMessageBox>
#include <QPainter>
#include <QPageSize>
#include <QPdfWriter>
#include <QPixmap>
#include <QSizeF>
#include <QUrl>

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

QImage Certgen::generateHQ()
{
    qreal convertedXPos = xPos/100*templateWidth;
    qreal convertedYPos = yPos/100*templateHeight;
    QRect rectangle = QRect(convertedXPos, convertedYPos,
                            templateWidth, templateHeight);

    QImage templateImg(templateFile);
    QPainter p;

    if (!p.begin(&templateImg)) {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Cannot load %1 for preview")
                                 .arg(QDir::toNativeSeparators(templateFile)));
        return QImage();
    }

    p.setPen(QPen(Qt::black));

    QFont font = QFont(fontFamily, fontSize);
    font.setBold(isBold);
    font.setItalic(isItalic);
    font.setUnderline(isUnderline);

    p.setFont(font);

    qreal width = p.boundingRect(rectangle, Qt::AlignCenter, previewName).width();
    qreal height = p.boundingRect(rectangle, Qt::AlignCenter, previewName).height();

    p.drawText(QRect(convertedXPos - width/2, convertedYPos - height/2, width, height), Qt::AlignCenter, previewName);
    ui->certificatePreview->setPixmap(QPixmap::fromImage(templateImg));

    return templateImg;
}

bool Certgen::loadTemplate()
{
    QString templateFileName = QFileDialog::getOpenFileName(this, "Open a template", ".", tr("All Supported Filetypes (*.jpg;*.jpeg;*.png)"));
    templateFile = templateFileName;

    QImageReader reader(templateFile);
    QSize sizeOfImg = reader.size();
    templateHeight = sizeOfImg.height();
    templateWidth = sizeOfImg.width();

    // MISSING if statement for handling errors and returning false

    generateHQ();
    return true;
}

void Certgen::on_actionLoadTemp_triggered() {loadTemplate();}
void Certgen::on_loadTemp_clicked() {loadTemplate();}

void Certgen::on_loadNames_clicked()
{
    QString namesFile = QFileDialog::getOpenFileName(this, "Open a file containing names", ".", tr("All Supported Filetypes (*.xlsx;*.xls)"));

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
                loadedNames.append(strCell);

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


void Certgen::on_hSlider_valueChanged(int value)
{
    xPos = value;
    generateHQ();
}

void Certgen::on_vSlider_valueChanged(int value)
{
    yPos = value;
    generateHQ();
}


void Certgen::on_chkBold_stateChanged(int arg1)
{
    isBold = arg1;
    generateHQ();
}

void Certgen::on_chkItalic_stateChanged(int arg1)
{
    isItalic = arg1;
    generateHQ();
}

void Certgen::on_chkUnderline_stateChanged(int arg1)
{
    isUnderline = arg1;
    generateHQ();
}

void Certgen::on_fontSize_activated(const QString &arg1)
{
    fontSize = arg1.toInt();
    generateHQ();
}

void Certgen::on_fontFamily_activated(const QString &arg1)
{
    fontFamily = arg1;
    generateHQ();
}

void Certgen::on_generate_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Save file"), ".", tr("Files (*.pdf)"));
    QPdfWriter writer(filename);

    writer.setPageOrientation(QPageLayout::Landscape);
    writer.setResolution(74);
    writer.setPageSize(QPageSize(QSizeF(templateHeight, templateWidth), QPageSize::Point));

    QPoint imageCoordinates(0, 0);
    QPainter pdfPainter(&writer);

    foreach(const QString& name, loadedNames)
    {
        previewName = name;
        pdfPainter.drawImage(imageCoordinates, generateHQ());
        writer.newPage();
    }

    pdfPainter.save();
    QDesktopServices::openUrl(QUrl(filename));
}
