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
	bool setup(int w, int h);

	void tick();

	int getW(){return w;}
	int getH(){return h;}
	inline int getWidth(){return getW();}
	inline int getHeight(){return getH();}

	inline QImage *getImage(int ID){if(ID >= imgSize)return 0; return images[ID];}

	inline int getMapText(int pos){if(pos > w*h)return 0; return mapText[pos];}  //pos = x+y*w
	const int *getMapText(){return mapText;}

	inline int getMapProp(int pos){if(pos > w*h || pos < 0)return 1; return mapProp[pos];}  //pos = x+y*w
	inline int getMapProp(int x, int y){if(x<0 || y < 0 || x>=w || y >= h)return 1; return mapProp[x+y*w];}
	const int *getMapProp(){return mapProp;}
protected:

	void generateTerrain();
	void checkPath(QPoint);
	void generateRock(QPoint pos, float prob);
	void generateBush(QPoint pos, float prob);
	void generateWater(QPoint pos, float prob);

	int tileW;
	int tileH;

	int w;
	int h;


	int *mapTemp;

	int *mapText;
	int *mapProp;

	int imgSize;
	QImage **images;

	vector <imgMov *> imgMovs;
};
