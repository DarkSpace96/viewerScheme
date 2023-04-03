#include <QApplication>
#include <QSettings>

#include <ModelTable/model.h>

#include <ModelTable/device.h>
#include <ModelTable/DevLor.h>
#include <ModelTable/DevOnOff.h>
//#include <ModelTable/DevVideoCapture.h>
//#include <ModelTable/DevClient.h>
#include <ModelTable/DevDisk.h>
#include <ModelTable/DevNT102_3.h>
//#include <ModelTable/DevNT16.h>
//#include <ModelTable/DevNT19.h>
#include <ModelTable/DevLamps.h>
//#include <ModelTable/DevZenit.h>
#include <ModelTable/DevPort.h>

#include "logicscheme.h"
#include "resource/struct.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LogicScheme logic;
    QSettings setting(":/settings.ini", QSettings::IniFormat);

    QVector<DevCam *> devCam;
    QVector<DevLor *> devLor;
    QVector<DevOnOff *> devNT211;
//    QVector<DevClient *> devClient;
    QVector<DevNT102_3 *> devNT102;
    QVector<DevLamps *> devLamps;
//    DevVideoCapture *devVideoCapture;

//    DevZenit *devZenit;
//    DevNT16 *devNT16;
//    DevNT19 *devNT19;
    DevPort *devPort;


    Model *modelMIC, *modelZary;
    QList<Model *> model, modelLight, modelNT203, modelNT211, modelNT03;
    QList<QString> nameModel;
    DataMesseg messeg;
//    GridData data;
    uchar nInterfaceNT211 = 0x03;
    int i;
    int n, n1;
    char buft[32];

    nameModel.append("КУ 1");
    nameModel.append("КУ 2");
    nameModel.append("КУ 3");
    nameModel.append("КУ 4");
    nameModel.append("КУ 5");
    nameModel.append("КУ 6");
    nameModel.append("КУ 7");
    nameModel.append("КУ 8");
    nameModel.append("КУ 9");
    nameModel.append("КУ 10");
    nameModel.append("КУ 11");
    nameModel.append("КУ 12");

    nameModel.append("Лор 1-5");
    nameModel.append("Лор 6-8");

//    nameModel.append("Монитор");
//    nameModel.append("ДВ");
//    nameModel.append("НИА");
//    nameModel.append("Руководитель");
//    nameModel.append("Switch 1");
//    nameModel.append("Switch 2");




//    nameModel.append("CPU");

    nameModel.append("светильник/1/1");
    nameModel.append("светильник/1/2");
    nameModel.append("светильник/1/3");
    nameModel.append("светильник/1/4");

    nameModel.append("светильник/2/1");
    nameModel.append("светильник/2/2");
    nameModel.append("светильник/2/3");
    nameModel.append("светильник/2/4");

    nameModel.append("светильник/3/1");
    nameModel.append("светильник/3/2");
    nameModel.append("светильник/3/3");
    nameModel.append("светильник/3/4");

    nameModel.append("светильник/4/1");
    nameModel.append("светильник/4/2");

    nameModel.append("светильник/5/1");
    nameModel.append("светильник/5/2");
    nameModel.append("светильник/5/3");
    nameModel.append("светильник/5/4");

    nameModel.append("светильник/6/1");
    nameModel.append("светильник/6/2");

    nameModel.append("НТ-102-2/1");
    nameModel.append("НТ-102-2/2");
    nameModel.append("НТ-102-2/3");

    nameModel.append("Диск 1");
    nameModel.append("Диск 2");


//    for (int i = 0; i < 6; ++i) {
//        sprintf(buft, "devNT211-%d", i);
//        devNT211.append(new DevOnOff(QString(buft)));
//        devNT211[i]->setInterfaceCanal(nInterfaceNT211);
//        devNT211[i]->setSizeState(6);
//        devNT211[i]->setSizeDate();
//        nInterfaceNT211 += 0x11;
//        for (int channel = 0; channel < 6; ++channel) {
//            QString strSetting = QString("devNT211-%1/channel-%2").arg(i).arg(channel);
//            if (!setting.value(strSetting).isNull()) {
//                int idDevice = setting.value(strSetting).toInt();
//                devNT211[i]->setChannel(channel, idDevice);
//            }
//        }
//    }

    for (int i = 0; i < CAM; ++i) {
        devCam.append(new DevCam(QString(buft)));
        devCam[i]->setName(QString::number(i + 1));
        devCam[i]->setNumber(i);
        int interface = setting.value(QString("devCam%1/interface").arg(i))
                .toString().toInt(nullptr, 16);
        int channel = setting.value(QString("devCam%1/channel").arg(i))
                .toString().toInt(nullptr, 16);
        devCam[i]->setInterfaceCanal(interface, channel);
        int course = setting.value(QString("devCam%1/course").arg(i))
                .toInt();
        int place = setting.value(QString("devCam%1/place").arg(i))
                .toInt();
        int viewingAngle = setting.value(QString("devCam%1/viewingAngle").arg(i))
                .toInt();
        int minFocus = setting.value(QString("devCam%1/minFocus").arg(i))
                .toInt();
        int maxFocus = setting.value(QString("devCam%1/maxFocus").arg(i))
                .toInt();
        int minScale = setting.value(QString("devCam%1/minScale").arg(i))
                .toInt();
        int maxScale = setting.value(QString("devCam%1/maxScale").arg(i))
                .toInt();
        int minCourse = setting.value(QString("devCam%1/minCourse").arg(i))
                .toInt();
        int maxCourse = setting.value(QString("devCam%1/maxCourse").arg(i))
                .toInt();
        int minPlace = setting.value(QString("devCam%1/minPlace").arg(i))
                .toInt();
        int maxPlace = setting.value(QString("devCam%1/maxPlace").arg(i))
                .toInt();
        devCam[i]->setAnglStart(propertyValue::COURSE, course);
        devCam[i]->setAnglStart(propertyValue::PLACE, place);
        devCam[i]->setValueMinMax(propertyValue::FOCUS, minFocus, maxFocus);
        devCam[i]->setValueMinMax(propertyValue::SCALE, minScale, maxScale);
        devCam[i]->setValueMinMax(propertyValue::COURSE, minCourse, maxCourse);
        devCam[i]->setValueMinMax(propertyValue::PLACE, minPlace, maxPlace);
        devCam[i]->setValue(propertyValue::FOCUS, 0);
        devCam[i]->setValue(propertyValue::SCALE, 0);
        devCam[i]->setValue(propertyValue::COURSE, 0);
        devCam[i]->setValue(propertyValue::PLACE, 0);
        devCam[i]->setViewingAngle(viewingAngle);
        n = i;
        sprintf(buft, "Cam_%d", i + 1);
        model.append(new Model(buft, nameModel[n], devCam[n]));
        devCam[i]->setStateDev(RUN);
        model[n]->clearData();
    }
    for (int i = 0; i < CAM_IP; ++i) {
        sprintf(buft, "devLor_%d", i);
        devLor.append(new DevLor(QString(buft)));
        devLor[i]->setName("Лор" + QString::number(i + 1));
        devLor[i]->setNumber(Lor1 + i);
        int lastIp = setting.value(QString("devLor-%1/lastIp").arg(i)).toInt();
        QString ip = QString("192.168.1.%1").arg(lastIp);
        devLor[i]->setIpAddress(ip);
    }

    sprintf(buft, "devLor_%d", 1);
    model.append(new Model(buft, nameModel[12], devLor[0]));
    devLor[0]->setStateDev(RUN);
    model[12]->clearData();

    sprintf(buft, "devLor_%d", 2);
    model.append(new Model(buft, nameModel[13], devLor[7]));
    model[13]->clearData();

    n1 = model.size();
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 4; ++j) {
            if ((i == 3 || i == 5) && j == 2)
                break;
            sprintf(buft, "Lamp_%01d%01d", i, j);
            devLamps.append(new DevLamps(QString(buft)));
            n = devLamps.size() - 1;
            devLamps[n]->setGroup(i);
            devLamps[n]->setNumber(j, n);
            devLamps[n]->setName("Гр " + QString::number(i + 1) + " Светильник № "  + QString::number(j + 1));

            sprintf(buft, "devLamps-%d_%d", i + 1, j + 1);

            model.append(new Model(buft, nameModel[n + n1], devLamps[n]));
            devLamps[n]->setStateDev(RUN);
            model[n + n1]->clearData();
        }
    }
    n1 = model.size();
    for (int i = 0; i < 3; ++i) {
        sprintf(buft, "devNT-102-2_%d", i + 1);
        devNT102.append(new DevNT102_3(QString(buft)));
        devNT102[i]->setNumber(i);
        int interface = setting.value(QString("devNT102-%1/interface").arg(i)).toInt();
        int channel = setting.value(QString("devNT102-%1/channel").arg(i)).toString().toInt(nullptr, 16);
        devNT102[i]->setInterfaceCanal(interface, channel);

        model.append(new Model(buft, nameModel[i + n1], devNT102[i]));
        devNT102[i]->setStateDev(RUN);
        model[n1 + i]->clearData();
    }



    sprintf(buft, "devDisk_%d", 0);
    DevDisk *disk_1 = new DevDisk(QString(buft), "hard1");
    sprintf(buft, "devDisk_%d", 1);
    DevDisk *disk_2 = new DevDisk(QString(buft), "hard2");

    disk_1->setNextDisk(disk_2);
    disk_1->setNumber(0);
    disk_1->setName("1");
    disk_2->setNextDisk(disk_1);
    disk_2->setNumber(1);
    disk_2->setName("2");
    disk_1->bExists = true;
    disk_2->bExists = true;

    sprintf(buft, "devDisk_%d", 1);
    model.append(new Model(buft, nameModel[model.size()], disk_1));
    sprintf(buft, "devDisk_%d", 2);
    model.append(new Model(buft, nameModel[model.size()], disk_2));

    sprintf(buft, "devPort_%d", 0);
    devPort = new DevPort(QString(buft));
    sprintf(buft, "devPort_%d", 1);
    model.append(new Model(buft, "", devPort));



//    devVideoCapture = new DevVideoCapture("devVideoCapture");

//    for (int i = 0; i < 3; ++i) {
//        sprintf(buft, "devClient_%d", i);
//        devClient.append(new DevClient(QString(buft), i + 1));
//    }

//    sprintf(buft, "devNT16_%d", 0);
//    devNT16 = new DevNT16(QString(buft));
//    devNT16->setInterfaceCanal(0xE4, 0x00);


//    sprintf(buft, "devNT19_%d", 0);
//    devNT19 = new DevNT19(QString(buft));





//    for (int i = 0; i < devLamps.size(); ++i) {
//        int interface = setting.value(QString("devLamp-%1/interface").arg(i)).toString().toInt(nullptr, 16);
//        int channel = setting.value(QString("devLamp-%1/channel").arg(i)).toString().toInt(nullptr, 16);
//        int channel102 = setting.value(QString("devLamp-%1/channel102").arg(i)).toInt();
//        devLamps[i]->setInterfaceCanal(interface, channel);
//        devLamps[i]->setNpch(channel102);
//    }

//    devZenit = new DevZenit(&messeg, &data, "Zenit");

//    for (i = 0; i < CAM; i++) {
//        sprintf(buft, "Cam_%d", i + 1);

//        model.append(new Model(buft, nameModel[i], devCam[i]));
//        devCam[i]->setStateDev(RUN);
//        model[i]->clearData();
//        //        model[i]->setStateDev(RUN);
//    }
//    sprintf(buft, "devLor_%d", 1);
//    model.append(new Model(buft, nameModel[12], devLor[0]));
//    devLor[0]->setStateDev(RUN);
//    model[12]->clearData();

//    sprintf(buft, "devLor_%d", 2);
//    model.append(new Model(buft, nameModel[13], devLor[7]));
//    model[13]->clearData();

//    n = model.size();
//    for (i = n; i < devLamps.size() + n; i++) {
//        sprintf(buft, "devLamps-%d_%d", i + 1);

//        model.append(new Model(buft, nameModel[i], devCam[i]));
//        devCam[i]->setStateDev(RUN);
//        model[i]->clearData();
//        //        model[i]->setStateDev(RUN);
//    }


    logic.initScheme(0);
    logic.showShceme();
    for (i = 0; i < model.size(); i++) {
        logic.addModel(model[i]);
    }


    for (i = 0; i < CAM; i++) {
        devCam[i]->setData(HUMIDITY, 30);
        devCam[i]->setData(TEMPERATURE, 25);
        devCam[i]->setData(VOLTAGE, 15);
        devCam[i]->setData(TIME, 200);
        devCam[i]->setData(PING, 10);
        model[i]->setStateCI(0, true);
        model[i]->setStateCI(1, true);
    }

    for (i = 0; i < devLamps.size(); i++) {
        devLamps[i]->setData(HUMIDITY, 30);
        devLamps[i]->setData(TEMPERATURE, 25);
        devLamps[i]->setData(VOLTAGE, 15);
        devLamps[i]->setData(TIME, 200);
    }

    for (i = 0; i < devNT102.size(); i++) {
        devNT102[i]->setData(HUMIDITY, 30);
        devNT102[i]->setData(TEMPERATURE, 25);
        devNT102[i]->setData(TIME, 200);
    }

    devPort->setStateDev(RUN);

    model[5]->setStateCI(0, false);
    model[4]->setStateCI(1, false);


    devLor[0]->setData(PING, 10);
    devLor[7]->setData(PING, 10);
    devLor[0]->setStateDev(RUN);
    devLor[7]->setStateDev(RUN);

    disk_1->setStateDev(RUN);
    disk_2->setStateDev(RUN);
    disk_1->setData(DISK_SPACE, 3000);
    disk_2->setData(DISK_SPACE, 2000);


    devCam[5]->setStateDev(ERROR);


//    model[0]->setValue(TEMPERATURE, 80);
//    model[0]->setValue(PING, NON_VALUE);
//    model[1]->setStateDev(ERROR);

    return a.exec();
}
