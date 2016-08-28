#include "wolf.h"
#include "player.h"
#include "game.h"
#include "images.h"

Wolf::Wolf(QVector2D pos): Enemy(pos){
	foodValue = 20;
	HP = 3;
	w = 32;
	h = 32;
	halfSZ = (w+h)/4;
}

void Wolf::reaction(QVector2D pos, int type){
	if(myReaction)return;

	if(HP < 2 || type == 1){
		QVector2D diff = pos-this->pos;

		QVector2D diffN = diff.normalized();
		if(diff.lengthSquared() < (25*4096) && move.lengthSquared() < diffN.lengthSquared()*14.5f){

			move = diffN*-4.f;
		}
	}else attack(pos);
}

void Wolf::attack(QVector2D pos){
	if(attackDelay){
		attackDelay--;
		return;
	}

	QVector2D diff = pos-this->pos;

	QVector2D diffN = diff.normalized();
	if(diff.lengthSquared() < (25*4096) && move.lengthSquared() < diffN.lengthSquared()*15.f){

		move = diffN*4.f;
	}





	int attackForce = 1;

	QVector2D attackPos = QVector2D(lastMove.x(), lastMove.y());
	attackPos.normalize();
	attackPos *= halfSZ*3/2;

	QVector2D attPs = this->pos + attackPos;

	Player *enemy = game->isPlayer(QRectF(attPs.x()-halfSZ/2, attPs.y()-halfSZ/2, halfSZ, halfSZ));

	if(enemy){
		enemy->attacked(attackForce);
		attackDelay = 200;
	}
}

void Wolf::crashed(){
	looped++;
	if(looped > 50){
		looped = 0;
		return;
	}
	float angle = (getRandDouble()*2-1) * 1;
	move = QVector2D(cos(angle)*move.x() - sin(angle)*move.y(), sin(angle)*move.x() + cos(angle)*move.y());
	tickMove();
}

void Wolf::tick(){
	if(!active)return;
	looped = 0;
	tickMove();
	move *= 0.997f;

	Player *player = game->getPlayer();

	QVector2D diff = player->pos-pos;
	float diffF = diff.lengthSquared();
	if(diffF < 36*36){
		reaction(player->pos);
	}else if(diffF < 150*150){
		QVector2D diffN = diff.normalized();
		QVector2D moveN = lastMove.normalized();
		if((diffN-moveN).lengthSquared() < 1.8)reaction(player->pos);
	}else if(move.lengthSquared()<0.3f){
		if(!getRandInt(150))move = QVector2D(getRandDouble2(), getRandDouble2());
		else move = QVector2D(0, 0);
	}
}

void Wolf::render(QPainter *painter){
	QVector2D screenPos = pos + game->getLevelOffset();

	float angle = atan2(lastMove.y(), lastMove.x())-M_PI/2;

	painter->translate(QPointF(screenPos.x(), screenPos.y()));
	painter->rotate(angle*180/M_PI);
	

	QImage *bodyImg = loadImage("textures/wolf_body.png");
	painter->drawImage(QRectF(-w/2, -h/2, w, h), *bodyImg);
	painter->resetTransform();

	QImage *headImg = loadImage("textures/wolf_head.png");
	QVector2D relPos = QVector2D(-sin(angle)*h/2, cos(angle)*h/2);
	painter->drawImage(QRectF(screenPos.x()+relPos.x()-w/4, screenPos.y()+relPos.y()-h/4, halfSZ, halfSZ), *headImg);
}
