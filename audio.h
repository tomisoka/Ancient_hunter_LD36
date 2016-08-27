#pragma once
#include "common.h"

extern int volumeSound;
extern int volumeMusic;
//extern QMediaPlayer *audioPlayer;

void updateVolume();

void playSound(QString name);
void playMusic(QString name);


void setup_audio();
void cleanup_audio();
