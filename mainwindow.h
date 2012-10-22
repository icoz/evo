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
    void onTimerTimeout();
    void on_btnStart_clicked();

    void on_btnStop_clicked();

private:
    Ui::MainWindow *ui;
    World w;
    QTimer tmr;
    QTimer tmr_food;
    bool timer_stop;
    void createAndSaveTestAnimals();
    void loadAnimals();
    void generateAndSaveAnimal();
    Animal* generateAnimal();
};

#endif // MAINWINDOW_H
