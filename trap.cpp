#include "trap.h"
#include "images.h"
#include "game.h"

Trap::Trap(QVector2D pos):Device(pos){

}


void Trap::tick(){
	if(!active)return;

	QRectF rect = QRectF(pos.x()-w/2, pos.y()-h/2, w, h);
	Player *player = game->isPlayer(rect);
	if(player){
		game->removeDevice(this);
		delete this;
	}

	Enemy *enemy = game->isEnemy(rect);
	if(enemy){
		enemy->attacked(100000);
		game->removeDevice(this);
		delete this;
	}
}

void Trap::render(QPainter *painter){
	QVector2D screenPos = pos + game->getLevelOffset();

	QImage *img;
	img = loadImage("textures/trap.png");
	painter->drawImage(QRectF(screenPos.x()-w/2, screenPos.y()-h/2, w, h), *img);
}
