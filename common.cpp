#include "common.h"
#include "window.h"
#include "game.h"
#include "render.h"
#include "images.h"
#include "menu.h"
#include "audio.h"
#include "unlock.h"
#include "trader.h"

QFont font;
QFontMetrics fontMet(font);


QGuiApplication *app;

bool strToDouble(const char * pos, double& returned, uint32_t len){
	uint32_t i=0;
	uint8_t is_dot=0;
	double multiplier = 1;
	returned = 0;

	if(*pos=='-')i++;

	for(; i < len; i++){
		if(pos[i]<=0x20)break;
		else if(pos[i] == '.' || pos[i] == ','){
			if(is_dot)
				return 1;
			else
				is_dot = 1;
		}else if(!is_dot){
			if(pos[i] < '0' || pos[i] > '9')return 1;
			returned *= 10;
			returned += pos[i] - '0';
		}else{
			if(pos[i] < '0' || pos[i] > '9')return 1;
			multiplier /= 10;
			returned += multiplier*(pos[i]-'0');
		}
	}
	if(*pos=='-')returned*=-1;
	return 0;
}

bool strToInt(const char *pos, int& returned, uint32_t len, int32_t base){
	uint32_t i=0;
	returned=0;
	char temp;

	if(base>26*2+10)return 1;

	if(*pos=='-')i++;
	for(; i<len;++i){
		temp = pos[i];
		if(temp<=0x20)break;
		returned*=base;

		if(temp>='0' && temp<='9'){
			temp-='0';
			if(base<=temp)return 1;
			returned+=temp;
		}else if(temp>='A' && temp <= 'Z'){
			temp = temp-'A'+10;
			if(base<=temp)return 1;
			returned+=temp;
		}else if(temp>='a' && temp <= 'z'){
			temp = temp-'a'+36;
			if(base<=temp)return 1;
			returned+=temp;
		}else return 1;
	}
	if(*pos=='-')returned*=-1;
	return 0;
}

char *numStr(char *s, uint32_t num){
	uint32_t i=1;

	if(!num){
		*s++ = '0';
		return s;
	}

	while(i<=num)i*=10;
	i/=10;

	while(i){
		*s = num/i+'0';
		s++;
		num %= i;
		i/=10;
	}

	return s;
}

char *doubleStr(char *s, double n, uint32_t afterpoint){
	if(n<0){
		*s++ = '-';
		n=-n;
	}
	int i;
	int ipart = (int)n;
 
	double fpart = n - (double)ipart;
 
 
	if (afterpoint){
		i = (int)pow(10, afterpoint-1);

		while(afterpoint--){
			fpart*=10;
		}

		int fp = (int) round(fpart);

		if(fp/i==10){
			ipart++;
			fp=0;
		}

		s = numStr(s, ipart);

		if(fp){
			*s++ = '.';
			while(i && fp){
				*s++ = fp/i+'0';
				fp %= i;
				i/=10;
			}
		}
	}else
		s = numStr(s, ipart);
	return s;
}


void setup(int argc, char **argv){
	app = new QGuiApplication(argc, argv);

	int fontID = QFontDatabase::addApplicationFont("fonts/OpenSans.ttf");
	QString fontFamily = QFontDatabase::applicationFontFamilies(fontID).at(0);
	font = QFont(fontFamily);
	fontMet = QFontMetrics(font);
	font.setPointSize(FONT_SIZE);

	setup_window();
	setup_render();
	setup_audio();
	setup_unlock();
	setup_trader();
	setup_game();
}

void cleanup(){
	cleanup_game();
	cleanup_unlock();
	cleanup_audio();
	cleanup_render();
	cleanup_window();
	cleanup_images();
	delete app;
}
