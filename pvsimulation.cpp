#include "pvsimulation.h"
#include "solardata.h"


PVSimulation::PVSimulation(QWidget * parent)
{
       SolarData * mySolar = new SolarData(parent);
       mySolarData = mySolar;
}

void PVSimulation::Start_simuation()
{
    double C_Battery_Aktuell = 0;
    double I_Load = 0;
    double U_Battery = 0;
    double SOC = 0;
    double I_PV = 0;
    double detal_t;
    double C_Battery_nom = 7.0;
    double C_Battery_last = C_Battery_nom*0.0;

    C_Battery_Ah.clear();
    SOC_Vector.clear();
    U_Bat_Vector.clear();
    I_Load_Vector.clear();

    if(SolarData_existing == false)
    {
            mySolarData->ImportSolarData(Solar_Data_File_path);
    }

    Time_Vector = mySolarData->SunDataTimeLine;

    detal_t = Time_Vector[1]-Time_Vector[0];



    for(int i=0; i < Time_Vector.size();  i++ )
    {
        SOC = C_Battery_last/C_Battery_nom;
        I_Load = PVSimulation::Load();
        I_PV = PVSimulation::PVString(mySolarData->SunDataIntensity[i],12);
        U_Battery =PVSimulation::Battery(I_Load,PVSimulation::ChargeController(I_Load, U_Bat_Vector[i-1], I_PV),SOC,detal_t);
      //  U_Battery = PVSimulation::Battery(0,PVSimulation::ChargeController(I_Load, U_Battery, I_PV),SOC,detal_t);


        C_Battery_Aktuell = C_Battery_last+(PVSimulation::ChargeController(I_Load, U_Battery, I_PV))*detal_t;
//nur für test
if (C_Battery_Aktuell>C_Battery_nom)
{C_Battery_Aktuell = C_Battery_nom;}



        C_Battery_Ah.append(C_Battery_Aktuell);
        SOC_Vector.append(C_Battery_Aktuell/C_Battery_nom*100);
        U_Bat_Vector.append(U_Battery);

         if(U_Battery>10.4){


         }
        double I_load_temp ;
        if(U_Battery>10.4){I_load_temp =I_Load; }
        else   {I_load_temp = 0;}
        I_Load_Vector.append(I_load_temp);

        C_Battery_last =  C_Battery_Aktuell;

    }





}


double PVSimulation::ChargeController(double I_Load, double U_Battery, double I_PV)
{
    double I_Charge;

    if((U_Battery < 13.4)& (U_Battery > 10.4))
    {
        if (/*Hysterises_high & */U_Battery > 13.2)
        {
            I_Charge = -I_Load;
        }
        else
        {
            Hysterises_high = false;
            I_Charge = I_PV - I_Load;
        }

    }
    if((U_Battery >= 13.4))
    {
        Hysterises_high =true;
        I_Charge =  - I_Load;
    }
    if((U_Battery <= 10.4))
    {
        I_Charge =  I_PV;
    }

    return I_Charge;
}

double PVSimulation::Battery(double I_Load, double I_PV,double SOC, double dt)
{
    double U_Battery = 12;
// Lead-Acid 12V 7Ah

    double E0 = 12.4659;    //[V]
    double R = 0.025;        //[Ohm]
    double K = 0.047;       //[V/Ah]
    double A = 0.83;        //[V]
    double B = 125;        //[Ah-1]
    double Q = 7.0;        //[Ah]

    double I =(I_Load)-(I_PV);
    double It = (1-SOC)*Q;//*0.875;//(1-SOC*0.875)*Q*0.875;



    //Discharge
    if(I >= 0 ){


        U_EXP = U_EXP + B*I*(-(U_EXP)+A*0)*dt;

        U_Battery = E0 - R * I -K*Q/(Q-It)*(It+I)-U_EXP;
     //   U_Battery=12-U_EXP;
    }
    //Charge
    if(I< 0){
        U_EXP = U_EXP + B*qAbs(I)*(-(U_EXP)-A)*dt;

        U_Battery = E0 - R * I -K*Q/(It-(double)0.1*Q)*(I)-K*Q/(Q-It)*(It)-U_EXP;
    //   U_Battery=12-U_EXP;
       // U_Battery = E0 - R * I -K*Q/(Q-It)*(It+I)-U_EXP;

        U_Battery = E0 - R * I -K*Q/(Q-It)*(It+I)-U_EXP;
        //if(U_Battery >13.7){U_Battery = 15;}
    }

    return U_Battery;
}


double PVSimulation::PVString(double Intensity, double U_Battery)
{

    return 0.4;//Intensity/1000*(double)0.7;
}

double PVSimulation::Load()
{
    double I_load =0.011;
    return I_load;
}



QVector <double> PVSimulation::get_BatteryCharacteristic_time()
{
    QVector <double> time;
    time.clear();
   for(int i = 1; i<501; i++)
   {
       time.append((double)20/(double)101*(double)i);
   }
    return time;
}

QVector <double> PVSimulation::get_BatteryCharacteristic(double I, double C )
{
    QVector <double> U;
    QVector <double> time = PVSimulation::get_BatteryCharacteristic_time();
    double dt = time[0]-time[1];
    for(int i = 0; i<time.size()-1; i++)
    {

        double SOC_temp = 1 - time[i]*I/C;
        if(SOC_temp >= 0){
        U.append(PVSimulation::Battery(I,0,SOC_temp,dt));
        }
    }

    return U;
}


void PVSimulation::Import_solar_Data(  QString fileName)
{
    mySolarData->ImportSolarData(Solar_Data_File_path);
    SolarData_existing = true;
}

//getter and setter fundtios

QVector <double> PVSimulation::SOC(){   return SOC_Vector;}
QVector <double> PVSimulation::get_U_Bat(){    return U_Bat_Vector;}
QVector <double> PVSimulation::get_I_Load(){    return I_Load_Vector;}

QVector <double> PVSimulation::Time(){    return mySolarData->SunDataTimeLine;}
QVector <double> PVSimulation::SunData(){    return mySolarData->SunDataIntensity;}

void PVSimulation::create_Solar_File_list(){    mySolarData->CreateFileList();}
QList<QString> PVSimulation::Output_Country_List(){   return mySolarData->Output_Country_List();}
QList<QString> PVSimulation::Output_City_List(QString Country){     return mySolarData->Output_City_List(Country);}

QString PVSimulation::Output_Filepath(int Index_of_City ){    Solar_Data_File_path = mySolarData->Output_Filepath(Index_of_City);   return Solar_Data_File_path;}
double PVSimulation::Output_longitude(){    return mySolarData->Output_longitude();}
double PVSimulation::Output_latitude(){    return mySolarData->Output_latitude();}
