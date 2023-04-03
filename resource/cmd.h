#ifndef CMD_H
#define CMD_H

#define DevOn 0x05
#define DevOff 0x06
#define GetDev 0x22

// NT-16
#define SetNT16 0x01

// Cam
#define getCam 0x20

//#define UpCam 0xA0
//#define DownCam 0x50

#define UpCam 0x50
#define DownCam 0xA0

#define Diaphragm 0x01

#define GetFocus 0x07
//#define newFocus 0x02

#define Scale 0x03
#define GetScale 0x04

#define Place 0x1D
#define SetPlace 0x0E
#define GetPlace 0x12

#define Course 0x1C
#define SetCourse 0x0D
#define GetCourse 0x11

// PSP
#define SetPSP 0x01
#define GetPSP 0x06

// NT-102
#define GetNT102 0x06


#endif // CMD_H
