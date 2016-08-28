#include "audio.h"

int volumeSound = 100;
int volumeMusic = 100;

//QtAV::AVPlayer soundPlayer;
//QtAV::AVPlayer musicPlayer;
//map <QString, QMediaPlayer*> sounds;
//map <QString, QMediaPlayer*> musics;
QMediaPlayer *audioPlayer;
QMediaPlayer *soundPlayer;

void updateVolume(){
	/*map<QString, QMediaPlayer*>::iterator it = sounds.begin();
	while(it!=sounds.end()){
		it->second->setVolume(volumeSound);
	}
	it = musics.begin();
	while(it!=musics.end()){
		it->second->setVolume(volumeSound);
	}*/

	soundPlayer->setVolume(volumeSound);
	audioPlayer->setVolume(volumeMusic);
}

void playSound(QString name){
	/*map<QString, QMediaPlayer*>::iterator it = sounds.find(name);
	QMediaPlayer* sound;
	if(it==sounds.end()){
		sound = new QMediaPlayer();
		sound->setMedia(QUrl::fromLocalFile(QDir::currentPath() + "/" + name));
		sounds[name] = sound;
		sound->setVolume(volumeSound);
	}else sound = it->second;

	sound->play();*/
	//printf("?? %s\n", name.toStdString().c_str());
	if(soundPlayer->state() != QMediaPlayer::StoppedState)soundPlayer->stop();
	soundPlayer->setMedia(QUrl::fromLocalFile(QDir::currentPath() + "/" + name));
	soundPlayer->play();
	//soundPlayer.play(name);
}

void playMusic(QString name){
	/*map<QString, QMediaPlayer*>::iterator it = musics.find(name);
	QMediaPlayer* music;
	if(it==musics.end()){
		music = new QMediaPlayer();
		music->setMedia(QUrl::fromLocalFile(QDir::currentPath() + "/" + name));
		musics[name] = music;
		music->setVolume(volumeMusic);
	}else music = it->second;

	music->play();*/


	if(audioPlayer->state() != QMediaPlayer::StoppedState)audioPlayer->stop();
	audioPlayer->setMedia(QUrl::fromLocalFile(QDir::currentPath() + "/" + name));
	audioPlayer->play();
	//musicPlayer.play(name);
}

void setup_audio(){
	xml_document doc;
	if(doc.load_file((char *)"audio_settings")){
		xml_node sound = doc.child("sound");
		if(sound){
			xml_attribute volS = sound.attribute("volume");
			volumeSound = volS.as_int();
		}

		xml_node music = doc.child("music");
		if(music){
			xml_attribute volM = music.attribute("volume");
			volumeMusic = volM.as_int();
		}
	}

	audioPlayer = new QMediaPlayer();
	audioPlayer->setVolume(volumeMusic);
	soundPlayer = new QMediaPlayer();
	soundPlayer->setVolume(volumeSound);
}

void cleanup_audio(){
	/*map<QString, QMediaPlayer*>::iterator it = sounds.begin();
	while(it!=sounds.end()){
		delete it->second;
		it++;
	}

	it = musics.begin();
	while(it!=musics.end()){
		delete it->second;
		it++;
	}*/


	delete audioPlayer;
	delete soundPlayer;

	xml_document doc;
	xml_node sound = doc.append_child("sound");
	xml_attribute volS = sound.append_attribute("volume");
	volS.set_value(volumeSound);

	xml_node music = doc.append_child("music");
	xml_attribute volM = music.append_attribute("volume");
	volM.set_value(volumeMusic);

	doc.save_file((char*)"audio_settings");
}
