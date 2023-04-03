#include "DevLor.h"

DevLor::DevLor(QString name, QObject *parent) : DevOnOff(name, parent) {
    image = new QImage(LOR_WIDTH, LOR_HEIGHT, QImage::Format_RGB32);
    ifd_qfm = -1;
    pbmp = image->bits();
    buf0[0] = 10;
    buf0[1] = buf0[2] = buf0[3] = 0;

    pqba = new QByteArray(160, 0);

    size = 1;
    data = new double[size];

    namePropertys = new QString[size];
    namePropertys[0] = "Пинг, мс";

    colorValues = new QColor[size];
    colorValues[0] = Qt::white;

    setSizeState();
}

DevLor::~DevLor() {

}

/**
 * @brief DevLor::writeMark - запись комментария для кадра при записи
 * @param qstb - (QString *)
 * @param nr - (int *)
 * @return - (bool)
 */
bool DevLor::writeMark(QString *qstb, int *nr) {
int n;
bool b = true;
    *pqba = qstb->toUtf8().data();
    pqba->truncate(128);
    pqba->replace(127, 1, buf0, 1);
    n = (int) qfmarks.write(*pqba);
    if (n > 0)
        *nr = n;
    else {
        b = 0;
        *nr = qfmarks.error();
    }
    return b;
}

void DevLor::setOnOff(bool nOn) {
    on = nOn;
    if (!on) {
        bShow = signal = connect = recording = false;
    }
    emit startTcpCam(getNumber(), on);
}

/**
 * @brief DevLor::setRecording - запустить/прекратить запись с КУ
 * @param b - (bool)
 */
void DevLor::setRecording(bool b, int num) {
char buf[80];
//struct tms tm1;
int m, n;
QString qtm("  ");
QDateTime qdt;
QDir qdr(*mainDir);
QFile qf;
    if (b && num == getNumber()) {
        if (!recording) { // начать запись
            qdt = QDateTime::currentDateTime();
            nfl = 0;
            n = number;
            sprintf(buf, "L%d_", n - Lor1 + 1);


            qtm = QString(buf) + qdt.toString("MM'_'dd'_'yyyy'_'hh'_'mm'_'ss'_'zzz");
            if (qdr.mkdir(qtm)) {
                fullDir = *mainDir + QString("/") + qtm;
                qdr.mkdir(fullDir + QString("-mp4"));
                qf.setFileName(fullDir + QString("/parameters"));
                if (qf.open(QIODevice::ReadWrite)) {
                    sprintf(buf,
                            "%ld time\n%d fps\n%d width\n%d height\n%d camera\n",
                            0/*times(&tm1)*/, 20, LOR_WIDTH, LOR_HEIGHT, number);
                    m = qf.write(buf);
                    fprintf(stdout, "camera %d %s to %s\n", number,
                            ipAddress.toLatin1().data(),
                            fullDir.toLatin1().data());
                    fflush(stdout);
                    recording = true;
                    qf.close();
                } else {
                    fprintf(stdout,
                            "camera %d, parameters file was not written.\n",
                            number);
                    fflush(stdout);
                }
                qfmarks.setFileName(fullDir + QString("/marks"));
                if (qfmarks.open
                    (QIODevice::WriteOnly | QIODevice::
                     Truncate | QIODevice::Append | QIODevice::
                     Text | QIODevice::Unbuffered)) {
                    ifd_qfm = qfmarks.handle();
                } else {
                    fprintf(stdout, "camera %d, marks file was not opened.\n",
                            number);
                    fflush(stdout);
                }
            } else {
                fprintf(stdout,
                        "camera %d, output directory was not created.\n",
                        number);
                fflush(stdout);
            }
        }
    } else { // остановить запись
        recording = false;
        if (ifd_qfm > 0)
            qfmarks.close();
        ifd_qfm = -1;
    }
}

/**
 * @brief DevLor::setMainDir
 * @param qs - (QString *)
 */
void DevLor::setMainDir(QString *qs) {
    mainDir = qs;
}

void DevLor::setConnection(bool con) {
    connect = con;
}

void DevLor::setNumber(int nNumber) {
    number = nNumber;

}

void DevLor::setIpAddress(QString nIpAddress) {
    ipAddress = nIpAddress;
}

void DevLor::setDataMesseg(DataMesseg *messeg) {
    this->messeg = messeg;
}

void DevLor::incNfl() {
    nfl++;
}

void DevLor::paintOn() {
    paint = true;
}

void DevLor::setData(const propertyDate property, const double value) {
    if (property != PING)
        return;

    int dataI = value * 100;
    data[0] = dataI / 100.;
}

void DevLor::setColorItem(const propertyDate &property, const QColor &color, const int n) {
    if (property != PING)
        return;
    colorValues[0] = color;
}

Color DevLor::getStateColor(int n) {
QColor colorPing = colorValues[0];
Color colorState;
    if (colorPing == Qt::red ) {
        colorState.color = QColor(Qt::red);
        colorState.raiting = GREEN;//RED;
        return colorState;
    }
    colorState.color = QColor(Qt::green);
    colorState.raiting = GREEN;
    return colorState;
}


int DevLor::getNumber() {
    return number;
}

int DevLor::getNfl() {
    return nfl;
}

int DevLor::getIfd() {
    return ifd_qfm;
}


bool DevLor::isRecording() {
    return recording;
}

bool DevLor::isPaint() {
bool b = paint;
    paint = false;
    return b;
}

QString DevLor::getIpAddress() {
    return ipAddress;
}

QString DevLor::getFullDir() {
    return fullDir;
}

QString DevLor::getFileName() {
char buft[30];
    sprintf(buft, "/%010d.jpg", getNfl());
    return getFullDir() + QString(buft);
}

QImage *DevLor::getImage() {
    return image;
}

unsigned char *DevLor::getPbmp() {
    return pbmp;
}
