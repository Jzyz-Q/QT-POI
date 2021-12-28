#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QProgressBar>
#include <QLabel>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

    Ui::Dialog *ui;
    void on_toolButton_clicked();

public slots:
    void slotHide();

private:

};

#endif // DIALOG_H
