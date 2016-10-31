#ifndef PVSIMULATION_H
#define PVSIMULATION_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include "solardata.h"
#include <QDebug>

class PVSimulation
{
public:
    PVSimulation(QWidget * parent);
    QVector<double> Time();
    QVector<double> SOC();



    void Import_solar_Data( QString fileName);
    void Start_simuation();
    QVector<double> SunData();
    void create_Solar_File_list();
    QList<QString> Output_Country_List();
    QList<QString> Output_City_List(QString Country);
    QString Output_Filepath(int Index_of_City);
    double Output_latitude();
    double Output_longitude();
    QVector<double> get_U_Bat();
    QVector<double> get_I_Load();
    QVector<double> get_BatteryCharacteristic(double I, double C);
    QVector<double> get_BatteryCharacteristic_time();
private:
    QString Solar_Data_File_path = "";
    SolarData *mySolarData;

    QVector <double> C_Battery_Ah;
    QVector <double> SOC_Vector;
    QVector <double> Time_Vector;
    QVector <double> U_Bat_Vector;
    QVector <double> I_Load_Vector;

    bool Hysterises_high = false;


    double U_EXP =0  ;
    double     last_EXP =0;




    bool SolarData_existing = false ;
    double Load();
    double Battery(double I_Load, double I_PV, double SOC, double dt);
    double ChargeController(double delta_t, double U_Battery, double I_PV);
    double PVString(double Intensity, double U_Battery);


};

#endif // PVSIMULATION_H
