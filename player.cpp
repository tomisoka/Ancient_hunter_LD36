#include "player.h"
#include "level.h"
#include "game.h"
#include "campfire.h"
#include "trap.h"

Player::Player():moveLeft(0), moveUp(0), moveRight(0), moveDown(0), w(32), h(32), active(0), lastMove(QVector2D(0,1)), attack(0), HP(20), maxHP(20), stamina(1000), maxStamina(1000), sprint(0), loadStamina(0){
	halfSZ = (w+h)/4;

	weapon = 0;

	maxHP = statsUnlocked[0];
	HP = maxHP;
	maxStamina = statsUnlocked[1];
	stamina = maxStamina;

	reset();
}

Player::~Player(){

}

void Player::reset(){
	moveLeft = 0;
	moveUp = 0;
	moveRight = 0;
	moveDown = 0;
 	lastMove = QVector2D(0,1);
	attack = 0;
	maxHP = statsUnlocked[0];
	HP = maxHP;
	maxStamina = statsUnlocked[1];
	stamina = maxStamina;
	loadStamina = 0;
	sprint = 0;
	attackedDelay = 0;
	renderAttack = 0;
	trader = 0;
	communicate = 0;
	bowDist = 0;
};




void Player::tick(){
	if(!active)return;
	tickMove();
	if(attack){
		tickAttack();
		attack = 0;
	}

	if(weapon==4 && bowDistChange){
		if(bowDistChange == -1 && bowDist)bowDist--;
		if(bowDistChange == 1 && bowDist < 52)bowDist++;
		
	}

	if(communicate){
		tickCommunication();
	}

	if(attackedDelay)attackedDelay--;

	if(loadStamina)loadStamina--;
	else if(stamina < maxStamina){
		stamina++;
	}
	if(renderAttack)renderAttack--;
}

void Player::tickAttack(){
	uint32_t i;
	if(stamina < 50)return;
	if(weapon == 3)return;
	stamina -= 50;
	loadStamina = 200;

	int attackForce = 1;

	if(weapon == 2){
		attackForce = 2;
	}else if(weapon == 5){
		attackForce = 4;
	}


	QVector2D attackPos = lastMove;
	attackPos.normalize();

	vector<QVector2D> attPs;
	if(weapon == 0 || weapon == 2 || weapon == 5){
		attackPos *= halfSZ*15/8.f;
		attPs.push_back(pos + attackPos);
	}else if(weapon == 1){	
		QVector2D temp;
		temp = attackPos*halfSZ*15/8.f;
		attPs.push_back(pos + temp);
		temp = attackPos*halfSZ*23/8.f;
		attPs.push_back(pos + temp);
	}else if(weapon==4){
		attackPos *= halfSZ*(44+bowDist)/8.f;
		attPs.push_back(pos + attackPos);
	}


	Enemy *enemy;

	for(i=0;i<attPs.size();++i){
		enemy = game->isEnemy(QRect(attPs[i].x()-halfSZ*2/4.f, attPs[i].y()-halfSZ*2/4.f, halfSZ*2/2.f, halfSZ*2/2.f));
		if(enemy)break;
	}

	if(enemy){
		enemy->reaction(pos);
		enemy->attacked(attackForce);
	}
	renderAttack = 20;
}

#define checkTiles(ID) if(tiles[0] == ID || tiles[1] == ID || tiles[2] == ID || tiles[3] == ID)
#define checkAllTiles(ID) if(tiles[0] == ID && tiles[1] == ID && tiles[2] == ID && tiles[3] == ID)
#define checkNotTiles(ID) if(tiles[0] != ID && tiles[1] != ID && tiles[2] != ID && tiles[3] != ID)
void Player::tickMove(){
	Level *level = game->getLevel();

	move = QVector2D(0,0);
	if(moveLeft)move+=QVector2D(-1,0);
	if(moveUp)move+=QVector2D(0,-1);
	if(moveRight)move+=QVector2D(1,0);
	if(moveDown)move+=QVector2D(0,1);

	if(move!=QVector2D(0,0)){
		move.normalize();
		move*=1.3f;

		if(sprint && stamina >= 10){
			move *= 2;
			stamina-= 10;
			loadStamina = 100;
		}

		QVector2D nwPos = pos + move;
		QPoint newPos = QPoint(nwPos.x(), nwPos.y());

		int tiles[4];
		tiles[0] = level->getMapProp((newPos.x()-w/2+64)/64-1, (newPos.y()-h/2+64)/64-1);
		tiles[1] = level->getMapProp((newPos.x()+w/2+64)/64-1, (newPos.y()-h/2+64)/64-1);
		tiles[2] = level->getMapProp((newPos.x()-w/2+64)/64-1, (newPos.y()+h/2+64)/64-1);
		tiles[3] = level->getMapProp((newPos.x()+w/2+64)/64-1, (newPos.y()+h/2+64)/64-1);


		checkNotTiles(1){
			QRectF rect = QRectF(nwPos.x()-w/2, nwPos.y()-h/2, w, h);
			if(!game->isEnemy(rect) && !game->isTrader(rect)){
				pos = nwPos;
			}
		}
		checkTiles(2){
			game->enemyReaction(pos);
		}
		checkAllTiles(4){
			game->setGameEnded();
		}
		lastMove = move;
	}
	if(weapon==3)game->enemyReaction(pos, 1);
}


void Player::tickCommunication(){
	QVector2D commPos = lastMove;
	commPos.normalize();

	commPos *= halfSZ*15/8.f;
	commPos += pos;

	trader = game->isTrader(QRect(commPos.x()-halfSZ*3/4.f, commPos.y()-halfSZ*3/4.f, halfSZ*3/2.f, halfSZ*3/2.f));
	if(trader)trader->setShopping(1);

	communicate = 0;
}

void Player::keyPress(QKeyEvent *ev){
	if(!active)return;
	int key = ev->key();

	if(trader){
		if(key == Qt::Key_F || key == Qt::Key_Escape){
			trader = 0;
			if(trader)trader->setShopping(0);
		}else trader->keyPress(ev);
	}else if(key == Qt::Key_A || key == Qt::Key_Left){
		moveLeft = 1;
	}else if(key == Qt::Key_W || key == Qt::Key_Up){
		moveUp = 1;
	}else if(key == Qt::Key_D || key == Qt::Key_Right){
		moveRight = 1;
	}else if(key == Qt::Key_S || key == Qt::Key_Down){
		moveDown = 1;
	}else if(key == Qt::Key_Space){
		attack = 1;
	}else if(key == Qt::Key_Control){
		sprint = 1;


	}else if(key == Qt::Key_Q){

		do{
			weapon++;
			if(weapon >= weaponsUnlocked.size())weapon = 0;
		}while(!weaponsUnlocked[weapon]);

	}else if(key == Qt::Key_E){

		do{
			weapon--;
			if(weapon <0)weapon =  weaponsUnlocked.size()-1;
		}while(!weaponsUnlocked[weapon]);

	}else if(key == Qt::Key_C){
		if(devicesUnlocked[0]){
			QVector2D campPos = lastMove;

			campPos*=halfSZ*18/8.f;
			campPos = pos + campPos;

			game->addDevice(new CampFire(campPos));
			devicesUnlocked[0]--;
		}
	}else if(key == Qt::Key_T){
		if(devicesUnlocked[1]){
			QVector2D trapPos = lastMove;

			trapPos*=halfSZ*18/8.f;
			trapPos = pos + trapPos;

			game->addDevice(new Trap(trapPos));
			devicesUnlocked[1]--;
		}


	}else if(key == Qt::Key_F){
		communicate = 1;


	}else if(key == Qt::Key_Z){
		bowDistChange = -1;
	}else if(key == Qt::Key_Y){
		bowDistChange = 1;
	}
}

void Player::keyRelease(QKeyEvent *ev){
	if(!active)return;
	int key = ev->key();
	if(trader){
		trader->keyRelease(ev);
	}
	if(key == Qt::Key_A || key == Qt::Key_Left){
		moveLeft = 0;
	}else if(key == Qt::Key_W || key == Qt::Key_Up){
		moveUp = 0;
	}else if(key == Qt::Key_D || key == Qt::Key_Right){
		moveRight = 0;
	}else if(key == Qt::Key_S || key == Qt::Key_Down){
		moveDown = 0;
	}else if(key == Qt::Key_Control){
		sprint = 0;


	}else if(key == Qt::Key_Z){
		bowDistChange = 0;
	}else if(key == Qt::Key_Y){
		bowDistChange = 0;
	}
}


QString weaponImgs[] = {"textures/bone_weapon.png", "textures/spear_weapon.png", "textures/club_weapon.png",
"textures/torch_weapon.png", "textures/bow_weapon.png", "textures/club2_weapon.png"};
void Player::render(QPainter* painter){
	QVector2D screenPos = pos + game->getLevelOffset();

	float angle = atan2(lastMove.y(), lastMove.x())-M_PI/2;

	painter->translate(QPointF(screenPos.x(), screenPos.y()));
	painter->rotate(angle*180/M_PI);

	QImage *bodyImg = loadImage("textures/caveman_body.png");
	painter->drawImage(QRectF(-w/2, -h/2, w, h), *bodyImg);

	QImage *headImg = loadImage("textures/caveman_head.png");
	painter->drawImage(QRectF(-halfSZ/2, -halfSZ/2 -4, halfSZ, halfSZ), *headImg);


	QImage *weaponImg = loadImage(weaponImgs[weapon]);
	if(renderAttack){
		painter->translate(QPointF(-halfSZ, halfSZ));
		painter->rotate(-45);
		painter->drawImage(QRectF(0, 0, halfSZ*2, halfSZ*2), *weaponImg);
	}else{
		painter->drawImage(QRectF(-halfSZ, halfSZ/2.f, halfSZ*2, halfSZ*2), *weaponImg);
	}

	painter->resetTransform();





	if(weapon == 4){
		QVector2D attackPos = lastMove;
		attackPos.normalize();
		attackPos *= halfSZ*(44+bowDist)/8.f;
		QVector2D attPs = pos + QVector2D((int)attackPos.x(), (int)attackPos.y()) + game->getLevelOffset();
		painter->drawLine(attPs.x()-halfSZ*2/4.f, attPs.y(), attPs.x() + halfSZ*2/4.f, attPs.y());
		painter->drawLine(attPs.x(), attPs.y()-halfSZ*2/4.f, attPs.x(), attPs.y()+halfSZ*2/4.f);
	}




	painter->fillRect(QRect(32, game->getH()-32, (game->getW()-64)*stamina/maxStamina, 8), QColor(0x8888FF));
	painter->drawRect(QRect(32, game->getH()-32, (game->getW()-64), 8));

	painter->fillRect(QRect(32, game->getH()-16, (game->getW()-64)*HP/maxHP, 8), QColor(0xFF0000));
	painter->drawRect(QRect(32, game->getH()-16, (game->getW()-64), 8));

	if(trader)trader->renderShop(painter);
}





void Player::attacked(int force){
	if(attackedDelay)return;
	HP-=force;
	if(HP <= 0){
		HP = 0;
		game->playerDied();
	}
	attackedDelay = 10;
}
