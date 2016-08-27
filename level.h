#pragma once
#include "common.h"
#include "images.h"

struct imgMov{
	int start;
	int end;
	int ticks;
	int tick;
	imgMov(int start, int end, int ticks):start(start), end(end), ticks(ticks), tick(0){}
};

class Level: public QObject{
	Q_OBJECT
public:
	Level();
	~Level();
	bool setup(const char *file);

	void tick();

	int getW(){return w;}
	int getH(){return h;}
	inline int getWidth(){return getW();}
	inline int getHeight(){return getH();}

	inline QImage *getImage(int ID){if(ID >= imgSize)return 0; return images[ID];}

	inline int getMapText(int pos){if(pos > w*h)return 0; return mapText[pos];}  //pos = x+y*w
	const int *getMapText(){return mapText;}

	inline int getMapProp(int pos){if(pos > w*h)return 0; return mapProp[pos];}  //pos = x+y*w
	const int *getMapProp(){return mapProp;}
protected:

	int tileW;
	int tileH;

	int w;
	int h;

	int *mapText;
	int *mapProp;

	int imgSize;
	QImage **images;

	vector <imgMov *> imgMovs;
};
