#include "campfire.h"
#include "game.h"
#include "player.h"
#include "images.h"

CampFire::CampFire(QVector2D pos):Device(pos){
	tickedN = 0;
}


void CampFire::tick(){
	if(!active)return;
	game->enemyReaction(pos, 1);
	
	QRectF rect = QRectF(pos.x()-w/2, pos.y()-h/2, w, h);
	Player *player = game->isPlayer(rect);
	if(player){
		player->attacked(1);
	}

	Enemy *enemy = game->isEnemy(rect);
	if(enemy){
		enemy->attacked(1);
	}
	tickedN++;
	if(tickedN>=24)tickedN=0;
}
void CampFire::render(QPainter *painter){
	QVector2D screenPos = pos + game->getLevelOffset();

	QImage *img = loadImage("textures/campfire.png");
	painter->drawImage(QRectF(screenPos.x()-w/2, screenPos.y()-h/2, w, h), *img, QRect((tickedN/3)*32,0,32,32));
}
