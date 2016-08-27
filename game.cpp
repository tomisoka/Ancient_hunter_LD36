#include "game.h"
#include "images.h"
#include "audio.h"

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


Game::Game():running(1), inMenu(1), level(0), paused(0), toLoadLevel(0), toEndLevel(0){
	menu = new Menu();
	QObject::connect(window, &Window::keyPressEv, menu, &Menu::keyPress);
	QObject::connect(window, &Window::keyReleaseEv, menu, &Menu::keyRelease);
	QObject::connect(window, &Window::keyPressEv, this, &Game::keyPress);
	QObject::connect(window, &Window::keyReleaseEv, this, &Game::keyRelease);

	//QObject::connect(audioPlayer, &QMediaPlayer::stateChanged, this, &Game::musicEnd);
}

void Game::run(){
	QTime *timing;
	timing = new QTime();
	timing->start();

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
	uint32_t x,y;
	painter->setPen(QColor(0xFFFFFF));

	//QFont font = painter->font();
	//font.setPointSize(FONT_SIZE);
	painter->setFont(font);



	painter->fillRect(0, 0, w, h, QColor(0xaaaaca));


	if(level){
		const int *mapText = level->getMapText();
		int minx, miny;
		int maxx, maxy;
		for(x=0;x<level->getW();++x){
			minx = x*32+(w-level->getW()*32)/2-16;
			for(y=0;y<level->getH();++y){
				miny = y*32+(h-level->getH()*32)/2-16;
				QImage *img = level->getImage(mapText[x+y*level->getW()]);
				painter->drawImage(QRect(minx, miny, 32, 32), *img);
			}
		}
	}
	if(inMenu || paused){
		menu->render(painter);
	}
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

void Game::keyPress(QKeyEvent *ev){
	int key = ev->key();
	if(level && !paused){
		if(key == Qt::Key_Escape){
			paused = 1;
			menu->setActive(1);
		}
	}
	if(key == Qt::Key_Q)playSound("audio/jump.wav");
	if(key == Qt::Key_R)playMusic("audio/jump.wav");
	if(key == Qt::Key_E)playMusic("audio/example.ogg");
}
void Game::keyRelease(QKeyEvent *ev){

}

#define MUSICS 2
QString musics[] = {"audio/jump.wav", "audio/example.ogg"};
void Game::musicEnd(QMediaPlayer::State state){
	if(state == QMediaPlayer::StoppedState){
		playMusic(musics[getRandInt(MUSICS)]);
	}
}



void Game::tick(){
	if(toEndLevel){
		delete level;
		level = 0;
		inMenu = 1;
		menu->setActive(1);
		toEndLevel = 0;
	}
	if(toLoadLevel){
		level = new Level();
		level->setup(levelName.toStdString().c_str());
		inMenu = 0;
		menu->setActive(0);
		toLoadLevel = 0;
	}
	if(toRestartLevel){
		delete level;
		level = new Level();
		level->setup(levelName.toStdString().c_str());
		toRestartLevel = 0;
	}

	if(inMenu || paused){
		menu->tick();
	}else if(level){
		level->tick();
	}
}
