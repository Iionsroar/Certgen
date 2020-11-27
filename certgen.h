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
    void on_actionLoadTemp_triggered();

    void on_loadTemp_clicked();

    void on_loadNames_clicked();

    bool loadTemplate();

    void on_names_itemClicked(QListWidgetItem *item);

    void on_vSlider_valueChanged(int value);

    void on_hSlider_valueChanged(int value);

    void on_chkBold_stateChanged(int arg1);

    void on_chkItalic_stateChanged(int arg1);

    void on_chkUnderline_stateChanged(int arg1);

    void on_fontSize_activated(const QString &arg1);

    void on_fontFamily_activated(const QString &arg1);

    void on_generate_clicked();

    QImage generateHQ();

private:
    Ui::Certgen *ui;
    QString currentTemplate;
};

#endif // CERTGEN_H
