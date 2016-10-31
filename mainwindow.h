#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"
#include "pvsimulation.h"

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


    void makePlot();


    void on_Button_Load_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_3_clicked();

    void on_Button_LoadSun_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_11_clicked();

    void on_comboBox_country_currentIndexChanged(const QString &arg1);



    void on_actionUpdate_input_files_triggered();

    void on_pushButton_10_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_13_clicked();

private:
        PVSimulation *mySolarSim;

        QVector<QComboBox*> ModuleType_Vector;
        QHBoxLayout *controlsLayout;
        QVector<QSpinBox*> ModuleNumber_Vector;

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
