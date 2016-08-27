#include "common.h"
#include "game.h"
#include <time.h>

int main(int argc, char **argv){
	srand(time(0));

	setup(argc, argv);

	int err;
	if((err = app->exec())){
		fprintf(stderr, "Error(%d) while executing Qt5!\n", err);
		return err;
	}

	cleanup();

	return 0;
}
