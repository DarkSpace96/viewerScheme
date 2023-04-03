#ifndef ZENIT_H
#define ZENIT_H

#define NWAIT 1200	//Время ожидания в мс для "Зенит"
#define RT_ADDR 6 /*RT Address*/ /* адрес ОУ */
#define NAN_PREC 5	//малый вес бита, умноженный на 10000
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <stdio.h>

//#ifdef Q_OS_UNIX
#include "ltmk_m-cpp.h"
//#endif

#endif // ZENIT_H
