#include "level.h"



struct imageData{
	QImage *img;
	int width;
	int height;

	int start;
	int length;
};


Level::Level(){
}


Level::~Level(){
	uint32_t i;
	for(i=0;i<imgSize;++i){
		delete images[i];
	}
	free(images);
	for(i=0;i<imgMovs.size();++i){
		delete imgMovs[i];
	}
	free(mapText);
	free(mapProp);
}


bool Level::setup(const char *file){
	uint32_t i;
	xml_document doc;

	if(!doc.load_file(file)){
		fprintf(stderr, "Error when loading level file!(%s)\n", file);
		return 1;
	}

	int dirFilePos = 0;
	i=0;
	while(file[i]){
		if(file[i]=='/' || file[i]=='\\')dirFilePos = i;
		i++;
	}
	QString dirFile;
	if(dirFilePos)dirFile = QString(file).mid(0,dirFilePos+1);
	else dirFile = "";

	xml_node mapN = doc.child("map");
	xml_attribute widthN = mapN.attribute("width");
	xml_attribute heightN = mapN.attribute("height");

	if(!widthN || !heightN) return 1;

	w = widthN.as_int();
	h = heightN.as_int();




	xml_attribute tilewidthN = mapN.attribute("tilewidth");
	xml_attribute tileheightN = mapN.attribute("tileheight");

	tileW = 32;
	tileH = 32;

	if(tilewidthN)tileW = tilewidthN.as_int();
	if(tileheightN)tileH = tileheightN.as_int();



	vector<imageData *> imgData;
	for(xml_node tilesetN: mapN.children("tileset")){
		imageData *imgD = new imageData();

		xml_node imgN = tilesetN.child("image");

		QString file = QString(imgN.attribute("source").as_string());
		file = dirFile + file;
		QImage *img = loadImage(file);

		imgD->img = img;
		imgD->width = imgN.attribute("width").as_int();
		imgD->height = imgN.attribute("height").as_int();

		imgD->start = tilesetN.attribute("firstgid").as_int();
		imgD->length = tilesetN.attribute("tilecount").as_int();

		imgData.push_back(imgD);

		imgSize = imgD->start+imgD->length;


		xml_node propsN = tilesetN.child("properties");
		if(propsN){
			for(xml_node propN: propsN.children("property")){
				const char *name = propN.attribute("name").as_string();

				if(!strcmp(name, "ticks")){
					imgMov *mov = new imgMov(imgD->start, imgD->start + imgD->length, propN.attribute("value").as_int());
					imgMovs.push_back(mov);
				}
			}
		}
	}

	images = (QImage **)malloc(sizeof(QImage *) * imgSize);
	mapText = (int *)malloc(sizeof(int) * w * h);
	mapProp = (int *)malloc(sizeof(int) * w * h);

	int actImgID = 0;
	int actImgPart = 0;
	int widImg;
	images[0] = new QImage();
	for(i=1;i<imgSize;++i){
		widImg = imgData[actImgID]->width/tileW;
		QRect rect((actImgPart%widImg)*tileW, (actImgPart/widImg)*tileH, tileW, tileH);


		images[i] = new QImage(imgData[actImgID]->img->copy(rect));

		actImgPart++;
		if(actImgPart == imgData[actImgID]->length){
			actImgPart = 0;
			actImgID++;
		}
	}



	int temp;
	for(xml_node layerN: mapN.children("layer")){
		xml_node dataN = layerN.child("data");

		const char *dataStr = dataN.text().as_string();
		const char *layerType = layerN.attribute("name").as_string();

		char *TEMP = (char*)malloc((strlen(dataStr)+1)*sizeof(char));
		strcpy(TEMP, dataStr);
		char *actData = strtok(TEMP, ",\n\r");
		int actPos = 0;

		if(!strcmp(layerType, "Properties")){
			while(actData){
				strToInt(actData, temp);
				mapProp[actPos++] = temp;
				actData = strtok(0, ",\n\r");
			}
		}else if(!strcmp(layerType, "Textures")){
			while(actData){
				strToInt(actData, temp);
				mapText[actPos++] = temp;
				actData = strtok(0, ",\n\r");
			}
		}
		free(TEMP);
	}







	for(i=0;i<imgData.size();++i){
		delete imgData[i];
	}

	return 0;
}

bool Level::setup(int w, int h){
	uint32_t i;

	this->w = w;
	this->h = h;

	tileW = 32;
	tileH = 32;



	vector<imageData *> imgData;





	imageData *imgD;
	QImage *img;
	imgMov *mov;

	imgD = new imageData();
	img = loadImage("./textures/font,png");

	imgD->img = img;
	imgD->width = 512;
	imgD->height = 512;

	imgD->start = 1;
	imgD->length = 256;
	imgData.push_back(imgD);
	imgSize = imgD->start+imgD->length;


	imgD = new imageData();
	img = loadImage("./textures/textures.png");

	imgD->img = img;
	imgD->width = 512;
	imgD->height = 512;

	imgD->start = 257;
	imgD->length = 256;
	imgData.push_back(imgD);
	imgSize = imgD->start+imgD->length;


	imgD = new imageData();
	img = loadImage("./textures/water.png");

	imgD->img = img;
	imgD->width = 256;
	imgD->height = 32;

	imgD->start = 513;
	imgD->length = 8;
	imgData.push_back(imgD);
	imgSize = imgD->start+imgD->length;

	mov = new imgMov(imgD->start, imgD->start + imgD->length, 8);
	imgMovs.push_back(mov);




	images = (QImage **)malloc(sizeof(QImage *) * imgSize);
	mapText = (int *)malloc(sizeof(int) * w * h);
	mapProp = (int *)malloc(sizeof(int) * w * h);

	int actImgID = 0;
	int actImgPart = 0;
	int widImg;
	images[0] = new QImage();
	for(i=1;i<imgSize;++i){
		widImg = imgData[actImgID]->width/tileW;
		QRect rect((actImgPart%widImg)*tileW, (actImgPart/widImg)*tileH, tileW, tileH);


		images[i] = new QImage(imgData[actImgID]->img->copy(rect));

		actImgPart++;
		if(actImgPart == imgData[actImgID]->length){
			actImgPart = 0;
			actImgID++;
		}
	}

	int x,y;
	for(x=0;x<w;++x){
		for(y=0;y<h;++y){
			mapText[x+y*w] = 257;
			mapProp[x+y*w] = 0;
		}
	}

	generateTerrain();
	





	for(i=0;i<imgData.size();++i){
		delete imgData[i];
	}

	return 0;
}

void Level::generateTerrain(){
	int temp;
	uint32_t i;
	QPoint startPs;

	temp = getRandInt(1, h-1);
	startPs = QPoint(0,temp);
	mapProp[(temp+1)*w] = 1;
	mapText[(temp+1)*w] = 259;
	mapProp[temp*w] = 3;
	mapProp[(temp-1)*w] = 1;
	mapText[(temp-1)*w] = 259;


	temp = getRandInt(1, h-1);
	mapProp[(temp+1)*w+w-1] = 1;
	mapText[(temp+1)*w+w-1] = 259;
	mapProp[temp*w+w-1] = 4;
	mapProp[(temp-1)*w+w-1] = 1;
	mapText[(temp-1)*w+w-1] = 259;


	int x,y;
	for(i=0;i<2 * w*h /1024;++i){
		x = getRandInt(0,w);
		y = getRandInt(0,h);

		generateWater(QPoint(x,y), 3);
	}

	for(i=0;i<32 * w*h /1024;++i){
		x = getRandInt(0,w);
		y = getRandInt(0,h);

		generateRock(QPoint(x,y), getRandDouble()*1.3);
	}

	for(i=0;i<120 * w*h /1024;++i){
		x = getRandInt(0,w);
		y = getRandInt(0,h);

		generateBush(QPoint(x,y), getRandDouble()*0.6f);
	}

	mapTemp = (int *)malloc(sizeof(int)*w*h);

	for(i=0;i<w*h;++i)mapTemp[i]=0;

	checkPath(startPs);

	for(i=0;i<w*h;++i){
		if(mapTemp[i]==0){
			if(mapProp[i]!=1){
				mapProp[i] = 1;
				mapText[i] = 259;
			}
		}
	}

	free(mapTemp);
}

void Level::checkPath(QPoint pos){
	int x = pos.x();
	int y = pos.y();

	if(mapTemp[x + y * w]!=0)return;
	if(mapProp[x + y * w]==1)return;
	mapTemp[x+y*w] = 1;

	if(x)checkPath(pos+QPoint(-1,0));
	if(y)checkPath(pos+QPoint(0,-1));
	if(x!=w-1)checkPath(pos+QPoint(1,0));
	if(y!=h-1)checkPath(pos+QPoint(0,1));
}

void Level::generateRock(QPoint pos, float prob){
	int x = pos.x();
	int y = pos.y();
	if(mapProp[x + y * w]!=0)return;

	if(x!=1 && y!=1 && x!=w-2 && y!=h-2){
		mapProp[x+y*w] = 1;
		mapText[x+y*w] = 259;
	}
	if(getRandDouble() < prob && x != 0 && (y!=1 || x==1) && (y!=h-2 || x==1))generateRock(pos+QPoint(-1,0), prob*(1-fabs(getRandDouble2())));
	if(getRandDouble() < prob && y != 0 && (x!=1 || y==1) && (x!=w-2 || y==1))generateRock(pos+QPoint(0,-1), prob*(1-fabs(getRandDouble2())));
	if(getRandDouble() < prob && x != w-1 && (y!=h-2 || x==w-2) && (y==1 || x==w-2))generateRock(pos+QPoint(1,0), prob*(1-fabs(getRandDouble2())));
	if(getRandDouble() < prob && y != h-1 && (x!=w-2 || y==h-2) && (x==1 || y==h-2))generateRock(pos+QPoint(0,1), prob*(1-fabs(getRandDouble2())));
}

void Level::generateBush(QPoint pos, float prob){
	int x = pos.x();
	int y = pos.y();
	if(mapProp[x + y * w]!=0)return;

	mapProp[x+y*w] = 2;
	mapText[x+y*w] = 258;
	if(getRandDouble() < prob && x != 0)generateBush(pos+QPoint(-1,0), prob*(1-fabs(getRandDouble2())));
	if(getRandDouble() < prob && y != 0)generateBush(pos+QPoint(0,-1), prob*(1-fabs(getRandDouble2())));
	if(getRandDouble() < prob && x != w-1)generateBush(pos+QPoint(1,0), prob*(1-fabs(getRandDouble2())));
	if(getRandDouble() < prob && y != h-1)generateBush(pos+QPoint(0,1), prob*(1-fabs(getRandDouble2())));
}

void Level::generateWater(QPoint pos, float prob){
	int x = pos.x();
	int y = pos.y();
	if(mapProp[x + y * w]!=0)return;

	mapProp[x+y*w] = 2;
	mapText[x+y*w] = 513;
	if(getRandDouble() < prob && x != 0)generateWater(pos+QPoint(-1,0), prob*(1-fabs(getRandDouble2())));
	if(getRandDouble() < prob && y != 0)generateWater(pos+QPoint(0,-1), prob*(1-fabs(getRandDouble2())));
	if(getRandDouble() < prob && x != w-1)generateWater(pos+QPoint(1,0), prob*(1-fabs(getRandDouble2())));
	if(getRandDouble() < prob && y != h-1)generateWater(pos+QPoint(0,1), prob*(1-fabs(getRandDouble2())));
}

void Level::tick(){
	uint32_t i, j;
for(i=0;i<imgMovs.size();++i){
		imgMovs[i]->tick++;
		if(imgMovs[i]->tick >= imgMovs[i]->ticks){
			imgMovs[i]->tick -= imgMovs[i]->ticks;
			j=imgMovs[i]->start;
			QImage *temp = images[j];
			for(++j;j<imgMovs[i]->end;++j){
				images[j-1] = images[j];
			}
			images[j-1] = temp;
		}
	}
}
