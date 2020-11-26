#ifndef CERTGEN_H
#define CERTGEN_H

#include <QMainWindow>
#include <QListWidgetItem>

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

    void on_actionLoadTemp_triggered();

    void on_loadTemp_clicked();

    void on_loadNames_clicked();

//    void name_clicked(QListWidgetItem* item);

    bool generateHQ();

    bool loadTemplate();

    void on_names_itemClicked(QListWidgetItem *item);

private:
    Ui::Certgen *ui;
    QString currentTemplate;
};



#endif // CERTGEN_H
