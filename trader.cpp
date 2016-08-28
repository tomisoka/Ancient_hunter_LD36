#include "trader.h"
#include "game.h"
#include "images.h"
#include "unlock.h"

int unlocks[] = {6,2,2};

QString names[][64] = {
	{
		"bone",
		"spear",
		"club",
		"torch",
		"bow",
		"big club",
	},
	{
		"campfire",
		"trap",
	},
	{
		"HP",
		"stamina",
	},
};
QString textures[][64] = {
	{	
		"textures/bone_weapon.png",
		"textures/spear_weapon.png",
		"textures/club_weapon.png",
		"textures/torch_weapon.png",
		"textures/bow_weapon.png",
		"textures/club2_weapon.png",
	},
	{
		"textures/campfire.png",
		"textures/trap.png",
	},
	{
		"",
		"",
	},
};

int costs[][64] = {
	{
		0,
		40,
		40,
		50,
		70,
		100,
	},
	{
		10,
		12,
	},
	{
		25,
		10,
	},
};

int addN[][64]{
	{
		1,
		1,
		1,
		1,
		1,
		1,
	},
	{
		1,
		1,
	},
	{
		1,
		10,
	},
};

QString info[][64] = {
	{
		"Default weapon: 1DMG, low range 1-13 u.",
		"Longer range weapon: 1DMG, longer range 1-21 u.",
		"Stronger weapon: 2DMG, low range 1-13 u.",
		"Fear weapon: repels animals.",
		"Long range weapon: 1DMG, long range 30-100 u. Distance controlled by Y and Z.",
		"Strong weapon: 4DMG, low range 1-13 u.",
	},
	{
		"Causes damage and repels animals. Place it with C.",
		"Causes instant kill to animals then destroys itself. Place it with T.",
	},
	{
		"More damage is needed to kill you.",
		"You can run longer.",
	},
};

bool buyOne[] = {
	1,
	0,
	0,
};

vector<vector<int>*> unlocksV;

void setup_trader(){
	unlocksV.push_back(&weaponsUnlocked);
	unlocksV.push_back(&devicesUnlocked);
	unlocksV.push_back(&statsUnlocked);
}


Trader::Trader(QVector2D pos, int ID):pos(pos), h(32), w(32), active(1), ID(ID), chosen(0), shopping(0), toAudio(0){
}

Trader::~Trader(){

}

void Trader::keyPress(QKeyEvent *ev){
	uint32_t i;
	if(!shopping)return;
	int key = ev->key();


	if(key == Qt::Key_W || key == Qt::Key_Up){
		if(chosen)chosen--;
	}else if(key == Qt::Key_S || key == Qt::Key_Down){
		if(chosen<unlocks[ID]-1)chosen++;
	}else if(key == Qt::Key_Enter || key == Qt::Key_Return){
		bool buyed = unlocksV[ID][0][chosen];
		if((!buyed || !buyOne[ID]) && game->getFood() >= costs[ID][chosen]){
			game->setFood(game->getFood() - costs[ID][chosen]);
			unlocksV[ID][0][chosen]+=addN[ID][chosen];
			toAudio = 1;
		}
	}
}

void Trader::keyRelease(QKeyEvent *ev){
	if(!shopping)return;

}

void Trader::setShopping(bool shopping){
	this->shopping = shopping;
	chosen = 0;
}
void Trader::tick(){
	if(toAudio){
		toAudio = 0;
	}
}

void Trader::render(QPainter *painter){
	QVector2D screenPos = pos + game->getLevelOffset();

	QImage *img;
	img = loadImage("textures/trader.png");
	painter->drawImage(QRectF(screenPos.x()-w/2, screenPos.y()-h/2, w, h), *img);
}

void Trader::renderShop(QPainter *painter){
	uint32_t i;
	QRectF winRect = QRectF(32, 48, game->getW()-64, game->getH()-96);

	painter->fillRect(winRect, QColor(0xaaaaaa));

	int xOffset = 384;

	painter->setPen(QColor(0x555555));
	painter->drawRect(winRect);
	painter->drawLine(QPoint(xOffset, 48), QPoint(xOffset, game->getH()-48));
	painter->setPen(QColor(0xFFFFFF));



	int yTextDiff = FONT_SIZE+12;
	int offset = 48 + yTextDiff;
	painter->drawText(40, chosen*yTextDiff + offset, ">");

	for(i=0;i<unlocks[ID];++i){
		painter->drawText(60, i*yTextDiff + offset, names[ID][i]);
	}


	if(textures[ID][chosen]!=""){
		QImage *img = loadImage(textures[ID][chosen]);

		int imgW = img->width();
		int imgH = img->height();

		if(imgW == imgH){
			painter->drawImage(QRectF(xOffset + 16, 64, 128, 128), *img);
		}else{
			int smaller = (imgW > imgH)?imgH:imgW;
			painter->drawImage(QRectF(xOffset + 16, 64, 128, 128), *img, QRect(0,0, smaller, smaller));
		}
	}


	int buyed = unlocksV[ID][0][chosen];
	if(buyed && buyOne[ID]) painter->drawText(QRect(xOffset + 32 + 128, 64, game->getW() - xOffset-32-128-32, 128), Qt::TextWordWrap, "Cost: Already buyed.");
	else painter->drawText(QRect(xOffset + 32 + 128, 64, game->getW() - xOffset-32-128-32, 128), Qt::TextWordWrap, "Cost: " + QString::number(costs[ID][chosen])
		 + ((addN[ID][chosen]!=1)?("(" + QString::number(addN[ID][chosen])+ " items)"):""));

	if(buyed && !buyOne[ID]){
		painter->drawText(QRect(xOffset + 32 + 128, 64 + 80, game->getW() - xOffset-32-128-32, 128), Qt::TextWordWrap, "You have:" + QString::number(buyed));
	}

	painter->drawText(QRect(xOffset + 32, 64 +128+16, game->getW() - xOffset-32-32, game->getH()-64-64-128-16), Qt::TextWordWrap, info[ID][chosen]);
}
