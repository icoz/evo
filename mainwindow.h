#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "world.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void onTmrRunTimeout();
    void onTmrNewAniTimeout();
    void on_btnStart_clicked();

    void on_btnStop_clicked();

    void on_btnSaveBest_clicked();

    void on_btnSavePics_clicked();

private:
    Ui::MainWindow *ui;
    World w;
    QTimer tmr_run;
    QTimer tmr_food;
    QTimer tmr_new_anis;
    bool timer_stop;
    bool is_saving_pics;
    quint32 round_count;
    void createAndSaveTestAnimals();
    void loadAnimals();
    void generateAndSaveAnimal();
    Animal* generateAnimal();
};

#endif // MAINWINDOW_H
