#include "images.h"

map<QString, QImage *> images;

QImage *loadImage(QString name){
	map<QString, QImage *>::iterator it = images.find(name);
	if(it!=images.end()){
		return it->second;
	}

	QImage *image = new QImage(name);
	if(!image->isNull()){
		images[name] = image;
	}
	return image;
}

void cleanup_images(){
	map<QString, QImage *>::iterator it = images.begin();
	while (it != images.end()){
    delete it->second;
		it++;
	}
	images.clear();
}
