#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "solardata.h"
#include "pvsimulation.h"
#include <QDebug>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)

{


    ui->setupUi(this);

    QPixmap  mypix(":/images/IoG-At-small.jpg");
    ui->label_Image->setPixmap(mypix);



    PVSimulation * mySolar = new PVSimulation(parent);
    mySolarSim = mySolar;

    mySolarSim->create_Solar_File_list();
    ui->comboBox_country->clear();
    ui->comboBox_country->addItems( mySolarSim->Output_Country_List());

}

MainWindow::~MainWindow()
{


    delete ui;
}
void MainWindow::makePlot()
{

    ui->customPlot->setNoAntialiasingOnDrag(true); // more performance/responsiveness during dragging
    ui->customPlot->addGraph();
    QPen pen;
    pen.setColor(QColor(255,170,100));
    pen.setWidth(2);
    pen.setStyle(Qt::DotLine);
    ui->customPlot->graph(0)->setPen(pen);
    ui->customPlot->graph(0)->setName("x");

    ui->customPlot->addGraph();
    ui->customPlot->graph(1)->setPen(QPen(Qt::red));
    ui->customPlot->graph(1)->setBrush(QBrush(QColor(255, 0, 0, 20)));
    ui->customPlot->graph(1)->setName("-sin(x)exp(x)");

    ui->customPlot->addGraph();
    ui->customPlot->graph(2)->setPen(QPen(Qt::blue));
    ui->customPlot->graph(2)->setBrush(QBrush(QColor(0, 0, 255, 20)));
    ui->customPlot->graph(2)->setName(" sin(x)exp(x)");

    ui->customPlot->addGraph();
    pen.setColor(QColor(0,0,0));
    pen.setWidth(1);
    pen.setStyle(Qt::DashLine);
    ui->customPlot->graph(3)->setPen(pen);
    ui->customPlot->graph(3)->setBrush(QBrush(QColor(0,0,0,15)));
    ui->customPlot->graph(3)->setLineStyle(QCPGraph::lsStepCenter);
    ui->customPlot->graph(3)->setName("x!");

    const int dataCount = 200;
    const int dataFactorialCount = 21;
    QVector<QCPGraphData> dataLinear(dataCount), dataMinusSinExp(dataCount), dataPlusSinExp(dataCount), dataFactorial(dataFactorialCount);
    for (int i=0; i<dataCount; ++i)
    {
      dataLinear[i].key = i/10.0;
      dataLinear[i].value = dataLinear[i].key;
      dataMinusSinExp[i].key = i/10.0;
      dataMinusSinExp[i].value = -qSin(dataMinusSinExp[i].key)*qExp(dataMinusSinExp[i].key);
      dataPlusSinExp[i].key = i/10.0;
      dataPlusSinExp[i].value = qSin(dataPlusSinExp[i].key)*qExp(dataPlusSinExp[i].key);
    }
    for (int i=0; i<dataFactorialCount; ++i)
    {
      dataFactorial[i].key = i;
      dataFactorial[i].value = 1.0;
      for (int k=1; k<=i; ++k) dataFactorial[i].value *= k; // factorial
    }
    ui->customPlot->graph(0)->data()->set(dataLinear);
    ui->customPlot->graph(1)->data()->set(dataMinusSinExp);
    ui->customPlot->graph(2)->data()->set(dataPlusSinExp);
    ui->customPlot->graph(3)->data()->set(dataFactorial);

    ui->customPlot->yAxis->grid()->setSubGridVisible(true);
    ui->customPlot->xAxis->grid()->setSubGridVisible(true);
    ui->customPlot->yAxis->setScaleType(QCPAxis::stLogarithmic);
    ui->customPlot->yAxis2->setScaleType(QCPAxis::stLogarithmic);
    QSharedPointer<QCPAxisTickerLog> logTicker(new QCPAxisTickerLog);
    ui->customPlot->yAxis->setTicker(logTicker);
    ui->customPlot->yAxis2->setTicker(logTicker);
    ui->customPlot->yAxis->setNumberFormat("eb"); // e = exponential, b = beautiful decimal powers
    ui->customPlot->yAxis->setNumberPrecision(0); // makes sure "1*10^4" is displayed only as "10^4"
    ui->customPlot->xAxis->setRange(0, 19.9);
    ui->customPlot->yAxis->setRange(1e-2, 1e10);
    // make range draggable and zoomable:
    ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

    // make top right axes clones of bottom left axes:
    ui->customPlot->axisRect()->setupFullAxesBox();
    // connect signals so top and right axes move in sync with bottom and left axes:
    connect(ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->yAxis2, SLOT(setRange(QCPRange)));

    ui->customPlot->legend->setVisible(true);
    ui->customPlot->legend->setBrush(QBrush(QColor(255,255,255,150)));
    ui->customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft|Qt::AlignTop); // make legend align in top left corner or axis rect

}



void MainWindow::on_Button_Load_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);

}

void MainWindow::on_pushButton_6_clicked()
{
    MainWindow::makePlot();
}

void MainWindow::on_pushButton_3_clicked()
{
        ui->stackedWidget->setCurrentIndex(1);


}

void MainWindow::on_Button_LoadSun_clicked()
{
        mySolarSim->Output_Filepath(ui->comboBox_City->currentIndex());
        mySolarSim->Import_solar_Data("");

        ui->label_Lat_Longitude->setText(QString("Lat.: %1").arg(mySolarSim->Output_latitude(),3)+QString("     Long.: %1").arg(mySolarSim->Output_longitude(),3));



        // create graph and assign data to it:
        ui->GaphPlotSun->addGraph();
        ui->GaphPlotSun->graph(0)->setData(mySolarSim->Time(), mySolarSim->SunData());
        // give the axes some labels:
        ui->GaphPlotSun->xAxis->setLabel("x");
        ui->GaphPlotSun->yAxis->setLabel("y");
        // set axes ranges, so we see all data:
        ui->GaphPlotSun->xAxis->setRange(mySolarSim->Time()[0], mySolarSim->Time()[mySolarSim->Time().size()-1]);
        ui->GaphPlotSun->yAxis->setRange(-10, 1300);
        ui->GaphPlotSun->replot();

}

void MainWindow::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_pushButton_2_clicked()
{


    int count=1;


    if(ModuleType_Vector.size()==0)
    {
        controlsLayout = new QHBoxLayout;
    }

    count = ModuleType_Vector.size()+1;

    QLabel* Label_Module_type = new QLabel("Module type");
    QLabel* Orientation_Vertical = new QLabel("Vertical Orientation");
    QLabel* Orientation_Horizontal = new QLabel("Vertical Horizontal");
    QLabel* Lable_N_Modules = new QLabel("Number of Modules");
    QComboBox* CobBox_Module_type = new QComboBox();
    QSpinBox* N_Modules = new QSpinBox();



    QGroupBox* groupbox = new QGroupBox("PV String "+QString::number(count));
    QVBoxLayout* subLayout= new QVBoxLayout;


    ModuleType_Vector.append(CobBox_Module_type);
    ModuleNumber_Vector.append(N_Modules);

    groupbox->setLayout(subLayout);

    subLayout->addWidget(Label_Module_type);
    subLayout->addWidget(CobBox_Module_type);
    subLayout->addWidget(Lable_N_Modules);
    subLayout->addWidget(N_Modules);
    N_Modules->setValue(1);
    subLayout->addWidget(Orientation_Vertical);
    subLayout->addWidget(Orientation_Horizontal);


    controlsLayout->addWidget(groupbox);
    ui->widget->setLayout(controlsLayout);

   groupbox->show();


}

void MainWindow::on_pushButton_7_clicked()
{

   // ui->pushButton_7->setText(QString::number(ModuleNumber_Vector[ModuleNumber_Vector.size()-1]->value()));

}

void MainWindow::on_pushButton_5_clicked()
{


    ui->stackedWidget->setCurrentIndex(3);

}

void MainWindow::on_pushButton_11_clicked()
{
      mySolarSim->Output_Filepath(ui->comboBox_City->currentIndex());

     //PVSimulation myPVSim(this);
     mySolarSim->Start_simuation();

     // create graph and assign data to it:
     ui->GaphBatterySOC->addGraph();
     ui->GaphBatterySOC->graph(0)->setData(mySolarSim->Time()  ,mySolarSim->SOC());
     // give the axes some labels:
     ui->GaphBatterySOC->xAxis->setLabel("time");
     ui->GaphBatterySOC->yAxis->setLabel("SOC (%)");
     // set axes ranges, so we see all data:
     ui->GaphBatterySOC->xAxis->setRange(mySolarSim->Time()[0],mySolarSim->Time()[mySolarSim->Time().size()-1]);
     ui->GaphBatterySOC->yAxis->setRange(-10, 110);
     ui->GaphBatterySOC->replot();

     // create graph and assign data to it:
     ui->GraphBatVoltage->addGraph();
     ui->GraphBatVoltage->graph(0)->setData(mySolarSim->Time()  ,mySolarSim->get_U_Bat());
     // give the axes some labels:
     //ui->GraphBatVoltage->xAxis->setScaleType(QCPAxis::stLogarithmic);
     ui->GraphBatVoltage->xAxis->setLabel("time");
     ui->GraphBatVoltage->yAxis->setLabel("Battery Voltage (V)");
     // set axes ranges, so we see all data:
     ui->GraphBatVoltage->xAxis->setRange(mySolarSim->Time()[0],mySolarSim->Time()[mySolarSim->Time().size()-1]);
     ui->GraphBatVoltage->yAxis->setRange(9, 15);


     ui->GraphBatVoltage->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
     // connect signals so top and right axes move in sync with bottom and left axes:
     connect(ui->GraphBatVoltage->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->xAxis, SLOT(setRange(QCPRange)));
  //   connect(ui->customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->yAxis2, SLOT(setRange(QCPRange)));


     ui->GraphBatVoltage->replot();

     // create graph and assign data to it:
     ui->GraphPlotSun2->addGraph();
     ui->GraphPlotSun2->graph(0)->setData(mySolarSim->Time()  ,mySolarSim->SunData());
     // give the axes some labels:
     ui->GraphPlotSun2->xAxis->setLabel("time");
     ui->GraphPlotSun2->yAxis->setLabel("Sun Intensity (W/m^2)");
     // set axes ranges, so we see all data:
     ui->GraphPlotSun2->xAxis->setRange(mySolarSim->Time()[0],mySolarSim->Time()[mySolarSim->Time().size()-1]);
     ui->GraphPlotSun2->yAxis->setRange(-10, 1300);
     ui->GraphPlotSun2->replot();

     // create graph and assign data to it:
     ui->GraphCapacityShortage->addGraph();
     ui->GraphCapacityShortage->graph(0)->setData(mySolarSim->Time()  ,mySolarSim->get_I_Load());
     // give the axes some labels:
     ui->GraphCapacityShortage->xAxis->setLabel("time");
     ui->GraphCapacityShortage->yAxis->setLabel("Load");
     // set axes ranges, so we see all data:
     ui->GraphCapacityShortage->xAxis->setRange(mySolarSim->Time()[0],mySolarSim->Time()[mySolarSim->Time().size()-1]);
     ui->GraphCapacityShortage->yAxis->setRange(-0, 0.5);
     ui->GraphCapacityShortage->replot();

}

void MainWindow::on_comboBox_country_currentIndexChanged(const QString &arg1)
{
    ui->comboBox_City->clear();
    ui->comboBox_City->addItems(mySolarSim->Output_City_List(ui->comboBox_country->currentText()));

}


void MainWindow::on_actionUpdate_input_files_triggered()
{
    mySolarSim->create_Solar_File_list();
    ui->comboBox_country->clear();
    ui->comboBox_country->addItems( mySolarSim->Output_Country_List());
}

void MainWindow::on_pushButton_10_clicked()
{

    // create graph and assign data to it:
    ui->BatteryCharacter->addGraph();
    ui->BatteryCharacter->graph(0)->setData(mySolarSim->get_BatteryCharacteristic_time()  ,mySolarSim->get_BatteryCharacteristic((double)0.35,(double)7));
    ui->BatteryCharacter->addGraph();
    ui->BatteryCharacter->graph(1)->setData(mySolarSim->get_BatteryCharacteristic_time()  ,mySolarSim->get_BatteryCharacteristic((double)0.64,(double)6.4));
    ui->BatteryCharacter->addGraph();
    ui->BatteryCharacter->graph(2)->setData(mySolarSim->get_BatteryCharacteristic_time()  ,mySolarSim->get_BatteryCharacteristic((double)1.18,(double)5.9));
    ui->BatteryCharacter->addGraph();
    ui->BatteryCharacter->graph(3)->setData(mySolarSim->get_BatteryCharacteristic_time()  ,mySolarSim->get_BatteryCharacteristic((double)4.2,(double)4.2));
    // give the axes some labels:
    ui->BatteryCharacter->xAxis->setScaleType(QCPAxis::stLogarithmic);
    ui->BatteryCharacter->xAxis->setLabel("time");
    ui->BatteryCharacter->yAxis->setLabel("Battery Voltage (V)");
    // set axes ranges, so we see all data:
    ui->BatteryCharacter->xAxis->setRange(mySolarSim->get_BatteryCharacteristic_time()[0],25);//mySolarSim->get_BatteryCharacteristic_time()[mySolarSim->get_BatteryCharacteristic_time().size()-1]);
    ui->BatteryCharacter->yAxis->setRange(8, 13);
    ui->BatteryCharacter->replot();

}

void MainWindow::on_pushButton_12_clicked()
{
    int index = ui->stackedWidget->currentIndex();


        ui->stackedWidget->setCurrentIndex(index+1);

}

void MainWindow::on_pushButton_13_clicked()
{
    ui->stackedWidget->setCurrentIndex(ui->stackedWidget->currentIndex()-1);

}
