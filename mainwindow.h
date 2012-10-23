#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "world.h"

namespace Ui {
class MainWindow;
}

#define PICS_DIR "pics"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void onTmrRunTimeout();
    void on_btnStart_clicked();
    void on_btnStop_clicked();
    void on_btnSaveBest_clicked();
    void on_btnSavePics_clicked();
    void on_cbxFast_toggled(bool checked);

private:
    Ui::MainWindow *ui;
    World w;
    QTimer tmr_run;
    bool timer_stop;
    bool is_saving_pics;
    bool is_fast;
    quint32 round_count;
    void createAndSaveTestAnimals();
    void loadAnimals();
    void generateAndSaveAnimal();
    Animal* generateAnimal();
    void appendNewAnimals();
};

#endif // MAINWINDOW_H
