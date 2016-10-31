#ifndef SOLARDATA_H
#define SOLARDATA_H


#include <QMainWindow>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QDebug>
#include <QString>
#include <QStandardItemModel>
#include <QVector>


class SolarData
{
public:

    QVector <double> SunDataYear;
    QVector <double> SunDataMonth;
    QVector <double> SunDataDay;
    QVector <double> SunDataHour;
    QVector <double> SunDataIrradiation;
    QVector <double> SunDataIntensity;
    QVector <double> SunDataTimeLine;

    SolarData(QWidget *parent);

    void ImportSolarData(QString fileName);
    void CreateFileList();
    QList<QString> Output_Country_List();
    QList<QString> Output_City_List(QString Country);
    QString Output_Filepath(int Index_of_City);
    double Output_longitude();
    double Output_latitude();
private:
    QList<QStringList> csv;
    QStandardItemModel *model;
    QList<QStandardItem*> standardItemList;
    QVector<QVector<QString> > String_2d_Vector;
    QVector <QString> temp_vector;
    QWidget * myparent;

    QFileInfoList list_of_Solar_files;
    QList<QString>  ListOfCountries;
    QList<QString>  ListOfCities;
    int Index_First_of_this_country = -1;
    QString relative_Folder_path;
    double longitude;
    double latitude;








    void Csv2Table();
    void checkString(QString &temp, QChar character = 0);




    void Plot_Solar_Data(QWidget *parent);
    void CSVFunction(QString fileName);
};

#endif // SOLARDATA_H
