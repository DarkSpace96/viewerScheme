#ifndef CAM_NUM
#define CAM_NUM

#define OP1_AD "192.168.1.8"
#define OP2_AD "192.168.1.9"
#define OP3_AD "192.168.1.10"

#define serv_port 6000
#define serv_m_port 6064

#define NNH20 10	//h из MI17 в m/NNH20
#define NNHK (NNH20/10)	// 5/10 - 2/1
#define NNHK20 (3-NNHK)		//(20/NNH20)
#define NMULT 100 	//мультипликатор для всех остальных камер

#define NHTMK 1		//tmk номер


#define NGRMAX 256

#define NALPHA 190	//камеры udp снижают градус в градусе/10, 134
#define NBTFL 381	//udp камера полный обзор по вертикали в градусах/10, 267
#define NALFL 288	//udp камера полный обзор по горизонтали в градусах/10, 216

#define CAMERAS 20	//все камеры
#define CAM_IP 8
#define CAM (CAMERAS-CAM_IP)
#define MTVC 4		//максимальное количество отображаемых камер
#define CAM_PB 10 //колличество кнопок вкл КУ

//#define CAM_WIDTH 1920
//#define CAM_HEIGHT 1080
//#define POLI_WIDTH (CAM_WIDTH / 2)
//#define POLI_HEIGHT (CAM_HEIGHT / 2)

#define LOR_WIDTH 704
#define LOR_HEIGHT 576

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define S2_WIDTH (SCREEN_WIDTH / 2)
#define S2_HEIGHT (SCREEN_HEIGHT / 2)

#define OTREZ 165
#define OT3 1000
#define ANCAM2 288

#define NMGX 400	//ширина увеличительного стекла
#define NMGY 320	//ширина увеличительного стекла

#define NCAM 4	// для оператора

#define NOVRL 15 	//камеры udp перекрываются в м/200 //in sm?

#define NX3UDP -15	//настройка X камеры 3 от udp-камер средняя в мм
#define NY3UDP 48	//настройка Y камеры 3 от udp-камер средняя в мм

#define ND32   300		//максимальный угол наклона отличается для камер 3,4 в градусах/10

#define NBRTTD 2000
#define NTOUT 10

#define NZOOM00 0
#define NZOOM01 185
#define NZOOM02 274
#define NZOOM03 318
#define NZOOM04 354
#define NZOOM05	377
#define NZOOM06 412
#define NZOOM07 422
#define NZOOM08 448
#define NZOOM09 460

#define CAM1 0
#define CAM2 1
#define CAM3 2
#define CAM4 3
#define CAM5 4
#define CAM6 5
#define CAM7 6
#define CAM8 7
#define CAM9 8
#define CAM10 9
#define CAM11 10
#define CAM12 11

#define Lor1 12
#define Lor2 13
#define Lor3 14
#define Lor4 15
#define Lor5 16
#define Lor6 17
#define Lor7 18
#define Lor8 19
#define BLe 15
#define BRi 16

#define Port_Base 50608

#define Pot1 0
#define Pot3 1
#define Pot4 2
#define Pot5 3
#define Pot6 4
#define Pot7 5
#define Pot8 6
#define PotLe 7
#define PotRi 8

#define N2D 12

#include <stdint.h>

#endif // CAM_NUM

