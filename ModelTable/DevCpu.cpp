#include "DevCpu.h"

DevCPU::DevCPU(QString name, QObject *parent) : DevSensor(name, parent) {
    size = 1;
    date = new double[size];
    namePropertys = new QString[size];

    namePropertys[0] = "Температура, ℃";
    timer = new QTimer();
    timer->setInterval(60 * 1000);
#ifdef Q_OS_UNIX
    timer->start();
    timeout();
#endif


    QObject::connect(timer, &QTimer::timeout, this, &DevCPU::timeout);
}

void DevCPU::setDate(const propertyDate property, const double nDate) {
int nDateI = nDate * 100;
    if (property != TEMPERATURE)
        return;
    date[0] = nDateI / 100.;

//        colorValues[0] = getColorTemp(value);

}

double DevCPU::getDate(const propertyDate property) {
    if (property != TEMPERATURE)
        return NON_VALUE;
    return date[0];
}

void DevCPU::timeout() {
    setDate(TEMPERATURE, tempMax());
}

int DevCPU::tempMax() {
QFile qf;
int i, j, n1, ntm, nt1, ntemp[16];
char buft[100];
#ifdef Q_OS_UNIX
    strcpy(buft, "/sys/devices/platform/coretemp.0/hwmon/hwmon1/temp1_input");	//Astra 1.6
#endif
    nt1 = ntm = 0;
    qf.setFileName(QString(buft));
    if (qf.exists()) {
        if (qf.open(QIODevice::ReadOnly | QIODevice::Text)) {
            j = qf.readLine(buft + 60, 20);
            if (j > 0) {
                sscanf(buft + 60, "%d", &n1);
                n1 /= 100;
                if (n1 > ntm)
                    ntm = n1;
                ntemp[nt1] = n1;
                nt1++;
            }
        }
        qf.close();
    }
    buft[0] = 0;
    for (i = 0; i < nt1; i++)
        sprintf(buft + strlen(buft), "%d.%d ", ntemp[i] / 10, ntemp[i] % 10);
    fprintf(stdout, "temperatures: %s\n", buft);
    fflush(stdout);
    return ntm;
}
