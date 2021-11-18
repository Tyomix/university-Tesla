#ifndef MAIN_H
#define MAIN_H

extern unsigned char data1[1800*4000];
extern double zoomX;
extern double zoomY;
extern bool done;
extern bool work;
extern bool save;
extern QString devname;

extern void ThreadADC();

#endif // MAIN_H
