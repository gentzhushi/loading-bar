#include<stdint.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/ioctl.h>
#include<time.h>
#include<unistd.h>

#define FILL    '|'
#define EMPTY   '.'

char* build_bar(uint16_t width, uint16_t breakpoint, char fill, char empty);

int main(void)
{
	int USE_COLOR = isatty(STDOUT_FILENO);
	struct timespec ts = { .tv_sec = 0, .tv_nsec = 50L * 1000 * 1000};
	struct winsize window;

	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &window) == -1){
		perror("ioctl");
		return 1;
	}
	
	uint16_t ROWS = window.ws_row;
	uint16_t COLS = window.ws_col;

    char *bar = NULL;
	for (int i = 0; i < COLS; i++){
		bar = build_bar(COLS - 6, i, FILL, EMPTY);
		
		printf("\r %3d%% %s", 100 * i / (COLS - 1), bar);
		
		nanosleep(&ts, NULL);
		fflush(stdout);
	}
	
    free(bar);
	return 0;
}

char* build_bar(uint16_t width, uint16_t breakpoint, char fill, char empty)
{
	if (breakpoint < 1 || width < 2)
		return NULL;

	char *string = malloc(width+1);

	if (!string){
        perror("malloc:");
		return NULL;
	}

	for (size_t i = 0; i < width; i++)
		if (i <= breakpoint)
            string[i] = fill;
		else
            string[i] = empty;

	string[width] = '\0';

	return string;
}
