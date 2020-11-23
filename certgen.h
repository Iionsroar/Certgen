#ifndef CERTGEN_H
#define CERTGEN_H

#include <QMainWindow>
QT_BEGIN_NAMESPACE
namespace Ui { class Certgen; }
QT_END_NAMESPACE



class Certgen : public QMainWindow
{
    Q_OBJECT

public:
    Certgen(QWidget *parent = nullptr);
    ~Certgen();

private slots:
//    void loadTemplate();

//    void loadSpreadsheet();

//    void pasteNames();

//    void generate();

//    void save();

//    void print();

//    void exit();

//    void selectFont();

//    void setFontBold(bool bold);

//    void setFontUnderline(bool underline);

//    void setFontItalic(bool italic);

//    void about();

//    void on_actionOpenTemp_triggered();

    void on_actionLoadTemp_triggered();

private:
    Ui::Certgen *ui;
    QString currentTemplate;
};



#endif // CERTGEN_H
