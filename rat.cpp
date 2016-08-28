#include "rat.h"
#include "game.h"
#include "player.h"

Rat::Rat(QVector2D pos): Enemy(pos){
	foodValue = 5;
	HP = 1;
	w=24;
	h=24;
	halfSZ = (w+h)/4;
}

void Rat::reaction(QVector2D pos, int type){
	if(myReaction)return;
	QVector2D diff = pos-this->pos;

	QVector2D diffN = diff.normalized();
	if(diff.lengthSquared() < (12*4096) && move.lengthSquared() < diffN.lengthSquared()*5.f){

		move = diffN*-2.5f;
	}
}

void Rat::crashed(){
	looped++;
	if(looped > 50){
		looped = 0;
		return;
	}
	float angle = (getRandDouble()*2-1) * 1;
	move = QVector2D(cos(angle)*move.x() - sin(angle)*move.y(), sin(angle)*move.x() + cos(angle)*move.y());
	tickMove();
}

void Rat::tick(){
	if(!active)return;
	looped = 0;
	tickMove();
	move *= 0.997f;


	Player *player = game->getPlayer();

	QVector2D diff = player->pos-pos;
	float diffF = diff.lengthSquared();
	if(diffF < 36*36){
		reaction(player->pos);
	}else if(diffF < 120*120){
		QVector2D diffN = diff.normalized();
		QVector2D moveN = lastMove.normalized();
		if((diffN-moveN).lengthSquared() < 1.2)reaction(player->pos);
	}else if(move.lengthSquared()<0.1f){
		
		if(!getRandInt(100))move = QVector2D(getRandDouble2(), getRandDouble2());
		else move = QVector2D(0, 0);
	}
}

void Rat::render(QPainter *painter){
	QVector2D screenPos = pos + game->getLevelOffset();

	float angle = atan2(lastMove.y(), lastMove.x())-M_PI/2;

	painter->translate(QPointF(screenPos.x(), screenPos.y()));
	painter->rotate(angle*180/M_PI);

	QImage *bodyImg = loadImage("textures/rat_body.png");
	painter->drawImage(QRectF(-w/2, -h/2, w, h), *bodyImg);

	painter->resetTransform();

	QImage *headImg = loadImage("textures/rat_head.png");
	QVector2D relPos = QVector2D(-sin(angle)*10, cos(angle)*10);
	painter->drawImage(QRectF(screenPos.x()+relPos.x()-10, screenPos.y()+relPos.y()-10, 20, 20), *headImg);
}
