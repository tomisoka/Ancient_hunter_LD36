#pragma once

#include "render.h"
#include "level.h"
#include "menu.h"
#include "player.h"
#include "enemy.h"
#include "device.h"
#include "trader.h"
#include "unlock.h"

class Game: public QObject, public QRunnable{
	Q_OBJECT
public:
	Game();
	~Game();

	int getW(){return w;}
	int getH(){return h;}

	void unpause(){paused = 0; menu->setActive(0); setActive(1); unpausedNow = 1;}
	void loadLevel(QString name);
	void endLevel();
	void restartLevel();
	QVector2D getLevelOffset();

	Enemy *isEnemy(QRectF rect);
	Player *isPlayer(QRectF rect);
	Device *isDevice(QRectF rect);
	Trader *isTrader(QRectF rect);
	void enemyKilled(Enemy *enemy);
	void playerDied();
	bool isGameEnded(){return gameEnded;}
	void setGameEnded();

	void enemyReaction(QVector2D pos, int type=0);

	int getFood(){return food;}
	void setFood(int food){this->food = food;}
	Level *getLevel(){return level;}
	Player *getPlayer(){return player;}
	bool isDeadPlayer(){return playerDead;}

	void addDevice(Device *device);
	void removeDevice(Device *device);


	void addTrader(Trader *trader);

	virtual void run();
public slots:
	void keyPress(QKeyEvent *ev);
	void keyRelease(QKeyEvent *ev);

	void close(){running = 0;}
protected:
	//void enemyKills();
	//void enemyKill(Enemy *enemy);


	int ticksRemaining;

	void spawnEnemy();

	QVector2D offset;

	void setActive(bool active);

	bool shop;
	bool playerDead;

	bool gameEnded;

	void tick();
	void render();
	void render(QPainter *painter);

	bool running;

	bool inMenu;
	bool paused;

	int w, h;

	Menu *menu;
	Level *level;
	Player *player;

	vector<Enemy*> enemyToKill;
	vector<Enemy*> enemies;
	vector<Device*> devices;
	vector<Trader*> traders;


	int food;



	bool toLoadLevel;
	QString levelName;
	bool toEndLevel;
	bool toRestartLevel;

	bool unpausedNow;
};

extern Game *game;

void setup_game();
void cleanup_game();
