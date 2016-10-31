#ifndef CSV_HANDLING_H
#define CSV_HANDLING_H

#include <QMainWindow>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QDebug>
#include <QString>
#include <QStandardItemModel>

class CSV_handling
{
public:
    CSV_handling();
private:
    void checkString(QString &temp, QChar character);



    QList<QStringList> csv;
    QStandardItemModel *model;
    QList<QStandardItem*> standardItemList;
    void Open_CSV(QWidget *parent);
};

#endif // CSV_HANDLING_H
