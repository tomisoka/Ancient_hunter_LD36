#include "audio.h"

int volumeSound = 100;
int volumeMusic = 100;

QtAV::AVPlayer soundPlayer;
QtAV::AVPlayer musicPlayer;
//map <QString, QSoundEffect*> sounds;
//QMediaPlayer *audioPlayer;

void updateVolume(){
	/*map<QString, QSoundEffect*>::iterator it = sounds.begin();
	while(it!=sounds.end()){
		it->second->setVolume(volumeSound/100.f);
	}

	audioPlayer->setVolume(volumeMusic);*/
}

void playSound(QString name){
	/*map<QString, QSoundEffect*>::iterator it = sounds.find(name);
	QSoundEffect* sound;
	if(it==sounds.end()){
		sound = new QSoundEffect();
		sound->setSource(QUrl::fromLocalFile(name));
		sounds[name] = sound;
	}else sound = it->second;

	sound->play();*/
	//soundPlayer.play(name);
}

void playMusic(QString name){
	//audioPlayer->setMedia(QUrl::fromLocalFile(QDir::currentPath() + "/" + name));
	//audioPlayer->play();
	//musicPlayer.play(name);
}

void setup_audio(){
	//audioPlayer = new QMediaPlayer();
	//audioPlayer->setVolume(volumeMusic);
}

void cleanup_audio(){
	//delete audioPlayer;
}
