#include "menu.h"
#include "game.h"

#define menuOptionLen 3 
QString menuOption[menuOptionLen] = {"Play!"/*, "Options"*/, "Info", "Exit"};
int menuOptionMaxLen = 0;

int WArr;
int HArr;
int offArr = 8;



//#define OptionsOptionLen 3
//QString OptionsOption[OptionsOptionLen] = {"Sounds:", "Music: (not added)", "Exit"};
//int OptionsOptionMaxLen = 0;



#define inGameOptionLen 3
QString inGameOption[inGameOptionLen] = {"Continue", "Restart", "Exit"};
int inGameOptionMaxLen = 0;



QString infoMenu =
"Hunt animals, collect food and upgrade!\n\
\n\
Controls:\n\
WASD/arrows - navigation in menu, movement in game.\n\
F - trade with trader.\n\
Space - attack.\n\
Ctrl - sprint.\n\
QE - change weapon (if unlocked).\n\
Enter - choose.\n\
\n\
Entrance is on the left, Exit is on the right between two rocks.\n\
\n\
This game was created for Ludum Dare 36 by Tomisoka in C++ with libraries {Qt5, pugixml} and with help of these tools: {Gimp, Tiled, Sfxr} + other smaller tools such as Vim.\n\
\n\
> Exit <";

Menu::Menu():inGameMenu(0), chosen(0), info(0), inOptions(0), levelChoose(0), yLevelOffset(0), active(1){
	uint32_t i;

	int max = 0;
	int temp;
	for(i=0;i<menuOptionLen;++i){
		temp = fontMet.width(menuOption[i]);
		if(temp > max)max = temp;
	}
	menuOptionMaxLen = max;


	WArr = fontMet.width(QString(">"));
	HArr = fontMet.height();


	/*max = 0;
	for(i=0;i<OptionsOptionLen;++i){
		temp = fontMet.width(OptionsOption[i]);
		if(temp > max)max = temp;
	}
	OptionsOptionMaxLen = max+16;*/


	max = 0;
	for(i=0;i<inGameOptionLen;++i){
		temp = fontMet.width(inGameOption[i]);
		if(temp > max)max = temp;
	}
	inGameOptionMaxLen = max;
}


void Menu::keyPress(QKeyEvent *ev){
	if(!active)return;
	uint32_t i;
	int options;

	int key = ev->key();

	if(inGameMenu){
		options = inGameOptionLen;
		if(key == Qt::Key_W || key == Qt::Key_Up){
			if(chosen)chosen--;
			if(chosen == 1 && !game->isGameEnded() && !game->isDeadPlayer())chosen--;
		}else if(key == Qt::Key_S || key == Qt::Key_Down){
			if(chosen<options-1)chosen++;
			if(chosen == 1 && !game->isGameEnded() && game->isDeadPlayer())chosen++;
		}else if(key == Qt::Key_Escape){
			game->unpause();
			chosen = 0;
		}else if(key == Qt::Key_Enter || key == Qt::Key_Return){
			if(chosen == 0 && !game->isDeadPlayer()){
				game->unpause();
				if(game->isGameEnded())game->restartLevel();
			}else if(chosen == 1){
				game->unpause();
				game->restartLevel();
				chosen = 0;
			}else if(chosen == 2){
				game->unpause();
				game->endLevel();
				inGameMenu = 0;
				chosen = 0;
			}
		}

	}else{



		if(levelChoose){



			options = levelNames.size();
			if(key == Qt::Key_W || key == Qt::Key_Up){
				if(chosen)chosen--;
			}else if(key == Qt::Key_S || key == Qt::Key_Down){
				if(chosen<options-1)chosen++;
			}else if(key == Qt::Key_Enter || key == Qt::Key_Return){
				if(chosen != 0){
					game->loadLevel(("maps/"+levelNames[chosen]+".tmx"));
					inGameMenu = 1;
				}
				chosen = 0;
				levelChoose=0;
			}else if(key == Qt::Key_Escape){
				chosen = 0;
				levelChoose=0;
			}else if(1);



		}/*else if(inOptions){



			options = OptionsOptionLen;

			if(key == Qt::Key_A || key == Qt::Key_Left){
				if(chosen==0)if(volumeSound)volumeSound--;
				if(chosen==1)if(volumeMusic)volumeMusic--;
			}else if(key == Qt::Key_D || key == Qt::Key_Right){
				if(chosen==0)if(volumeSound<100)volumeSound++;
				if(chosen==1)if(volumeMusic<100)volumeMusic++;
			}else if(key == Qt::Key_W || key == Qt::Key_Up){
				if(chosen)chosen--;
			}else if(key == Qt::Key_S || key == Qt::Key_Down){
				if(chosen<options-1)chosen++;
			}else if(key == Qt::Key_Enter || key == Qt::Key_Return){
				if(chosen == 2){
					updateVolume();
					inOptions = 0;
					chosen = 0;
				}
			}else if(key == Qt::Key_Escape){
				inOptions = 0;
				chosen = 0;
			}



		}*/else if(info){



			if(key == Qt::Key_Enter || key == Qt::Key_Return || key == Qt::Key_Escape){
				info = 0;
				chosen = 0;
			}



		}else{



			options = menuOptionLen;
			if(key == Qt::Key_W || key == Qt::Key_Up){
				if(chosen)chosen--;
			}else if(key == Qt::Key_S || key == Qt::Key_Down){
				if(chosen<options-1)chosen++;
			}else if(key == Qt::Key_Enter || key == Qt::Key_Return){
				if(chosen == 0){
					game->loadLevel("");
					inGameMenu = 1;
				}/*else if(chosen == 1){
					inOptions = 1;
					chosen = 0;
				}*/else if(chosen == 1){
					info = 1;
					chosen = 0;
				}else if(chosen == 2){
					window->close();
				}
			}else if(key == Qt::Key_L){
				if(chosen==0){
					levelChoose = 1;
					yLevelOffset = 0;
					levelNames.clear();
					levelNames.push_back("Exit level selection");
					QDir dir("maps");

					QFileInfoList files = dir.entryInfoList();

					for(i=0;i<files.size();++i){
						QString suffix = files[i].suffix();
						if(!files[i].isFile() || suffix != "tmx")continue;
						QString name = files[i].fileName();
						levelNames.push_back(name.mid(0, name.size() - suffix.size()-1));
					}
				}
			}
		}
	}
}

void Menu::keyRelease(QKeyEvent *ev){
	if(!active)return;

}



void Menu::tick(){
	if(!active)return;

}


void Menu::render(QPainter *painter){
	if(!active)return;

	uint32_t i;
	int yOffset;
	int yTextDiff = FONT_SIZE+12;
	int options;

	if(inGameMenu){
		options = inGameOptionLen;
		yOffset = (game->getH()-options*yTextDiff)/2;

		if(game->isGameEnded())painter->drawText(QRect(0, 0, game->getW(), 48), Qt::AlignHCenter, endGameMessage);

		for(i=0;i<options;++i){
			if(i!=1 || !game->isGameEnded() || game->isDeadPlayer())painter->drawText(0,yOffset+i*yTextDiff, game->getW(), FONT_SIZE*2, Qt::AlignHCenter, inGameOption[i]);
		}

		painter->drawText((game->getW()/2 - inGameOptionMaxLen)  - offArr - WArr*2, yOffset+chosen*yTextDiff, WArr*2, HArr*2, Qt::AlignHCenter, ">");
		painter->drawText((game->getW()/2 + inGameOptionMaxLen)  + offArr, yOffset+chosen*yTextDiff, WArr*2, HArr*2, Qt::AlignHCenter, "<");
	}else{
		if(levelChoose){
			options = levelNames.size();

			int sumArr = yLevelOffset+32+chosen*yTextDiff;
			if(sumArr > game->getH()-32)yLevelOffset -= sumArr - (game->getH()-32);
			if(sumArr < 32+HArr)yLevelOffset += 32+HArr - sumArr;

			for(i=0;i<options;++i){
				painter->drawText(48,yLevelOffset+i*yTextDiff, game->getW(), FONT_SIZE*2, 0, levelNames[i]);
			}
			painter->drawText(16,yLevelOffset+chosen*yTextDiff, game->getW(), FONT_SIZE*2, 0, ">");


		}/*else if(inOptions){
			options = OptionsOptionLen;
			yOffset = (game->getH()-options*yTextDiff)/2;
			static int yDiff[3]={-32,0,32};

			for(i=0;i<options;++i){
				painter->drawText(0,yOffset+yDiff[i]+i*yTextDiff, game->getW(), FONT_SIZE*2, Qt::AlignHCenter, OptionsOption[i]);
			}

			painter->drawText((game->getW()/2 - OptionsOptionMaxLen)  - offArr - WArr*2, yDiff[chosen]+yOffset+chosen*yTextDiff, WArr*2, HArr*2, Qt::AlignHCenter, ">");
			painter->drawText((game->getW()/2 + OptionsOptionMaxLen)  + offArr, yDiff[chosen]+yOffset+chosen*yTextDiff, WArr*2, HArr*2, Qt::AlignHCenter, "<");


			painter->drawRect(QRect(game->getW()/2-OptionsOptionMaxLen, yDiff[0]+yOffset+1*yTextDiff+16+2, OptionsOptionMaxLen*2,4));
			painter->fillRect(QRect(game->getW()/2-OptionsOptionMaxLen, yDiff[0]+yOffset+1*yTextDiff+16+2, (OptionsOptionMaxLen*2)*volumeSound/100,4), QColor(0xFFFFFF));

			painter->drawRect(QRect(game->getW()/2-OptionsOptionMaxLen, yDiff[1]+yOffset+2*yTextDiff+16+2, OptionsOptionMaxLen*2,4));
			painter->fillRect(QRect(game->getW()/2-OptionsOptionMaxLen, yDiff[1]+yOffset+2*yTextDiff+16+2, (OptionsOptionMaxLen*2)*volumeMusic/100,4), QColor(0xFFFFFF));


		}*/else if(info){
			font.setPointSize(FONT_SIZE/2);
			painter->setFont(font);

			painter->drawText(32,yOffset, game->getW()-64, game->getH()-yOffset, Qt::AlignCenter | Qt::TextWordWrap, infoMenu);

			font.setPointSize(FONT_SIZE);
			painter->setFont(font);
		}else{
			options = menuOptionLen;
			yOffset = (game->getH()-options*yTextDiff)/2;

			font.setPointSize(FONT_SIZE*3/2);
			painter->setFont(font);
			painter->drawText(0,yOffset-3*yTextDiff, game->getW(), FONT_SIZE*4, Qt::AlignHCenter, "Ancient Hunter");
			font.setPointSize(FONT_SIZE);
			painter->setFont(font);


			for(i=0;i<options;++i){
				painter->drawText(0,yOffset+i*yTextDiff, game->getW(), FONT_SIZE*2, Qt::AlignHCenter, menuOption[i]);
			}

			painter->drawText((game->getW()/2 - menuOptionMaxLen)  - offArr - WArr*2, yOffset+chosen*yTextDiff, WArr*2, HArr*2, Qt::AlignHCenter, ">");
			painter->drawText((game->getW()/2 + menuOptionMaxLen)  + offArr, yOffset+chosen*yTextDiff, WArr*2, HArr*2, Qt::AlignHCenter, "<");
		}
	}
}
