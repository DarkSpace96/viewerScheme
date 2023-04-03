#include "DevZenit.h"

DevZenit::DevZenit(DataMesseg *messeg, GridData *data, QString name, QObject *parent) : DevSensor(name, parent) {
int i, j;
unsigned short int un;
    cp = 'S';
    cl = 'W';
    this->messeg = messeg;
    this->messeg->ncp = this->messeg->npp = 0;
    this->messeg->nhp = data->height;
    fillAngle(&this->messeg->latitibe, 59, 29, 29, cl);
    fillAngle(&this->messeg->longitube, 59, 29, 29, cp);
    gridData = data;
#ifdef Q_OS_UNIX
    dataBaseSql = "/home/database/targets.sqlite";
#endif
#ifdef Q_OS_WINDOWS
    dataBaseSql = "D:/5507-1/targets.sqlite";
#endif
    bufc = new unsigned short int[8192];
    bufd = bufc + 4096;
    memset(bufc, 0, 16384);
    bufd[4064] = 0x0800;
    bufd[4066] = 21845;
    bufd[4067] = 43690;
    j = 1;
    for (i = 0; i < 16; i++) {
        bufd[4068 + i] = j;
        j *= 2;
    }
    bufd[4084] = 65535;
    bufd[4085] = 0x8000;
    un = 0;
    for (i = 0; i < 31; i++)
        un += bufd[4064 + i];
    bufd[4095] = un & 0xffff;
    for (i = 0; i < 32; i++)
        bufd[i] = bufd[1984 + i] = bufd[2048 + i] = bufd[4064 + i];

    nzone_w = 84;
    nzone_h = 126;

    arrDate = new int[20];

//    arrDate[0] = 1005;// резервный параметр
//    arrDate[1] = 90;// Отстояние от дна(Н), м
//    arrDate[2] = 95;// Отстояние от дна(Ц), м
//    arrDate[3] = 97;// Отстояние от дна(К), м
//    arrDate[4] = -35755;// угол килевой качки
//    arrDate[5] = 36255;// курс комплексный
//    arrDate[6] = 641;// продольная составляющая абсолютной скорости
//    arrDate[7] = 202;// продольная составляющая относительной скорости
//    arrDate[8] = 153;// поперечная составляющая абсолютной скорости;
//    arrDate[9] = 104;// поперечная составляющая относительной скорости
//    arrDate[10] = 89;// вертикальная составляющая абсолютной скорости
//    arrDate[11] = 50;//
//    arrDate[12] = 50;//
//    arrDate[13] = 2700;//

    arrDate[0] = 1005;// Частота вращения М/Д об/с
    arrDate[1] = 90;// Частота вращения РД1 об/с
    arrDate[2] = 95;// Частота вращения РД2 об/с
    arrDate[3] = 97;// Частота вращения НВПУ1 об/с
    arrDate[4] = -35755;// Частота вращения НВПУ2 об/с
    arrDate[5] = 36255;// Частота вращения КВПУ об/с
    arrDate[6] = 641;// Частота вращения НГПУ об/с
    arrDate[7] = 202;// Частота вращения КГПУ об/с
    arrDate[8] = 153;// Курс заданный градус (0,0)
    arrDate[9] = 104;// Глубина м (0,0)
    arrDate[10] = 89;// Дифферент градус (0,0)
    arrDate[11] = 50;// Курс градус (0,0)
    arrDate[12] = 50;// Продольная составляющая относительной скорости м/с (0,0)
    arrDate[13] = 2700;// Продольная составляющая абсолютной скорости м/с (0,0)
    arrDate[14] = 2700;// Поперечная составляющая относительной скорости м/с (0,0)
    arrDate[15] = 2700;// Поперечная составляющая абсолютной скорости м/с (0,0)
    arrDate[16] = 2700;// Скорость вспл/погр м/с (0,0)
    arrDate[17] = 2700;// Отстояние в носу м (0,0)
    arrDate[18] = 2700;// Отстояние в средней точке м (0,0)
    arrDate[19] = 2700;// Отстояние в корме м (0,0)


    showingFromZarya = false;
#ifdef Q_OS_UNIX
    ptmk = NULL;
    ptmkth = new TmkThread(NHTMK, this);
    QObject::connect(ptmkth, &TmkThread::tmk_cr, this, &DevZenit::tmkCreated);
    QObject::connect(ptmkth, &TmkThread::tmkNotCreat, this,
                     &DevZenit::tmkNotCreated);
    ptmkth->start();
#endif
    tmr.setInterval(30000);
    tmr_z.setInterval(5000);
    QObject::connect(&tmr, &QTimer::timeout, this, &DevZenit::clearbTA13U);
    QObject::connect(&tmr_z, &QTimer::timeout, this, &DevZenit::checkZar);

//    initDb();
}

//GridData DevZenit::getGridData()
//{
//    return gridData;
//}

DevZenit::~DevZenit(){}

//void DevZenit::setDataMesseg(DataMesseg *messeg) {
//    this->messeg = messeg;
//}

void DevZenit::setMarks(int nCam, unsigned short un) {
//int n = 2048 * NHTMK + 18 * 32;
//    bufd[n + 1] = nCam;
//    bufd[n + 2] = un;
//    bufd[n + 3] = 0xC000;
//    bufd[n + 4] = 5;
//    bufd[n + 5] = 3;
}

int *DevZenit::getArrDate() {
    return arrDate;
}

QString DevZenit::getDataBaseSql() {
    return dataBaseSql;
}

void DevZenit::tmkNotCreated(int n) {
    Q_UNUSED(n);
#ifdef Q_OS_UNIX
    ptmkth->quit();
    ptmkth->wait();
    delete ptmkth;
    ptmkth = NULL;
    ptmk = NULL;
    setDistance(messeg->nhp, 100);
#endif
    bTA1_3U[0] = false;
    bTA1_3U[1] = false;
    emit errorMessage(QString("Ошибка: \"Зенит\" не отвечает"));
}

void DevZenit::tmkCreated(int n) {
#ifdef Q_OS_UNIX
    ptmk = ptmkth->getTmk();
    ptmk->setBuf(bufc + n * 2048, bufd + NHTMK * 2048);
//    statusScreen->upDate();
    //when MI was got from Zarya
    QObject::connect(ptmk, &Tmk::miGot, this, &DevZenit::miRecv);
    ptmk->fillsSubad(1);
#endif
    bTA1_3U[0] = true;
    fillBufd0();
}

void DevZenit::fillBufd0() {
//updates MI19 exclude mark
//0 - service word, 1 - active camera number, 2 - PI7, 3 - PI8, 4 - target type,
//5 - detecting probability, 6 - distance, 7/8 - zone width/height
int i, no, nf;
unsigned short int us = 0, un = 1;
#ifdef Q_OS_UNIX
    if (ptmk == NULL)
        return;
#endif
    no = 18 * 32;
//    nf = NHTMK * 2048;
#ifdef Q_OS_UNIX
    bzero(bufd + nf + no, 64);
#endif
    memset(bufd + nf + no, 0, 64);
    bufd[nf + no] = 0x9800;
    bufd[nf + no + 1] = 0;//camMenu->getScr();
    bufd[nf + no + 3] = bufd[nf + no + 4] = bufd[nf + no + 5] = 0;
//    for (i = 0; i < CAMERAS; i++) {
//        if (camerasInfo[i]->isTurnedOn())
//            bufd[nf + no + 2] += un;
//        un *= 2;
//    }
    bufd[nf + no + 6] = messeg->nhp;
    bufd[nf + no + 7] = nzone_w;
    bufd[nf + no + 8] = nzone_h;
    for (i = 0; i < 31; i++)
        us += bufd[nf + no + i];
    bufd[nf + no + 31] = us & 0xffff;

#ifdef Q_OS_UNIX
    ptmk->fillsSubad(19);
#endif
}

void DevZenit::miRecv(int nm, int nt) {
QDateTime targetDateTime;
char buf[128];
int nc, no;
unsigned short int nh;

    bTA1_3U[1] = true;
//    statusScreen->upDate();
    if (tmr.isActive())
        tmr.stop();
    tmr.start();
    nc = 32 * (nm - 1);
    no = 2048 * nt;
    switch (nm) {
    case NMT:
        fprintf(stdout, "MT: %d %u %0X\n", nt, bufc[no + nc + 2],
                bufc[no + nc + 21]);
        fflush(stdout);
        break;
    case 6:
        if ((bufc[no + nc + 1]) == SHOW_TARGET_MI6 && !showingFromZarya) {
            getDttm(&dt6, bufc + no + nc + 7);
            targetDateTime = QDateTime(QDate(dt6.myear, dt6.month, dt6.mday),
                                       QTime(dt6.mhour, dt6.min, dt6.msec));
            fprintf(stdout, "dttm_06: %d %u %u %u %02u %02u %02u\n", nt,
                    dt6.myear, dt6.month, dt6.mday, dt6.mhour, dt6.min,
                    dt6.msec);
            fflush(stdout);

            getTargetMi6(targetDateTime.addYears(2000));
        }
        break;
    case 11:
        bzar = true;
        cntz++;
        if (cntz == 18) {
            cntz = 0;
            emit dateFromOnOff(false);
//            if (dateForm->tmr.isActive())
//                dateForm->tmr.stop();
            if (tmr_z.isActive())
                tmr_z.stop();
            tmr_z.start();
        }
        arrDate[9] = bufc[no + nc + 14];
        if (!bdate1) {
            getDttm(&dt6_1, bufc + no + nc + 4);	//MI6.8
            fprintf(stdout, "dttm_11_1: %d %u %u %u %u %u %u\n", nt, dt6_1.myear,
                    dt6_1.month, dt6_1.mday, dt6_1.mhour, dt6_1.min, dt6_1.msec);
            fflush(stdout);
            bdate1 = true;
            return;
        }
        getDttm(&dt6, bufc + no + nc + 4);	//MI6.8
        fprintf(stdout, "dttm_11: %d %u %u %u %u %u %u\n", nt, dt6.myear,
                dt6.month, dt6.mday, dt6.mhour, dt6.min, dt6.msec);
        fflush(stdout);
        sprintf(buf, "%02d.%02d.20%02d", dt6.mday, dt6.month, dt6.myear);
        messeg->date = QString(buf);
        sprintf(buf + 16, "%02d:%02d:%02d", dt6.mhour, dt6.min, dt6.msec);
        messeg->time = QString(buf + 16);
        if (!bdate) {
            if (dt6_1.myear == dt6.myear && dt6_1.month == dt6.month && dt6_1.mday == dt6.mday &&
                    dt6_1.mhour == dt6.mhour && dt6_1.min == dt6.min && dt6_1.msec == dt6.msec)
                return;
            fprintf(stdout, "qsl_to: %d %s %s\n", nt, buf, buf + 16);
            fflush(stdout);
            setDateTime(dt6.mhour, dt6.min, dt6.msec, dt6.mday, dt6.month, dt6.myear, true);
        }
//        getAng2(bufc + no + nc + 6, &cp, &mpg, &mpm, &mps, &mpsf, &mpmf);	//MI6.4
//        getAng2(bufc + no + nc + 8, &cl, &mlg, &mlm, &mls, &mlsf, &mlmf);	//MI6.6

        if (cp == '+')
            cp = 'N';
        else {
            cp = 'S';
        }
        if (cl == '+')
            cl = 'E';
        else {
            cl = 'W';
        }
        fprintf(stdout, "phi: %c %d %d %d %d  lyambda: %c %d %d %d %d  %d\n", cp,
                mpg, mpm, mps, mpsf, cl, mlg, mlm, mls, mlsf, nt);
        fflush(stdout);
        fillAngle(&messeg->longitube, mpg, mpm, mpmf, cp);
        fillAngle(&messeg->latitibe, mlg, mlm, mlmf, cl);
        emit upFrame();
        break;
    case 12:
        union anglez ang;
        union val speed;
        short int n;

        n = bufc[no + nc + 6];
        arrDate[10] = n / 100;
//        arrDate[4] = n;
//        arrDate[4] *= 100;
        messeg->npp = n;
        gridData->pitchingY = n / 10.;

//        qDebug() << "zenit.npp" << zenit.npp ;

        n = bufc[no + nc + 7];
        messeg->ncp = n;
        gridData->pitchingX = n / 10.;

//        qDebug() << "zenit.ncp:" << zenit.ncp;

//        polyscreen->gridUpdate();

        ang.an2[1]=*(bufc + no + nc + 2);
        ang.an2[0]=*(bufc + no + nc + 3);
        arrDate[11] = ang.iang;

        speed.nval = bufc[no + nc + 10];
        arrDate[13] = (speed.nval);
        speed.nval = bufc[no + nc + 8];
        arrDate[12] = (speed.nval);
        speed.nval = bufc[no + nc + 11];
        arrDate[15] = (speed.nval);
        speed.nval = bufc[no + nc + 9];
        arrDate[14] = (speed.nval);
        speed.nval = bufc[no + nc + 12];
        arrDate[16] = (speed.nval);
        emit updateGrid();
        emit updateArr();
        break;
    case 17:
        //to clinet in m/10
        bool b = false;
        if (gridData->height / 10 != bufc[no + nc + 5])
            b = true;
        messeg->nhp = gridData->height = nh = bufc[no + nc + 5];
        gridData->height *= 10;
        qDebug() << "messeg->nhp:" << messeg->nhp;
        if (b) {
            emit updateGrid();
        }
        arrDate[17] = bufc[no + nc + 2];
        arrDate[18] = bufc[no + nc + 3];
        arrDate[19] = bufc[no + nc + 4];
        emit updateArr();

        for (int i = 17; i <= 19; i++)
            qDebug() << "arrDate[" << i << "]: " << arrDate[i];
        setDistance(messeg->nhp, 100);
        bufd[no + 18 * 32 + 6] = messeg->nhp;
        bufd[no + nc + 7] = nzone_w;
        bufd[no + 18 * 32 + 8] = nzone_h;
        fprintf(stdout, "h: %d  %d\n", nh, nt);
        fflush(stdout);
//        emit transmNarr();
        break;
    }
}

void DevZenit::setDateTime(int hh, int mm, int ss, int dd, int MM, int yyyy, bool bZenit) {
char buf[128];
bool b = false, bf;
QString *qStr = new QString(QChar('0'));
QStringList qsl;
QFile qf1;
QDir qd;

    sprintf(buf, "%02d.%02d.20%02d", dd, MM, yyyy);
    sprintf(buf + 16, "%02d:%02d:%02d", hh, mm, ss);

    fprintf(stdout, "setDateTime: %s %s\n", buf, buf + 16);
    fflush(stdout);
    qsl << QString::number(yyyy + 2000);
    qsl << QString::number(MM);
    qsl << QString::number(dd);
    qsl << QString::number(hh);
    qsl << QString::number(mm);
    qsl << QString::number(ss);

#ifdef Q_OS_UNIX
    b = QProcess::startDetached("/usr/local/bin/set_srv_time.elf", qsl);
#endif

    qd = QDir::current();
    *qStr = qd.absolutePath() + QString("/log/right_time");
    if (QFile::exists(*qStr))
        QFile::remove(*qStr);
    qf1.setFileName(*qStr);
    bf = qf1.open(QIODevice::ReadWrite);
    if (bf) {
        sprintf(buf, "%d_%02d_%02d_%02d_%02d_%02d\n", yyyy + 2000, MM, dd, hh, mm, ss);
        qf1.write(buf);
        qf1.close();
    }
    if (b) {
        if (bZenit)
            bdate = true;
        emit masseg(QString("Время установлено."));
    } else
        emit errorMessage(QString("Время не установлено!"));

}

void DevZenit::getDttm(struct dttm *pdttm, unsigned short int *buf) {
//got date and time from MI6 8,9
unsigned short int nb;
    nb = *buf;
    pdttm->myear = nb & 0x7f;
    nb >>= 7;
    pdttm->month = nb & 0x0f;
    nb >>= 4;
    pdttm->mday = nb & 0x1f;
    nb = *(buf + 1);
    pdttm->msec = (nb & 0x1f) * 2;
    nb >>= 5;
    pdttm->min = nb & 0x3f;
    nb >>= 6;
    pdttm->mhour = nb & 0x1f;
}

void DevZenit::setDistance(int nh, int n20) {
int nwh, ndc;
char buft[20];
    nwh = nh / n20;
    ndc = (nh % n20) * (100 / n20);
    sprintf(buft, "%01d.%02d", nwh, ndc);
    fprintf(stdout, "setDistance: %d %d  %d %d\n", nh, n20, nwh, ndc);
    fflush(stdout);
    messeg->distance = QString(buft);
    emit upFrame();
}

void DevZenit::fillAngle(QString *qStr, int ng, int nm, int nmm, char cp) {
//fills string for phi or lymbda,ng - degrees, nm - minutes,
//nmm - thousand minute fraction, cp - lattitude (N/S) or longitude (W/E)
char buft[60];
    sprintf(buft, "%d", ng);
    sprintf(buft + 4, "%02d.%03d'", nm, nmm);
    *qStr = QString(buft) + QString(QChar(0x00B0)) + QString(buft + 4) + QString(QChar(cp));
}

void DevZenit::checkZar() {
//checks whether Zarya works
    bzar = false;
    emit dateFromOnOff(true);
//    if (!dateForm->tmr.isActive())
//        dateForm->tmr.start(1000);
}

void DevZenit::clearbTA13U() {
    bTA1_3U[1] = false;
    emit dateFromOnOff(true);
//    dateForm->start();
//    statusScreen->upDate();
}

void DevZenit::getTargetMi6(QDateTime qdt) {
//int n;
//bool lor = false;
//QFileInfo qfi;
//QString fileName, jpgname;
//QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
//    db.setDatabaseName(dataBaseSql);
//    if (!db.open())
//        qDebug() << db.lastError();
//QSqlQuery q(db);
//    if (!q.prepare(QString
//                   ("SELECT * FROM targets WHERE dateTime BETWEEN ? and ? ")))
//        qDebug() << "qLastError()" << q.lastError();
//    q.addBindValue(qdt.addSecs(-2).toString("yyyy.MM.dd hh:mm:ss.zzz"));
//    q.addBindValue(qdt.addSecs(2).toString("yyyy.MM.dd hh:mm:ss.zzz"));

//    q.exec();
//    if (q.first()) {
//        fileName = QDir::homePath() + q.value(1).toString();
//        n = fileName.lastIndexOf("/");
//        jpgname = fileName.mid(n + 1);
//        qfi.setFile(fileName);
//        if (qfi.exists()) {
//            if (!fileName.contains(".avi"))
//                lor = true;
//            emit setFileName(fileName, lor);
//        } else {
//            QMessageBox::warning(0, "Внимание", QString("Зенит: Файл по данной метке отсутствует"));
//            emit masseg(QString("Зенит: Файл по данной метке отсутствует"));
//        }

//    } else {
//        QMessageBox::warning(0, "Внимание", QString("Зенит: Файл по данной метке отсутствует"));
//        emit masseg(QString("Зенит: Файл по данной метке отсутствует"));
//    }
//    db.close();
}

//QSqlError DevZenit::writeTargetInBase(QString camName, QString name) {
//QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
//    db.setDatabaseName(dataBaseSql);

//    if (!db.open())
//        return db.lastError();
//    fprintf(stdout, "write target db opened");
//    fflush(stdout);
//    QSqlQuery q(db);
//    q.prepare(QString("insert into targets (filePath, cameraNumber, dateTime, phi,lyambda) values (?,?,?,?,?)"));
//    q.addBindValue(name);
//    q.addBindValue(camName);
//    q.addBindValue(QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz"));
//    q.addBindValue(messeg->latitibe);
//    q.addBindValue(messeg->longitube);

//    if (!q.exec())
//        return q.lastError();
//    fprintf(stdout, "write target in db");
//    fflush(stdout);
//    db.close();
//    emit createCommit();
////    pfc12->show();
//    return QSqlError();
//}

//QSqlError DevZenit::writeQStcmnInBase(QString commit) {
//bool b3 = 0;
//QString qStr;
//QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
//    db.setDatabaseName(dataBaseSql);
//    if (!db.open())
//        return db.lastError();
//    QSqlQuery q(db);
//    b3 = q.exec("SELECT MAX(id) FROM targets");
//    if (!b3)
//        qDebug() << "errors_b3" << q.lastError();
//    q.isSelect();
//    q.last();
//    q.at();

//    qStr = QString("UPDATE targets set ") + tr("comment") +
//            QString(" = :comment where id = ") + q.value(0).toString();
//    q.prepare(qStr);
//    q.bindValue(":comment", commit);
//    if (!q.exec()) {
//        return q.lastError();
//    }
//    q.clear();
//    db.close();
//    return QSqlError();
//}

//QSqlError DevZenit::initDb() {
//QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
//    db.setDatabaseName(dataBaseSql);

//    if (!db.open())
//        return db.lastError();
//    QStringList tables = db.tables();
//    if (tables.contains("targets", Qt::CaseInsensitive))
//        return QSqlError();

//    QSqlQuery q;
//    if (!q.exec(QString("create table targets(id integer primary key, filePath varchar, cameraNumber integer, dateTime datetime, phi varchar, lyambda varchar, comment varchar)")))
//        return q.lastError();
//    db.close();

//    return QSqlError();
//}
