#include "solardata.h"
#include <QDir>
#include <iostream>

SolarData::SolarData(QWidget * parent  )
{
  myparent  = parent;
}



void SolarData::CreateFileList( )
{
    relative_Folder_path ="Data_Files/Solar_Irradiation";
    QDir dir(relative_Folder_path);
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setSorting(QDir::Name );

    list_of_Solar_files = dir.entryInfoList();

    ListOfCountries.clear();

    for (int i = 0; i < list_of_Solar_files.size(); ++i) {
        QFileInfo fileInfo = list_of_Solar_files.at(i);

        QString tempFileName = fileInfo.fileName();
        QString Country = tempFileName.left(tempFileName.indexOf("_"));
        if(ListOfCountries.indexOf(Country) == -1){
            ListOfCountries.append(Country);

        }
    }
}

QList<QString> SolarData::Output_Country_List()
{
    return ListOfCountries;
}


QList<QString> SolarData::Output_City_List(QString Country)
{
    Index_First_of_this_country  = -1;

    ListOfCities.clear();

    for (int i = 0; i < list_of_Solar_files.size(); ++i) {
        QFileInfo fileInfo = list_of_Solar_files.at(i);
        QString tempFileName = fileInfo.fileName();

    if(tempFileName.contains(Country))
    {
        if(Index_First_of_this_country == -1)
        {
            Index_First_of_this_country = i;
        }

        QString temp2FileName =  tempFileName.replace(Country+"_", "");

        QString City = temp2FileName.left(temp2FileName.indexOf("_"));

            ListOfCities.append(City);


    }


}
    return ListOfCities;
}


QString SolarData::Output_Filepath(int Index_of_City )
{
    int total_index = Index_First_of_this_country+Index_of_City;
    QString Filepath = list_of_Solar_files.at(total_index).path();
    QString FileName = list_of_Solar_files.at(total_index).fileName();

    QString tempFileName = FileName;
    QString temp = tempFileName.left(tempFileName.indexOf("lat"));
    tempFileName = tempFileName.replace(temp+"lat","");


    QString Latitude_Str = tempFileName.left(tempFileName.indexOf("_"));
    latitude = Latitude_Str.toDouble(false);

    temp = tempFileName.left(tempFileName.indexOf("lon"));
    tempFileName = tempFileName.replace(temp+"lon","");

    QString Longitude_Str = tempFileName.left(tempFileName.indexOf("_"));
    longitude = Longitude_Str.toDouble(false);


    std::cout << qPrintable(QString(Filepath));
          std::cout << std::endl;
    std::cout << qPrintable(QString(FileName));
          std::cout << std::endl;


    std::cout << qPrintable(QString(Longitude_Str));
      std::cout << std::endl;
    return Filepath+"/"+FileName;

}


void SolarData::ImportSolarData( QString fileName)
{
    SolarData::CSVFunction(fileName);
    SolarData::Csv2Table();
}

double SolarData::Output_longitude()
{
    return longitude;
}

double SolarData::Output_latitude()
{
    return latitude;
}


void SolarData::CSVFunction( QString fileName_in)
{
QString fileName = fileName_in;
if(fileName == ""){
    fileName = QFileDialog::getOpenFileName (myparent, "Open CSV file",QDir::currentPath(), "CSV (*.csv)");
}
    QFile file (fileName);
    if (file.open(QIODevice::ReadOnly)) {
        String_2d_Vector.clear();
        QString data = file.readAll();
        data.remove( QRegExp("\r") ); //remove all ocurrences of CR (Carriage Return)
        QString temp;
        QChar character;
        QTextStream textStream(&data);
        while (!textStream.atEnd()) {
            textStream >> character;
            if (character == ';') {
                checkString(temp, character);
            } else if (character == '\n') {
                checkString(temp, character);
            } else if (textStream.atEnd()) {
                temp.append(character);
                checkString(temp);
            } else {
                temp.append(character);
            }
        }
    }


}

void SolarData::checkString(QString &temp, QChar character )
{


    if(temp.count("\"")%2 == 0) {
        //if (temp.size() == 0 && character != ';') //problem with line endings
        //    return;
        if (temp.startsWith( QChar('\"')) && temp.endsWith( QChar('\"') ) ) {
            temp.remove( QRegExp("^\"") );
            temp.remove( QRegExp("\"$") );
        }
        //FIXME: will possibly fail if there are 4 or more reapeating double quotes
        temp.replace("\"\"", "\"");

        temp_vector.push_back(temp);

        if (character != QChar(';')) {

            String_2d_Vector.push_back(temp_vector);

            temp_vector.clear();

        }

        temp.clear();
    } else {
        temp.append(character);
    }
}


void SolarData::Csv2Table()
{   double time_last = 0;
    bool ok = false;
    double timestep;
    double hour_aktuell = 0;
    double hour_next = 0;
    double minute = (float)1/(float)3600;


    int row = 0, col = 0;


    row =  (String_2d_Vector).size();
    if (row > 0){
        col =  (String_2d_Vector[0]).size();


    }

    SunDataYear.clear();
    SunDataMonth.clear();
    SunDataDay.clear();
    SunDataHour.clear();
    SunDataIrradiation.clear();
    SunDataIntensity.clear();
    SunDataTimeLine.clear();

    timestep = (String_2d_Vector[2][3].toDouble(&ok)-String_2d_Vector[1][3].toDouble(&ok));


    if ((row>0)&&(col>0))
    {

        for( int i_col = 1; i_col < row-1 ; i_col++ )
        {
            hour_aktuell = String_2d_Vector[i_col][3].toDouble(&ok);
            hour_next = String_2d_Vector[i_col+1][3].toDouble(&ok);


            for( int i_min = 0; i_min <24*3600 ; i_min++ )
            {
                if((i_min/(float)3600 >= hour_aktuell)&(i_min/(float)3600 < hour_next))
                {

                    SunDataYear.push_back((String_2d_Vector[i_col])[0].toDouble(&ok));
                    SunDataMonth.push_back(String_2d_Vector[i_col][1].toDouble(&ok));
                    SunDataDay.push_back(String_2d_Vector[i_col][2].toDouble(&ok));
                    SunDataHour.push_back(hour_aktuell);
                    SunDataIrradiation.push_back(String_2d_Vector[i_col][4].toDouble(&ok)+(String_2d_Vector[i_col+1][4].toDouble(&ok)-String_2d_Vector[i_col][4].toDouble(&ok))*((i_min/(float)3600-hour_aktuell)/timestep));
                    SunDataIntensity.push_back(SunDataIrradiation[(SunDataIrradiation.size()-1)]/timestep);
                    time_last = time_last + minute;
                    SunDataTimeLine.push_back(time_last);


                }
            }
        }

    }

}


void SolarData::Plot_Solar_Data(QWidget * parent)
{

}
