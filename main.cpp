#include "certgen.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication CertgenApp(argc, argv);
    Certgen Editor;
    Editor.show();
    return CertgenApp.exec();
}
