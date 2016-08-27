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
	delete images;
	for(i=0;i<imgMovs.size();++i){
		delete imgMovs[i];
	}
	delete mapText;
	delete mapProp;
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
