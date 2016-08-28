#include "game.h"
#include "images.h"
#include "audio.h"
#include "rat.h"
#include "wolf.h"
#include "bear.h"

Game *game;
QThreadPool *gameThread;


void setup_game(){
	game = new Game();

	QObject::connect(closeFilter, &CloseEventFilter::closed, game, &Game::close);

	gameThread = new QThreadPool();
	gameThread->tryStart(game);
}

void cleanup_game(){
	delete gameThread;
}


Game::Game():running(1), inMenu(1), level(0), paused(0), toLoadLevel(0), toEndLevel(0), toRestartLevel(0), gameEnded(0){
	menu = new Menu();
	QObject::connect(window, &Window::keyPressEv, menu, &Menu::keyPress);
	QObject::connect(window, &Window::keyReleaseEv, menu, &Menu::keyRelease);
	QObject::connect(window, &Window::keyPressEv, this, &Game::keyPress);
	QObject::connect(window, &Window::keyReleaseEv, this, &Game::keyRelease);

	player = new Player();
	player->setActive(0);
	QObject::connect(window, &Window::keyPressEv, player, &Player::keyPress);
	QObject::connect(window, &Window::keyReleaseEv, player, &Player::keyRelease);
	//QObject::connect(audioPlayer, &QMediaPlayer::stateChanged, this, &Game::musicEnd);
	unpausedNow = 0;
}
Game::~Game(){
	delete player;
	delete menu;
}

void Game::run(){
	QTime *timing;
	timing = new QTime();
	timing->start();

	food = 0;

	int elapsed, lastElapsed = 0;
	int diff;
	double notTicked=0;
	int second=0;
	bool ticked = 0;

	int ticks=0, renders=0;

	while(running){
		elapsed = timing->elapsed();
		diff = elapsed - lastElapsed;
		second+=diff;
		notTicked+=diff;
		lastElapsed = elapsed;

		if(elapsed>23*60*60*1000){
			lastElapsed = 0;
			timing->restart();
		}



		if(notTicked > TICK_TIME*1000)while(notTicked > TICK_TIME*1000){
			notTicked -= TICK_TIME*1000;
			ticked = 1;

			ticks++;

			w = window->width();
			h = window->height();
			tick();
		}

		if(ticked){
			ticked = 0;
			renders++;

			resetRenderWindowSizeUpdater();

			render();
		}else{
			QThread::usleep(100);
		}


		if(second>=1000){
			printf("TPS: %d; FPS: %d\n", ticks, renders);
			second-=1000;
			ticks = 0;
			renders = 0;
		}
	}
	player->setActive(0);
	if(level)delete level;
	delete timing;
}




void Game::render(){
	if(!window->isExposed())
		return;


	QRect rect(0, 0, w, h);
	backingStore->beginPaint(rect);
	QPaintDevice *paintDevice = backingStore->paintDevice();
	QPainter *painter = new QPainter(paintDevice);

	render(painter);

	delete painter;

	if(running){
		backingStore->endPaint();

		backingStore->flush(rect);
	}
}

void Game::render(QPainter *painter){
	uint32_t x,y, i;
	painter->setPen(QColor(0xFFFFFF));

	//QFont font = painter->font();
	//font.setPointSize(FONT_SIZE);
	painter->setFont(font);



	painter->fillRect(0, 0, w, h, QColor(level?0x147e1e:0xaaaaba));


	if(level){
		QVector2D plPos = player->pos;

		QVector2D levelOffset = getLevelOffset();
		QVector2D diff = plPos + levelOffset - QVector2D(w/2.f,h/2.f);
		if(diff.lengthSquared()>0.2f){

			if(level->getW()*64 < w){
				offset.setX(0);
			}else if(plPos.x() < w/2){
				offset.setX((level->getW()*64-w)/2);
			}else if(plPos.x() > level->getW()*64-w/2){
				offset.setX((w-level->getW()*64)/2);
			}else{
				offset.setX(offset.x() - diff.x());
			}

			if(level->getH()*64 < h){
				offset.setY(0);
			}else if(plPos.y() < h/2-48){
				offset.setY((level->getH()*64-h)/2+48);
			}else if(plPos.y() > level->getH()*64-h/2+40){
				offset.setY((h-level->getH()*64)/2-40);
			}else{
				offset.setY(offset.y() - diff.y());
			}
		}


		const int *mapText = level->getMapText();
		int minx, miny;
		int maxx, maxy;
		levelOffset = getLevelOffset();
		for(x=0;x<level->getW();++x){
			minx = x*64+levelOffset.x();
			if(minx < - 64 || minx > w)continue;
			for(y=0;y<level->getH();++y){
				miny = y*64+levelOffset.y();
				if(miny < - 64 || miny > h)continue;
				QImage *img = level->getImage(mapText[x+y*level->getW()]);
				painter->drawImage(QRect(minx, miny, 64, 64), *img);
			}
		}
		for(i=0;i<devices.size();++i){
			devices[i]->render(painter);
		}
		for(i=0;i<traders.size();++i){
			traders[i]->render(painter);
		}
		for(i=0;i<enemies.size();++i){
			enemies[i]->render(painter);
		}

		if(!gameEnded){
		 	if(!shop)painter->drawText(QRect(0, 0, w, 48), Qt::AlignHCenter | Qt::TextWordWrap, "Collect food: " + QString::number(food) + " food. " + QString::number(ticksRemaining/60) + " seconds remaining.");
			else painter->drawText(QRect(0, 0, w, 48), Qt::AlignHCenter | Qt::TextWordWrap, "You can trade your food: " + QString::number(food) + ".");
		}

		player->render(painter);
	}
	if(inMenu || paused){
		/*int minx, miny;
		if(!level)for(x=0;;++x){
			minx = x*64;
			if(minx > w)break;
			for(y=0;;++y){
				miny = y*64;
				if(miny > h)break;
				QImage *img = loadImage("textures/textures.png");
				painter->drawImage(QRect(minx, miny, 64, 64), *img, QRect(0, 0, 32, 32));
			}
		}*/
		menu->render(painter);
	}
	if(unpausedNow)unpausedNow = 0;
}







void Game::loadLevel(QString name){
	levelName = name;
	toLoadLevel = 1;
}
void Game::endLevel(){
	toEndLevel = 1;
}
void Game::restartLevel(){
	toRestartLevel = 1;
}

QVector2D Game::getLevelOffset(){
	return offset + QVector2D((w - level->getW()*64)/2, (h - level->getH()*64)/2);
}

void Game::setActive(bool active){
	uint32_t i;
	player->setActive(active);
	for(i=0;i<enemies.size();++i){
		enemies[i]->setActive(active);
	}
	for(i=0;i<devices.size();++i){
		devices[i]->setActive(active);
	}
	for(i=0;i<traders.size();++i){
		traders[i]->setActive(active);
	}
}

void Game::keyPress(QKeyEvent *ev){
	int key = ev->key();
	if(level && !paused){
		if(key == Qt::Key_Escape && !player->getTrader() && !paused && !unpausedNow){
			paused = 1;
			menu->setActive(1);
			setActive(0);
		}
		if(key == Qt::Key_R && enemies.size()){
			enemyKilled(enemies[0]);
		/*food++;
		delete enemies[0];
		enemies.erase(enemies.begin());*/

		/*for(int i=0;i<enemies.size();++i){
			food+=1;
			delete enemies[i];
		}
		enemies.clear();*/
		}
	}
	//if(key == Qt::Key_Q)playSound("audio/jump.wav");
	//if(key == Qt::Key_R)playMusic("audio/jump.wav");
	//if(key == Qt::Key_E)playMusic("audio/example.ogg");
}
void Game::keyRelease(QKeyEvent *ev){

}

#define MUSICS 2
QString musicsa[] = {"audio/jump.wav", "audio/example.ogg"};
void Game::musicEnd(QMediaPlayer::State state){
	if(state == QMediaPlayer::StoppedState){
		playMusic(musicsa[getRandInt(MUSICS)]);
	}
}



void Game::tick(){
	uint32_t i;
	if(toEndLevel){
		delete level;
		level = 0;
		inMenu = 1;
		menu->setActive(1);
		toEndLevel = 0;
		for(i=0;i<enemies.size();++i){
			delete enemies[i];
		}
		enemies.clear();
		for(i=0;i<devices.size();++i){
			delete devices[i];
		}
		devices.clear();
		for(i=0;i<traders.size();++i){
			delete traders[i];
		}
		player->reset();
		traders.clear();
	}
	if(toLoadLevel){
		level = new Level();
		if(levelName != ""){
			level->setup(levelName.toStdString().c_str());
		}else{
			level->setup(32, 32);
		}

		int prop;
		for(i=0;i<level->getW() * level->getH();++i){
			prop = level->getMapProp(i);
			if(prop == 3){
				player->pos = QVector2D((i%level->getW()) * 64 +32,(i/level->getW())*64+32);
			}else if(prop>=0x11 && prop <= 0x2A){
				Trader *trader = new Trader(QVector2D((i%level->getW()) * 64 +32,(i/level->getW())*64+32), prop-0x11);
				addTrader(trader);
			}
		}

		inMenu = 0;
		menu->setActive(0);
		toLoadLevel = 0;
		player->setActive(1);
		player->reset();

		ticksRemaining = 60*300+food*60;
		if(levelName == "maps/shop.tmx"){
			shop = 1;
		}else{
			shop = 0;
			food = 0;
		}


		gameEnded = 0;

		playerDead = 0;
		offset = QVector2D(0,0);
	}
	if(toRestartLevel){
		delete level;
		for(i=0;i<enemies.size();++i){
			delete enemies[i];
		}
		enemies.clear();
		for(i=0;i<devices.size();++i){
			delete devices[i];
		}
		devices.clear();
		for(i=0;i<traders.size();++i){
			delete traders[i];
		}
		traders.clear();

		ticksRemaining = 60*300+food*60;
		if(levelName == "maps/shop.tmx"){
			shop = 1;
		}else{
			shop = 0;
			food = 0;
		}

		level = new Level();
		if(levelName != ""){
			level->setup(levelName.toStdString().c_str());
		}else{
			level->setup(32, 32);
		}
		int prop;
		for(i=0;i<level->getW() * level->getH();++i){
			prop = level->getMapProp(i);
			if(prop == 3){
				player->pos = QVector2D((i%level->getW()) * 64 +32,(i/level->getW())*64+32);
			}else if(prop>=0x11 && prop <= 0x2A){
				Trader *trader = new Trader(QVector2D((i%level->getW()) * 64 +32,(i/level->getW())*64+32), prop-0x11);
				addTrader(trader);
			}
		}

		player->setActive(1);
		player->reset();
		gameEnded = 0;

		playerDead = 0;

		offset = QVector2D(0,0);
		toRestartLevel = 0;
	}






	if(inMenu || paused){
		menu->tick();
	}else if(level){
		level->tick();
		//enemyKills();
		player->tick();

		if(!shop)if(enemies.size()*96 < level->getW() * level->getH()){
			if(getRandDouble() < 0.5){
				spawnEnemy();
			}
		}



		if(ticksRemaining)ticksRemaining--;
		else{
			if(food)food--;
		}


		for(i=0;i<enemies.size();++i){
			enemies[i]->tick();
		}
		for(i=0;i<devices.size();++i){
			devices[i]->tick();
		}
		for(i=0;i<traders.size();++i){
			traders[i]->tick();
		}



	}
}










Player *Game::isPlayer(QRectF rect){
	uint32_t i;
	QRectF playRect = QRectF(player->pos.x() - player->getW()/2, player->pos.y()-player->getW()/2, player->getW(), player->getH());
	if(rect.intersects(playRect)){
		return player;
	}
	return 0;
}

void Game::playerDied(){
	playerDead = 1;
	paused = 1;
	menu->setActive(1);
	setActive(0);
	gameEnded = 1;
	menu->setEndGameMessage("You died! You collected: " + QString::number(game->getFood()) + " food. However your family is starving.");
	food = 0;
}



Enemy *Game::isEnemy(QRectF rect){
	uint32_t i;
	for(i=0;i<enemies.size();++i){
		if(enemies[i]->myReaction)continue;
		QRectF enemRect = QRectF(enemies[i]->pos.x()-enemies[i]->getW()/2, enemies[i]->pos.y()-enemies[i]->getH()/2, enemies[i]->getW(), enemies[i]->getH());
		if(rect.intersects(enemRect)){
			return enemies[i];
		}
	}
	return 0;
}
/*void Game::enemyKill(Enemy *enemy){
	enemyToKill.push_back(enemy);
}


void Game::enemyKills(){
	uint32_t i;
	for(i=0;i<enemyToKill.size();++i){
		enemyKill(enemyToKill[i]);
	}
	enemyToKill.clear();
}*/

void Game::enemyKilled(Enemy *enemy){
	uint32_t i;

	for(i=0;i<enemies.size();++i){
		if(enemy==enemies[i]){
			food += enemy->getFood();
			delete enemy;
			enemies.erase(enemies.begin()+i);
			return;
		}
	}
}

void Game::enemyReaction(QVector2D pos, int type){
	uint32_t i;
	for(i=0;i<enemies.size();++i){
		enemies[i]->reaction(pos, type);
	}	
}




Device *Game::isDevice(QRectF rect){
	uint32_t i;
	for(i=0;i<devices.size();++i){
		QRectF devRect = QRectF(devices[i]->pos.x()-devices[i]->getW()/2, devices[i]->pos.y()-devices[i]->getH()/2, devices[i]->getW(), devices[i]->getH());
		if(rect.intersects(devRect)){
			return devices[i];
		}
	}
	return 0;
}
Trader *Game::isTrader(QRectF rect){
	uint32_t i;
	for(i=0;i<traders.size();++i){
		QRectF shopRect = QRectF(traders[i]->pos.x()-traders[i]->getW()/2, traders[i]->pos.y()-traders[i]->getH()/2, traders[i]->getW(), traders[i]->getH());
		if(rect.intersects(shopRect)){
			return traders[i];
		}
	}
	return 0;
}



void Game::spawnEnemy(){
	int pos;
	QVector2D posa;
	pos = getRandInt(0, level->getW()*level->getH());
	posa = QVector2D((pos%level->getW())*64, (pos/level->getW())*64);
	posa+=QVector2D(32,32);


	double enemyDiff = (1 - 1.f/((food+20)/20.f))*abs(getRandDouble2());
	if(enemyDiff < 0.2){
		QRectF Rect = QRectF(posa.x()-12, posa.y()-12, 24, 24);
		if(level->getMapProp(pos)!=0 || isEnemy(Rect) || isPlayer(Rect) || isDevice(Rect) || isTrader(Rect)){
			return;
		}
		enemies.push_back(new Rat(posa));
	}else if(enemyDiff < 0.5){
		QRectF Rect = QRectF(posa.x()-16, posa.y()-16, 32, 32);
		if(level->getMapProp(pos)!=0 || isEnemy(Rect) || isPlayer(Rect) || isDevice(Rect) || isTrader(Rect)){
			return;
		}
		enemies.push_back(new Wolf(posa));
	}else{
		QRectF Rect = QRectF(posa.x()-16, posa.y()-16, 32, 32);
		if(level->getMapProp(pos)!=0 || isEnemy(Rect) || isPlayer(Rect) || isDevice(Rect) || isTrader(Rect)){
			return;
		}
		enemies.push_back(new Bear(posa));
	}
}

void Game::addDevice(Device *device){
	devices.push_back(device);
}

void Game::removeDevice(Device *device){
	uint32_t i;

	for(i=0;i<devices.size();++i){
		if(devices[i] == device){
			devices.erase(devices.begin()+i);
			break;
		}
	}
}
void Game::addTrader(Trader *trader){
	traders.push_back(trader);
}





void Game::setGameEnded(){
	if(shop){
		levelName = "";
		toRestartLevel = 1;
	}else{
		levelName = "maps/shop.tmx";
		gameEnded = 1;

		paused = 1;
		menu->setActive(1);
		setActive(0);
		menu->setEndGameMessage("Level completed! You collected " + QString::number(food) + " food.");
	}
}
