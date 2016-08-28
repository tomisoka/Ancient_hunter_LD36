#include "enemy.h"
#include "level.h"
#include "game.h"

Enemy::Enemy(QVector2D pos):w(32), h(32), move(QVector2D(0,0)), pos(pos), active(1), myReaction(0){
	foodValue = 1;
	HP = 1;
	lastMove = QVector2D(0,1);
	attackDelay = 0;
	halfSZ = (w+h)/4;
	looped = 0;
}

Enemy::~Enemy(){

}

void Enemy::killed(){
	game->enemyKilled(this);
}


void Enemy::tick(){
	if(!active)return;
	looped = 0;
	tickMove();
}

#define checkTiles(ID) if(tiles[0] == ID || tiles[1] == ID || tiles[2] == ID || tiles[3] == ID)
#define checkNotTiles(ID) if(tiles[0] != ID && tiles[1] != ID && tiles[2] != ID && tiles[3] != ID)
void Enemy::tickMove(){
	Level *level = game->getLevel();

	QVector2D nwPos = pos + move;
	QPoint newPos = QPoint((int)nwPos.x(), (int)nwPos.y());

	int tiles[4];

	tiles[0] = level->getMapProp((newPos.x()-w/2+64)/64-1, (newPos.y()-h/2+64)/64-1);
	tiles[1] = level->getMapProp((newPos.x()+w/2+64)/64-1, (newPos.y()-h/2+64)/64-1);
	tiles[2] = level->getMapProp((newPos.x()-w/2+64)/64-1, (newPos.y()+h/2+64)/64-1);
	tiles[3] = level->getMapProp((newPos.x()+w/2+64)/64-1, (newPos.y()+h/2+64)/64-1);


	checkNotTiles(1){
		myReaction = 1;
		QRectF rect = QRectF(nwPos.x()-w/2, nwPos.y()-h/2, w, h);
		if(!game->isEnemy(rect) && !game->isTrader(rect)){
			if(!game->isPlayer(rect)){
				pos = nwPos;
			}else if (attackDelay)crashed();
		}else crashed();
		myReaction = 0;
	}else{
		crashed();
	}
	checkTiles(2){
		myReaction = 1;
		game->enemyReaction(pos, 1);
		myReaction = 0;
	}

	if(move!=QVector2D(0,0)){
		lastMove = move;
	}
}

void Enemy::render(QPainter* painter){
	QVector2D screenPos = pos + game->getLevelOffset();
	painter->fillRect(QRectF(screenPos.x()-w/2, screenPos.y()-h/2, w, h), QColor(0xFF0000));



	QVector2D attackPos = QVector2D(lastMove.x(), lastMove.y());
	attackPos.normalize();
	attackPos *= halfSZ*3/2;
	QVector2D attPs = pos + QVector2D(attackPos.x(), attackPos.y()) + game->getLevelOffset();
	painter->drawRect(QRectF(attPs.x()-halfSZ/2, attPs.y()-halfSZ/2, halfSZ, halfSZ));
}
