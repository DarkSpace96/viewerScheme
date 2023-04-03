#include "DevVideoCapture.h"

#include <QTextCodec>
#include <utility>
#include <complex>

DevVideoCapture::DevVideoCapture(QString name, QObject *parent) : DevOnOff(name, parent) {
    modScreen = new int[4];

//    nStnd = ID_PAL;
    for (int i = 0; i < 4; ++i) {
        image.append(new QImage(SCREEN_WIDTH, SCREEN_HEIGHT, QImage::Format_RGB32));
        modScreen[i] = -1;
        bPrev[i] = rec[i] = false;

        struct qim_par par;
        par.nw = SCREEN_WIDTH;
        par.nh = SCREEN_HEIGHT;
        par.nb = image[i]->byteCount()/(SCREEN_WIDTH * SCREEN_HEIGHT);
        par.ngot = 0;
        par.pqi = image[i];
        par.npr = 0;
        parm.append(par);

        ulPitch[i] = image[i]->bytesPerLine();
    }
    setInterfaceCanal(0xE4, 0);
    nCard[0] = 0;
    nCard[1] = 2;
    nCard[2] = 4;
    nCard[3] = 6;

    pqba = new QByteArray(160, 0);

    initSub();
}

DevVideoCapture::~DevVideoCapture(){
//    delete date;
}

void DevVideoCapture::startVideo() {
int nerr;
//    ulFOURCC = mmioFOURCC('R', 'G', 'B', '4');
    for (int i = 0; i < 4; ++i) {

//        nerr = MPG4K_InitCard(nCard[i]);
        if (nerr/* != ID_OK*/) {
            bInit[i] = false;
        } else
            bInit[i] = true;
//        MPG4K_SetInputStandard(nCard[i], nStnd);
//        MPG4K_SetOutputType(nCard[i], 0, ID_AVI_BIT);

        unsigned long ulTotalDataSize=0;
        unsigned long ulFileSize=0;

//        MPG4K_SetMode(nCard[i], ID_PAL);
//        MPG4K_AddPreviewCallback(nCard[i], PreviewCBs, (void *)&parm.at(i));
//        nerr = MPG4K_SetPreviewDestination(nCard[i], 0);
        if(nerr/* != ID_OK*/){

        }
//        if (ulFOURCC != 0)
//            MPG4K_SetPreviewFOURCC(nCard[i], ulFOURCC);
//        if (nPitch != 0) {
//        nerr = MPG4K_SetPreviewPitch(nCard[i],  ulPitch[i]);	//Pitch
        if (nerr/* != ID_OK*/) {

        }
//        }
//        nerr = MPG4K_StartPreview(nCard[i]);
        if (nerr/* != ID_OK*/) {
            bPrev[i] = false;
        } else
            bPrev[i] = true;

//        MPG4K_EnablePreview(nCard[i], ID_DISABLE);

//        nerr = MPG4K_GetPreviewFOURCC(nCard[i]);
        if (nerr/* == ID_ERR_INVALID_CHANNEL*/ || nerr/* == ID_ERR_NODEVICE*/) {

        } else {

        }
//        nerr = MPG4K_GetPreviewDepth(nCard[i]);
        if (nerr/* == ID_ERR_INVALID_CHANNEL*/ || nerr/* == ID_ERR_NODEVICE*/) {

        } else{

        }
//        nerr = MPG4K_GetPreviewPitch(nCard[i]);
        if (nerr/* == ID_ERR_INVALID_CHANNEL*/ || nerr/* == ID_ERR_NODEVICE*/) {

        } else {

        }
        if (!bInit[i] || !bPrev[i]) {
//            pqmba = new QMessageBox(QMessageBox::Warning,tr("Defects"), *qsqmb,
//                                    QMessageBox::Ok | QMessageBox::Default, 0, WST | Qt::WindowTitleHint |
//                                    Qt::Dialog);
//            pqmba->setGeometry(8, 8, CAM_W, 20 * 6);
//            pqmba->open();
//            QTimer::singleShot(NMESSG, this, SLOT(del_pqmba()));
        } else {
            parm[i].ngot=0;
        }

//        nerr = MPG4K_SetOutputType(nCard[i], 0, ID_AVI_BIT);
        if (nerr/* == ID_ERR_NODEVICE*/ || nerr/* == ID_ERR_NOTFOUND*/) {

        } else {

        }
//        MPG4K_SetEncodingMode(nCard[i], 0, ID_VBR);
//        MPG4K_SetQuality(nCard[i], 0, 5);
//        MPG4K_SetBitrate(nCard[i], 0, 1000000);
//        MPG4K_EnableChannel(nCard[i], 0, ID_ENABLE);
//        MPG4K_AddVideoCallback(nCard[i], 0, VideoSizeCB, NULL);
    }
}

void DevVideoCapture::initSub()
{
    for (int i = 0; i < 4; ++i) {
        SubFiles files;
        files.id = i;
        files.timer = new QTimer();
        files.timer->setInterval(1000);//через секунду новые субтитры
        files.strFile = new QFile();
        files.messageFile = new QFile();
        QTextCodec *cod = QTextCodec::codecForName("ISO-8859-1");
        files.strStream = new QTextStream(files.strFile);
        files.strStream->setCodec(cod);
        files.messageStream = new QTextStream(files.messageFile);
        files.messageStream->setCodec(cod);
        files.timeRecord.start();
        files.lastRecord = QTime::currentTime();
        subFiles.insert(std::pair<int, SubFiles>(i, files));

        connect(files.timer, &QTimer::timeout, this, std::bind([this] (int screen) {

            if (!subFiles[screen].timer->isActive()) {
                return;
            }

            subFiles[screen].countFrame += 1;
            QDateTime time = QDateTime::currentDateTime();
            int np, nc, nzm, nhw, nhf;
            np = nc = nzm = 0;
            if (modScreen[screen] >= CAM1) {
                np = (devCam[modScreen[screen]]->getValue(propertyValue::PLACE) - messeg->npp) / 10 + devCam[modScreen[0]]->getAnglStart(PLACE) * 10;
                nc = (devCam[modScreen[screen]]->getValue(propertyValue::COURSE) - messeg->ncp) / 10 + devCam[modScreen[0]]->getAnglStart(COURSE) * 10;
                nzm = devCam[modScreen[screen]]->getValue(propertyValue::SCALE);
            }
//            qDebug() << "initSub" << np << nc << nzm;
            nhw = messeg->nhp / 100;
            nhf = ((messeg->nhp) >= 0 ? (messeg->nhp) : -(messeg->nhp)) % 100;
            std::string comment = string_format("%02d.%02d.%d %02d:%02d:%02d.%03d %d.%02d ",
                                                time.date().day(), time.date().month(),
                                                time.date().year(), time.time().hour(),
                                                time.time().minute(), time.time().second(),
                                                time.time().msec(), nhw, nhf);
            int ncw = nc / 10;
            int ncf = ncf = ((nc) >= 0 ? (nc) : -(nc)) % 10;
            if (ncw == 0 && nc < 0 && ncf > 0) {
                comment.append("-");
            }
            comment.append(string_format("%d.%d ", ncw, ncf));
            int npw = np / 10;
            int npf = ((np) >= 0 ? (np) : -(np)) % 10;
            if (npw == 0 && np < 0 && npf > 0) {
                comment.append("-");
            }
            comment.append(string_format("%d.%d", npw, npf));
            QString nameCam = "0";
            if (modScreen[screen] >= CAM1) {
                nameCam = devCam[modScreen[screen]]->getName();
            }
            QString fullComment = QString("%1 %2 %3 %4 %5")
                    .arg(nameCam)
                    .arg(messeg->longitube)
                    .arg(messeg->latitibe)
                    .arg(QString::fromStdString(comment))
                    .arg(messeg->messeg);

            *pqba = fullComment.toUtf8().data();
//            pqba->truncate(fullComment.size());
//            qDebug() << "fullComment" << fullComment << QString::fromUtf8(pqba->data(), pqba->size());

            QString format("hh:mm:ss.zzz");
            QTime stm, etm;
            stm = subFiles[screen].lastRecord;
            etm = QTime::fromMSecsSinceStartOfDay(subFiles[screen].timeRecord.elapsed());
            QTextStream* strTextStream = subFiles[screen].strStream;
            if (subFiles[screen].countFrame != 1) {
                *strTextStream << "\n";
            }
            *strTextStream << subFiles[screen].countFrame << "\n";
            *strTextStream << stm.toString(format) << " --> " << etm.toString(format) << "\n";
            *strTextStream << pqba->data() /*fullComment*/ << "\n";
            strTextStream->flush();

            int nw = messeg->nhp;
            std::string strGridData = string_format("%d %d %d %d", nw, nc, np, nzm);
            QString strMessage = QString("%1 %2").arg(fullComment).arg(QString::fromStdString(strGridData));
            *pqba = strMessage.toUtf8().data();
//            qDebug() << "strMessage" << strMessage << pqba->data();
//            pqba->truncate(strMessage.size());
            QTextStream* messageTextStream = subFiles[screen].messageStream;
            (*messageTextStream) << pqba->data() /*strMessage*/ << "\n";
            messageTextStream->flush();

            subFiles[screen].lastRecord = etm;
        }, i));
    }
}

//fullFilename без фомрата
void DevVideoCapture::startRecordSub(QString fullFilename, int screen) {
auto it = subFiles.find(screen);
    if(it == subFiles.end()) {
        return;
    }

    if((*it).second.strFile->isOpen()) {
        stopRecordSub(screen);
    }

    SubFiles files = (*it).second;
    files.strFile->setFileName(fullFilename + ".str");
    files.strFile->open(QIODevice::WriteOnly | QIODevice::Text);
    files.messageFile->setFileName(fullFilename + "-message");
    files.messageFile->open(QIODevice::WriteOnly | QIODevice::Text);
    files.timer->start();
    files.timeRecord.restart();
}

void DevVideoCapture::stopRecordSub(int screen)
{
    auto it = subFiles.find(screen);
    if(it == subFiles.end()) {
        return;
    }

    SubFiles files = (*it).second;
    files.strStream->flush();
    files.strFile->flush();
    files.strFile->close();

    files.messageStream->flush();
    files.messageFile->flush();
    files.messageFile->close();

    files.countFrame = 0;
    files.lastRecord = QTime(0, 0, 0, 0);
    files.timer->stop();
}

void DevVideoCapture::setMod(int nMod) {
    mod = nMod;
    emit upDateMod();
}

void DevVideoCapture::setModScreen(QVector<Devices *> dev, int numScr, int newCam) {
int select = -1, nCam, i;
//char *cmd;
//priorityRequest status = priorityRequest::control;
    nCam = modScreen[numScr];
    if (newCam == nCam)
        return;
    if (newCam >= CAM1) {
        for (i = 0; i < NCAM; ++i) {
            if (modScreen[i] == newCam) {
                select = i;
                break;
            }
        }
    }
    if (rec[numScr]) {
        if (modScreen[numScr] < Lor1 && newCam >= Lor1) {
            stopRecCam(numScr);
        } else if (modScreen[numScr] >= Lor1 && newCam < Lor1) {
            startRecCam(numScr);
        }
        emit setRecordingLor(rec[numScr], newCam);
    }
    modScreen[numScr] = newCam;
    if (modScreen[numScr] >= Lor1)
        emit commandNT16(SetNT16, 0, numScr + 1);
//        cmd = createCommand(SetNT16, 0, numScr + 1);
    else
        emit commandNT16(SetNT16, modScreen[numScr] + 1, numScr + 1);
//        cmd = createCommand(SetNT16, modScreen[numScr] + 1, numScr + 1);
//    emit appendCmd(cmd, 6, status);

    if (newCam >= CAM1) {
        dev[newCam]->setShow(true);
    }
    if (nCam >= CAM1) {
        dev[nCam]->setShow(false);
    }
    if (select >= 0) {
        if (rec[select]) {
            if (modScreen[select] < Lor1 && nCam >= Lor1) {
                stopRecCam(select);
            } else if (modScreen[select] >= Lor1 && nCam < Lor1) {
                startRecCam(select);
            }
            emit setRecordingLor(rec[select], nCam);
        }
        if (nCam >= CAM1) {
            dev[nCam]->setShow(true);
            modScreen[select] = nCam;
        } else {
            modScreen[select] = -1;
        }
        emit commandNT16(SetNT16, modScreen[select] + 1, select + 1);
//        cmd = createCommand(SetNT16, modScreen[select] + 1, select + 1);
//        emit appendCmd(cmd, 6, status);
    }
    emit upDateScreen();
}

void DevVideoCapture::setModScreen(int *nModScreen) {
//char *cmd;
//priorityRequest status = priorityRequest::control;
    *modScreen = *nModScreen;
    for (int i = 0; i < 4; ++i) {
        emit commandNT16(SetNT16, modScreen[i] + 1, i + 1);
//        cmd = createCommand(SetNT16, modScreen[i] + 1, i + 1);
//        emit appendCmd(cmd, 6, status);
    }
    emit upDateScreen();
}

void DevVideoCapture::analizCmd(QByteArray *cmd) {
QString error = tr("НТ-16: ");
uchar Ncmd = cmd->at(0),
      byte_2 = cmd->at(1);
    if (byte_2 >= 0xF0) {
        createAnErrorMessage(error, byte_2);
        return;
    }
}

void DevVideoCapture::setOnOff(bool nOn) {
    on = nOn;
}

void DevVideoCapture::setDataMesseg(DataMesseg *messeg) {
    this->messeg = messeg;
}

void DevVideoCapture::setDevCam(QVector<DevCam *> dev) {
    devCam = dev;
}

void DevVideoCapture::getModScreen(int *scr1, int *scr2, int *scr3, int *scr4) {
    *scr1 = modScreen[0];
    *scr2 = modScreen[1];
    *scr3 = modScreen[2];
    *scr4 = modScreen[3];
}

void DevVideoCapture::setDirRec(QString *st) {
    diskPath = st;
}

void DevVideoCapture::startRec(int screen) {
    if (rec[screen])
        return;
    rec[screen] = true;
    if (modScreen[screen] < Lor1)
        startRecCam(screen);
    else
        emit setRecordingLor(rec[screen], modScreen[screen]);
    emit upRec();
}

void DevVideoCapture::startRecCam(int screen) {
//QDateTime qdt;
//QString fullpath;
//listAviFile file;
//    qdt = QDateTime::currentDateTime();
//    QString filename = QString("%1/screen%2/%3").arg(*diskPath).arg(QString::number(screen + 1)).arg(qdt.toString("MM'_'dd'_'yyyy'_'hh'_'mm'_'ss'_'zzz"));
//    fullpath = filename + ".avi";
//    file.name = fullpath;
//    file.start = qdt;
//    newAviFile.append(file);
////    aviFile.append(file);
//    nRec[screen] = newAviFile.size() - 1;
////    nRec[screen] = aviFile.size() - 1;
//    qDebug() << "DevVideoCapture" << fullpath << filename;
//    startRecordSub(filename, screen);
////    MPG4K_SetAVIFilename(nCard[i],  0,  const_cast<char*>(fullpath.toStdString().messeg()));
////    MPG4K_Start(nCard[i]);
}

void DevVideoCapture::stopRec(int screen) {
    if (!rec[screen])
        return;
    rec[screen] = false;
    if (modScreen[screen] < Lor1)
        stopRecCam(screen);
    else
        emit setRecordingLor(rec[screen], modScreen[screen]);
    emit upRec();
}

void DevVideoCapture::stopRecCam(int screen) {
//    newAviFile[nRec[screen]].end = QDateTime::currentDateTime();
////    aviFile[nRec[screen]].end = QDateTime::currentDateTime();
//    aviFile.append(newAviFile[nRec[screen]]);
//    nRec[screen] = - 1;
//    if (!rec[0] && !rec[1] && !rec[2] && !rec[3])
//        newAviFile.clear();
////    MPG4K_StopEncoder(nCard[screen]);
////    MPG4KX_EnableAnalogueOutput(nCard[screen], ID_PREVIEW , ID_DISABLE);
//    stopRecordSub(screen);
}

void DevVideoCapture::resetRec() {
//    for (int i = 0; i < 4; ++i) {
//        if (rec[i])
//            if (modScreen[i] < Lor1) {
//                stopRecCam(i);
//            } else {
//                emit setRecordingLor(false, modScreen[i]);
//            }
//    }
//    newAviFile.clear();
//    for (int i = 0; i < 4; ++i) {
//        if (rec[i])
//            if (modScreen[i] < Lor1) {
//                startRecCam(i);
//            } else {
//                emit setRecordingLor(rec[i], modScreen[i]);
//            }
//    }
}

void DevVideoCapture::addingSound(QString sound) {
//int i, num, n = -1;
//    for (i = 0; i < 3; i++) {
//        if (!bAviThr[i]) {
//            n = i;
//            break;
//        }
//    }
//    addingSoundThread.append(new AddingSoundThread(n, sound, aviFile));
//    aviFile.clear();
//    num = addingSoundThread.size() - 1;
//    QObject::connect(addingSoundThread[num], &AddingSoundThread::addingSound, this, &DevVideoCapture::endThread);
//    if (n < 0)
//        return;
//    bAviThr[n] = true;
//    addingSoundThread[num]->start();
}

void DevVideoCapture::endThread(int n) {
//int i, size, num, num1 = -1;
//    size = addingSoundThread.size();
//    for (i = 0; i < 3; i++) {
//        if (i == size)
//            break;
//        if (n == addingSoundThread[i]->isNumber()) {
//            num = i;
//        }
//    }
//    QObject::disconnect(addingSoundThread[num], &AddingSoundThread::addingSound, this, &DevVideoCapture::endThread);
//    addingSoundThread[num]->quit();
//    addingSoundThread[num]->wait();
//    addingSoundThread.remove(num);
//    bAviThr[n] = false;

//    size = addingSoundThread.size();
//    num = -1;
//    for (i = 0; i < 3; i++) {
//        if (i == size)
//            break;
//        if (addingSoundThread[i]->isNumber() == -1) {
//            num = i;
//        }
//    }
//    if (num < 0)
//        return;
//    for (i = 0; i < 3; i++) {
//        if (!bAviThr[i]) {
//            num1 = i;
//            break;
//        }
//    }
//    if (num1 < 0)
//        return;
//    bAviThr[num1] = true;
//    addingSoundThread[num]->setNumber(num1);
//    addingSoundThread[num]->start();
}

int DevVideoCapture::getMod() {
    return mod;
}

int DevVideoCapture::getModScreen(int numScr) {
    return modScreen[numScr];
}

bool DevVideoCapture::isRec(int scr) {
    return rec[scr];
}

QString DevVideoCapture::getFileName(int nScr) {
    return "";//aviFile[nRec[nScr]].name;
}

QVector<QImage *> DevVideoCapture::getImage() {
    return image;
}

template<typename ... Args>
std::string DevVideoCapture::string_format(const std::string &format, Args ... args)
{
    int size_s = std::snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
    if( size_s <= 0 ){ throw std::runtime_error( "Error during formatting." ); }
    auto size = static_cast<size_t>( size_s );
    std::unique_ptr<char[]> buf( new char[ size ] );
    std::snprintf( buf.get(), size, format.c_str(), args ... );
    return std::string( buf.get(), buf.get() + size - 1 ); // We don't want the '\0' inside
}
