#pragma once

#include "render.h"
#include "level.h"
#include "menu.h"

class Game: public QObject, public QRunnable{
	Q_OBJECT
public:
	Game();

	int getW(){return w;}
	int getH(){return h;}

	void unpause(){paused = 0; menu->setActive(0);}
	void loadLevel(QString name);
	void endLevel();
	void restartLevel();

	virtual void run();
public slots:
	void keyPress(QKeyEvent *ev);
	void keyRelease(QKeyEvent *ev);

	void musicEnd(QMediaPlayer::State state);

	void close(){running = 0;}
protected:
	void tick();
	void render();
	void render(QPainter *painter);

	bool running;

	bool inMenu;
	bool paused;

	int w, h;

	Menu *menu;
	Level *level;


	bool toLoadLevel;
	QString levelName;
	bool toEndLevel;
	bool toRestartLevel;
};

extern Game *game;

void setup_game();
void cleanup_game();
