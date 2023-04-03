#include "DevDisk.h"

DevDisk::DevDisk(QString name, QString path, QObject *parent) : Devices(name, parent) {
    this->path = QDir::homePath() + QString("/") + path;
    size = 1;
    data = new double[1];
    namePropertys = new QString[size];
    namePropertys[0] = "Дисковое пронстранство, %";

    colorValues = new QColor[size];
    colorValues[0] = Qt::white;
    setSizeState();
}

/**
 * @brief DevDisk::exists - проверка наличия дискового носителя
 * @return - (bool)
 */
bool DevDisk::exists()  {
    return QFile::exists(path);
}

/**
 * @brief DevDisk::getPath - возвращение каталога, куда подключён дисковый носитель
 * @return - (QString)
 */
QString DevDisk::getPath() {
    return path;
}

/**
 * @brief DevDisk::getValue - получение данных по размеру диска и его заполнения
 * @param maxValue - размер диска (int &)
 * @param value - заполнение диска (int &)
 * @param hours - заполнение в часах диска (int &)
 * @param mins - заполнение в минутах диска (int &)
 */
void DevDisk::getValue(int &maxValue, int &value, int &hours, int &mins)  {
    maxValue = this->maxValue;
    value = this->value;
    hours = this->hours;
    mins = this->mins;
}

void DevDisk::clearData() {
    for (int i = 0; i < size; ++i) {
        data[i] = NON_VALUE;
    }
}

void DevDisk::setData(const propertyDate property, const double value) {
    if (property != DISK_SPACE)
        return;
    int valuesI = value * 100;
    data[0] = valuesI / 100.;
}

/**
 * @brief DevDisk::calculateDiskSpace - проверка дискового пространства
 */
void DevDisk::calculateDiskSpace() {
char buft[5000];
int nfrp, nfra, nfrb;
    sprintf(buft + 1024, "%s", path.toLatin1().data());
    getFree(buft + 1024, &nfrp, &nfra, &nfrb);

    (int&)maxValue = nfrp;
    (int&)value = nfra;
    (int&)hours = nfra / ((299515248.64/nfrb) * 60);
    (int&)mins = nfra / (299515248.64/nfrb) - hours * 60;
}

/**
 * @brief DevDisk::isFillDisk - проверка на полное заполнение диска
 * @return - (bool)
 */
bool DevDisk::isFillDisk() {
int maxProcentValue, currentValue;
    this->calculateDiskSpace();
    maxProcentValue = (maxValue / 100) * 95;//95%
    currentValue = maxValue - value;
    if (currentValue > maxProcentValue)
        return true;
    return false;
}

/**
 * @brief DevDisk::setNextDisk - назначение следующего диска
 * @param disk - (DevDisk *)
 */
void DevDisk::setNextDisk(DevDisk *disk) {
    nextDisk = disk;
}

/**
 * @brief DevDisk::checkDisk - проверка диска
 * @return - (bool)
 */
bool DevDisk::checkDisk() {
    if (!exists()) {
        (QString&) error = QString("Накопитель %1 не найден").arg(getName());
        return false;
    }
    if (isFillDisk()) {
        (QString&) error = QString("Накопитель %1 переполнен").arg(getName());
        return false;
    }
    (QString&) error = QString();
    return true;
}

/**
 * @brief getFree - получение свободного место на диске для записи
 * @param flpth - должна быть именем любого файла на этом диске (const char *)
 * @param npr - (int *)
 * @param nab - (int *)
 * @param nbb - (int *)
 */
void DevDisk::getFree(const char *flpth, int *npr, int *nab, int *nbb) {
int nfrp, nfra, nfrb;
    getFreeR(flpth, &nfrp, &nfra, &nfrb);
    *npr = nfrp;
    *nab = nfra;
    *nbb = nfrb;
}

/**
 * @brief getFreeR - получение свободного место на диске для записи
 * @param flpth - должна быть именем любого файла на этом диске (const char *)
 * @param npr - (int *)
 * @param nab - (int *)
 * @param nbb - (int *)
 * @return - в случае успеха вернёт 0 (int)
 */
int DevDisk::getFreeR(const char *flpth, int *npr, int *nab, int *nbb) {
unsigned long long  n2, n3;
struct stat st;
int n = 0, n4, nfrp, nfra, nfrb;
#ifdef Q_OS_UNIX
struct statvfs stfs;
    n = statvfs(flpth, &stfs);
#endif
    if (n != 0) {
        *npr = *nab = 0;
        return -1;
    }
#ifdef Q_OS_UNIX
    n = stat(flpth, &st);
    n2 = stfs.f_blocks;
    n3 = stfs.f_bfree;
    n4 = st.st_blksize;
#endif
    *npr = nfrp = (int)(n2);
    *nab = nfra = (int)(n3);
    *nbb = nfrb = (int)(n4);
#ifdef Q_OS_UNIX
//    fprintf(stdout, "getFree: %d %lu %llu %llu  %lu %d %d\n", n4, stfs.f_bfree, n3,
//            n2, stfs.f_frsize, nfrp, nfra);
#endif

    return 0;
}

/**
 * @brief DevDisk::getStringError - возвращение строки с ошибкой
 * @return - (QString)
 */
QString DevDisk::getStringError() {
    return error;
}

/**
 * @brief DevDisk::getNextDisk - возвращение следующего диска
 * @return - (DevDisk *)
 */
DevDisk *DevDisk::getNextDisk() {
    return nextDisk;
}

void DevDisk::setColorItem(const propertyDate &property, const QColor &color, int) {
    if (property != DISK_SPACE)
        return;
    colorValues[0] = color;
}

Color DevDisk::getStateColor(int) {
QColor colorSpace = colorValues[0];
Color colorState;
    if (colorSpace == Qt::red) {
        colorState.color = QColor(Qt::red);
        colorState.raiting = GREEN;;
        return colorState;
    }
    colorState.color = QColor(Qt::green);
    colorState.raiting = GREEN;
    return colorState;
}

double DevDisk::getData(const propertyDate property) {
    if (property != DISK_SPACE)
        return NON_VALUE;
    return data[0];
}
