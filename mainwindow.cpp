#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qsrand(QDateTime::currentDateTimeUtc().toMSecsSinceEpoch());
    //init
    for (int i=0; i < 150; i++){
        w.addAnimal(generateAnimal());
    }
    //createAndSaveTestAnimals();
    //loadAnimals();
    tmr_run.setInterval(1);
    connect(&tmr_run, SIGNAL(timeout()), SLOT(onTmrRunTimeout()));
    round_count = 0;
    is_fast = true;
    is_saving_pics = false;
    ui->cbxFast->setChecked(is_fast);
    ui->cbxSavePics->setChecked(is_saving_pics);
    ui->cbxSaveOnQuit->setChecked(true);
    config.redraw = ui->sbRedraw->value();
    config.food = ui->sbFood->value();
    config.animal = ui->sbAnimal->value();
    config.low_animal_count = ui->sbLowAnimal->value();
    config.high_animal_count = ui->sbManyAnimal->value();
}

MainWindow::~MainWindow()
{
    w.saveBestAnimal();
    delete ui;
}

void MainWindow::onTmrRunTimeout()
{
    //qDebug("time!");
    round_count++;
    tmr_run.stop();
    if (round_count % config.food == 0)
        w.feedAnimal();
    if (round_count % config.animal == 0)
        appendNewAnimals();
    w.makeStep();           // go to next round: move all animals
    qApp->sendPostedEvents();   // no window freezing!
    qApp->processEvents();
    if ( is_saving_pics || !is_fast || (round_count % config.redraw == 0) ) { // speed optimization
        ui->lblMap->setPixmap(QPixmap::fromImage(w.getImage())); //very slow operation
        ui->lblCount->setText(QString::number(w.getAnimalCount()));
        ui->lblFitness->setText(
                    QString("Best (%1) with fitness (%2)"   ).arg(
                            w.getBestAnimalID()             ).arg(
                            w.getBestAnimalFitness()        ));
        ui->lblCurAniID->setText(QString::number(w.getCurrentID()));
    }
    if (is_saving_pics){
        if (!QDir().exists(PICS_DIR))
            QDir().mkdir(PICS_DIR);
        ui->lblMap->pixmap()->save(QString(PICS_DIR)+QString("/round_%1.png").arg(round_count));
    }
    ui->lblTime->setText(QString::number(round_count));

    if (w.getAnimalCount() < config.low_animal_count) // if we have less then 50 animals then generate new!
        appendNewAnimals();

    if (w.getAnimalCount() > config.high_animal_count) // if we have too many animals then kill weak animals!
        w.killWeakAnimals();

    if (!timer_stop){
        tmr_run.start();
    }
}

void MainWindow::appendNewAnimals()
{
    for (int i=0; i < 150; i++){
        w.addAnimal(generateAnimal());
    }
}

void MainWindow::createAndSaveTestAnimals()
{
    QList<AnimalCommand> c,c2,c3;
    c << start
      << action_move_left
      << action_eat_down
      << action_eat_up
      << action_eat_left
      << action_eat_right
      << action_move_down
      << action_eat_down
      << action_eat_up
      << action_eat_left
      << action_eat_right
      << action_move_right
      << action_eat_down
      << action_eat_up
      << action_eat_left
      << action_eat_right
      << action_move_down
      << action_eat_down
      << action_eat_up
      << action_eat_left
      << action_eat_right
      << action_split
      << end;
    c2 << start
      << action_eat_left
      << action_move_left
      << action_eat_down
      << action_move_down
      << action_split
      << end;
    c3 << start // spiral
       << action_split
       << load_from_mem //load from var1
       << move_mem_right //save var2
       << data_dec
       << save_to_mem

       << move_mem_right //save to var3
       << data_dec
       << save_to_mem
           //right
               << action_move_right
               << data_dec
           << jump_to_ifnz
           << AnimalCommand(-3)
           //up
           << load_from_mem
           << data_dec
           << save_to_mem
               << action_move_up
               << data_dec
           << jump_to_ifnz
           << AnimalCommand(-3)
           //left
           << load_from_mem
           << data_dec
           << save_to_mem
               << action_move_left
               << data_dec
           << jump_to_ifnz
           << AnimalCommand(-3)
           //down
           << load_from_mem
           << data_dec
           << save_to_mem
               << action_move_down
               << data_dec
           << jump_to_ifnz
           << AnimalCommand(-3)
           //dec var2
           << move_mem_left
           << load_from_mem
           << data_dec
           << save_to_mem
           << jump_to_ifnz
           << AnimalCommand(-33)
           << move_mem_left // restore var1
//           << load_from_mem
       << end;
    QList<quint8> cmds = Animal::compile(c);
    QList<quint8> cmds2 = Animal::compile(c2);
    QList<quint8> cmds3 = Animal::compile(c3);
    QList<quint8> mems;
    Animal::saveAnimal("c1.ani",cmds,mems);
    Animal::saveAnimal("c2.ani",cmds2,mems);
    mems << 10 << 0 << 0;
    Animal::saveAnimal("c3.ani",cmds3,mems);
}

void MainWindow::loadAnimals()
{
    for (int i=0; i < 50; i++){
        w.addAnimal(Animal::loadAnimal("c1.ani"));
        w.addAnimal(Animal::loadAnimal("c2.ani"));
        w.addAnimal(Animal::loadAnimal("c3.ani"));
    }
}

void MainWindow::generateAndSaveAnimal()
{
    QList<AnimalCommand> cmd;
    quint8 i, maxCmd, maxMem;
    maxCmd = qrand()%1500 + 1;
    maxMem = qrand()%1500 + 1;
    QList<quint8> mems;
    for (i=0; i< maxCmd; i++){
        cmd << AnimalCommand(qrand() % MAX_ANIMAL_COMMAND);
        cmd << AnimalCommand(qrand() % (2 << sizeof(quint8)));
    }
    for (i=0; i< maxMem; i++)
        mems << char(qrand() % sizeof(quint8));
    QList<quint8> cmds = Animal::compile(cmd);
    Animal::saveAnimal("rnd.ani",cmds,mems);
}

Animal *MainWindow::generateAnimal()
{
    QList<AnimalCommand> cmd;
    quint8 i, maxCmd, maxMem;
    maxCmd = qrand()%1500 + 1;
    maxMem = qrand()%1500 + 1;
    QList<quint8> mems;
    for (i=0; i< maxCmd; i++){
        cmd << AnimalCommand(qrand() % MAX_ANIMAL_COMMAND);
        //cmd << AnimalCommand(qrand() % (2<<sizeof(quint8)));
    }
    for (i=0; i< maxMem; i++)
        mems << char(qrand() % sizeof(quint8));
    QList<quint8> cmds = Animal::compile(cmd);
    return (new Animal(cmds,mems));
}

void MainWindow::on_btnSaveBest_clicked()
{
    w.saveBestAnimal();
}

void MainWindow::on_cbxFast_toggled(bool checked)
{
    is_fast = checked;
}

void MainWindow::on_cbxSavePics_toggled(bool checked)
{
    is_saving_pics = checked;
}

void MainWindow::on_cbxSaveOnQuit_toggled(bool checked)
{
    w.setSaveBestOnQuit(checked);
}

void MainWindow::on_btnStart_toggled(bool checked)
{
    if (checked){
        tmr_run.start();
        timer_stop = false;
    }else{
        timer_stop = true;
#ifdef DEBUG
        qDebug("MainWindow: timer is stopped.");
#endif
    }
}

void MainWindow::on_sbRedraw_valueChanged(int arg1)
{
    config.redraw = arg1;
}

void MainWindow::on_sbAnimal_valueChanged(int arg1)
{
    config.animal = arg1;
}

void MainWindow::on_sbFood_valueChanged(int arg1)
{
    config.food = arg1;
}

void MainWindow::on_btnSavePic_clicked()
{
    QPixmap px = ui->lblMap->pixmap()->copy();
    //TODO: open save-dialog box
    px.save("pic.png");
}
