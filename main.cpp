#include "certgen.h"

#include <QApplication>

#include "xlsxdocument.h"
#include "xlsxchartsheet.h"
#include "xlsxcellrange.h"
#include "xlsxchart.h"
#include "xlsxrichstring.h"
#include "xlsxworkbook.h"
using namespace QXlsx;

int main(int argc, char *argv[])
{
    QApplication CertgenApp(argc, argv);
    Certgen Editor;
    Editor.show();
    return CertgenApp.exec();
}
