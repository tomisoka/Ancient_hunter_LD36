#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <map>
#include <string.h>
#include <string>


#include <pugixml.hpp>
using namespace pugi;


#include <QtGui>
#include <QtMultimedia>
#include <QtCore>

#include <QtAV>

#define APPNAME "LD36 - APP"

#define TPS 60
#define TICK_TIME 1./TPS
#define FONT_SIZE 24

using namespace std;

extern QFont font;
extern QFontMetrics fontMet;



extern QGuiApplication *app;



inline uint32_t getRandInt(uint32_t n){return rand()%n;}
inline uint32_t getRandInt(uint32_t start, uint32_t end){return start+rand()%(end-start);}
inline double getRandDouble(){return rand()/float(RAND_MAX);}
inline double getRandDouble2(){return getRandDouble()-getRandDouble();}

bool strToDouble(const char * pos, double& returned, uint32_t len=100000);
bool strToInt(const char *pos, int& returned, uint32_t len=100000, int32_t base=10);

char *numStr(char *s, uint32_t num);
char *doubleStr(char *s, double n, uint32_t afterpoint = 3);


void setup(int argc, char **argv);
void cleanup();
