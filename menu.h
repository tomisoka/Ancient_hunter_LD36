#pragma once

#include "common.h"

class Menu: public QObject{
	Q_OBJECT
public:
	Menu();

	void tick();
	void render(QPainter *painter);

	void setActive(bool active){this->active = active;}
	void setEndGameMessage(QString message){endGameMessage = message;}

public slots:
	void keyPress(QKeyEvent *ev);
	void keyRelease(QKeyEvent *ev);

protected:
	QString endGameMessage;

	bool inGameMenu;
	bool levelChoose;
	bool info;
	bool inOptions;

	vector<QString> levelNames;
	int yLevelOffset;

	int chosen;

	bool active;
};
