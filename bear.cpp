#include "bear.h"
#include "player.h"
#include "game.h"
#include "images.h"

Bear::Bear(QVector2D pos): Enemy(pos){
	foodValue = 80;
	HP = 10;
	w = 32;
	h = 32;
	halfSZ = (w+h)/4;
}

void Bear::reaction(QVector2D pos, int type){
	if(myReaction)return;

	if(HP < 1 || type == 1){
		QVector2D diff = pos-this->pos;

		QVector2D diffN = diff.normalized();
		if(diff.lengthSquared() < (16*4096) && move.lengthSquared() < diffN.lengthSquared()*3.f){

			move = diffN*-2.f;
		}
	}else attack(pos);
}

void Bear::attack(QVector2D pos){
	if(attackDelay){
		attackDelay--;
		return;
	}

	QVector2D diff = pos-this->pos;

	QVector2D diffN = diff.normalized();
	if(diff.lengthSquared() < (16*4096) && move.lengthSquared() < diffN.lengthSquared()*2.f){

		move = diffN*1.8f;
	}





	int attackForce = 4;

	QVector2D attackPos = QVector2D(lastMove.x(), lastMove.y());
	attackPos.normalize();
	attackPos *= halfSZ*3/2;

	QVector2D attPs = this->pos + attackPos;

	Player *enemy = game->isPlayer(QRectF(attPs.x()-halfSZ/2, attPs.y()-halfSZ/2, halfSZ, halfSZ));

	if(enemy){
		enemy->attacked(attackForce);
		attackDelay = 400;
	}
}

void Bear::crashed(){
	looped++;
	if(looped > 50){
		looped = 0;
		return;
	}
	float angle = (getRandDouble()*2-1) * 0.8f;
	move = QVector2D(cos(angle)*move.x() - sin(angle)*move.y(), sin(angle)*move.x() + cos(angle)*move.y());
	tickMove();
}

void Bear::tick(){
	if(!active)return;
	looped = 0;
	tickMove();
	move *= 0.997f;

	Player *player = game->getPlayer();

	QVector2D diff = player->pos-pos;
	float diffF = diff.lengthSquared();
	if(diffF < 36*36){
		reaction(player->pos);
	}else if(diffF < 130*130){
		QVector2D diffN = diff.normalized();
		QVector2D moveN = lastMove.normalized();
		if((diffN-moveN).lengthSquared() < 1.7)reaction(player->pos);
	}else if(move.lengthSquared()<0.2f){
		if(!getRandInt(150))move = QVector2D(getRandDouble2(), getRandDouble2());
		else move = QVector2D(0, 0);
	}
}

void Bear::render(QPainter *painter){
	QVector2D screenPos = pos + game->getLevelOffset();

	float angle = atan2(lastMove.y(), lastMove.x())-M_PI/2;

	painter->translate(QPointF(screenPos.x(), screenPos.y()));
	painter->rotate(angle*180/M_PI);
	

	QImage *bodyImg = loadImage("textures/bear_body.png");
	painter->drawImage(QRectF(-w/2, -h/2, w, h), *bodyImg);
	painter->resetTransform();

	QImage *headImg = loadImage("textures/bear_head.png");
	QVector2D relPos = QVector2D(-sin(angle)*h/2, cos(angle)*h/2);
	painter->drawImage(QRectF(screenPos.x()+relPos.x()-w/4, screenPos.y()+relPos.y()-h/4, halfSZ, halfSZ), *headImg);
}
