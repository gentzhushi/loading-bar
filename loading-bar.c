#include<stdint.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/ioctl.h>
#include<time.h>
#include<unistd.h>

#define CLEAR_CLR 	"\x1b[0m"
#define RED_CLR		"\x1b[31m"
#define GREEN_CLR	"\x1b[32m"

char* build_bar(size_t width, size_t breakpoint, char fill, char empty);
static void print_bar(const char *bar, size_t breakpoint, int use_color);

int main(void){

	int USE_COLOR = isatty(STDOUT_FILENO);
	struct timespec ts = { .tv_sec = 0, .tv_nsec = 20L * 1000 * 1000}; // 50ms
	struct winsize window;

	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &window) == -1){
		perror("ioctl");
		return 1;
	}
	
	int ROWS = window.ws_row;
	int	COLS = window.ws_col;

	printf("Terminal dimensions are: %dx%d\n", ROWS, COLS);
	
	fflush(stdout);

	for (int i = 1; i < COLS; i++){
		print_bar(build_bar((size_t)COLS, (size_t)i, '|', '.'), i, USE_COLOR);
		
		nanosleep(&ts, NULL);
		
		if (i == COLS-1){
			printf("\n");
			break;
		}
		fflush(stdout);
	}
	
	printf("U KRY BABO!");

	return 0;
}

char* build_bar(size_t width, size_t breakpoint, char fill, char empty){
	if (breakpoint < 1){
		printf("Returning null because breakpoint is less than 2. It is %zu.", breakpoint);
	   	return NULL;
	}

	if (width < 2){
		printf("Returning null because width is less than 2. It is %zu.", width);
		return NULL;
	}

	char *string = malloc(width+1); // +1 per EOS
	if (!string){
		printf("Returning null because !string. It is %s.", string);
		return NULL;
	}

	string[0] = '[';

	for (size_t i = 1; i < width - 1; i++){
		if (i <= breakpoint) string[i] = fill;
		else string[i] = empty;
	}	

	string[width-1] = ']';
	string[width] = '\0';

	return string;
}

static void print_bar(const char *bar, size_t breakpoint, int use_color) {

	// green
	if (use_color) fputs(GREEN_CLR, stdout);

	putchar('[');
	for (size_t j = 1; j < breakpoint;j++) putchar(bar[j]);

	// red
	if (use_color) fputs(RED_CLR, stdout);

	for (size_t j = 1 + breakpoint; bar[j] && bar[j] != ']'; j++) putchar(bar[j]);
	putchar(']');

	// clear
	if (use_color) fputs(CLEAR_CLR, stdout);

	putchar('\r');
}
