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
    void on_btnSaveBest_clicked();
    void on_cbxFast_toggled(bool checked);
    void on_cbxSavePics_toggled(bool checked);
    void on_cbxSaveOnQuit_toggled(bool checked);
    void on_btnStart_toggled(bool checked);

    void on_sbRedraw_valueChanged(int arg1);

    void on_sbAnimal_valueChanged(int arg1);

    void on_sbFood_valueChanged(int arg1);

    void on_btnSavePic_clicked();

private:
    Ui::MainWindow *ui;
    World w;
    QTimer tmr_run;
    bool timer_stop;
    bool is_saving_pics;
    bool is_fast;
    quint32 round_count;
    struct {
        quint32 food;
        quint32 animal;
        quint32 redraw;
        quint32 low_animal_count;
        quint32 high_animal_count;
    } config;
    void createAndSaveTestAnimals();
    void loadAnimals();
    void generateAndSaveAnimal();
    Animal* generateAnimal();
    void appendNewAnimals();
};

#endif // MAINWINDOW_H
