#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_STACK_SIZE 100
#define MAZE_SIZE 10


typedef struct {
	short r;
	short c;
} element;

typedef struct {
	element data[MAX_STACK_SIZE];
	int top;
}StackType;

void init_stack(StackType* s) {
	s->top = -1;
}

int is_empty(StackType* s) {
	return(s->top == -1);
}

int is_full(StackType* s) {
	return(s->top == (MAX_STACK_SIZE - 1));
}

void push(StackType* s, element item) {
	if (is_full(s)) {
		fprintf(stderr, "Stack Full!\n");
		return;
	}
	else s->data[++(s->top)] = item;
}

element pop(StackType* s) {
	if (is_empty(s)) {
		fprintf(stderr, "Stack Empty!\n\n");
		return;
	}
	else return s->data[(s->top)--];
}

element peek(StackType* s) {
	if (is_empty(s)) {
		fprintf(stderr, "Stack Empty!\n\n");
		return;
	}
	else return s->data[(s->top)];
}

element here = { 1,0 }, entry = { 1,0 };

char maze[MAZE_SIZE][MAZE_SIZE] = {
		{'1','1','1','1','1','1','1','1','1','1'},
		{'e','1','0','1','0','0','0','1','0','1'},
		{'0','0','0','1','0','0','0','1','0','1'},
		{'0','1','0','0','0','1','1','0','0','1'},
		{'1','0','0','0','1','0','0','0','0','1'},
		{'1','0','0','0','1','0','0','0','0','1'},
		{'1','0','0','0','0','0','1','0','1','1'},
		{'1','0','1','1','1','0','1','1','0','1'},
		{'1','1','0','0','0','0','0','0','0','x'},
		{'1','1','1','1','1','1','1','1','1','1'}
};

int back_count = 0;
element prev = { -1, -1 };

void push_loc(StackType* s, int r, int c) {
	if (r < 0 || c < 0) return;
	if (maze[r][c] != '1' && maze[r][c] != '.') {
		element tmp;
		tmp.r = r;
		tmp.c = c;
		push(s, tmp);
	}
}

void maze_print(char maze[MAZE_SIZE][MAZE_SIZE]) {
	for (int r = 0; r < MAZE_SIZE; r++) {
		for (int c = 0; c < MAZE_SIZE; c++) {
			printf("%c", maze[r][c]);
		}
		printf("\n");
	}
	printf("\n");
}

void route_print(StackType* s) {
	printf("Route: ");
	for (int i = 0; i <= s->top; i++) {
		printf("(%d,%d) ", s->data[i].r, s->data[i].c);
	}
	printf("\n");
}

int main(void) {
	int r, c;
	StackType s, route;
	init_stack(&s);
	init_stack(&route);
	here = entry;

	while (maze[here.r][here.c] != 'x') {
		r = here.r;
		c = here.c;
		maze[r][c] = '.';
		maze_print(maze);
		push_loc(&s, r - 1, c);
		push_loc(&s, r + 1, c);
		push_loc(&s, r, c - 1);
		push_loc(&s, r, c + 1);

		if (is_empty(&s)) {
			printf("Fail\n");
			return 0;
		}
		else {
			element prev_peek = peek(&s);
			here = pop(&s);

			if (!is_empty(&s))
				prev_peek = peek(&s);

			if (prev.r == prev_peek.r && prev.c == prev_peek.c) {
				back_count++;
				pop(&route);
			}
			else if (maze[here.r][here.c] == '.' || maze[here.r][here.c] == '1') {
				back_count++;
				pop(&route);
			}
			else {
				if (!is_empty(&route)) {
					element route_top = peek(&route);
					while (here.r != route_top.r && here.c != route_top.c && !is_empty(&route)) {
						pop(&route);
						if (!is_empty(&route))
							route_top = peek(&route);
					}
				}
				push(&route, here);
				prev = here;
			}

		}


	}
	printf("Success!\n");
	printf("\nBack count: %d\n\n", back_count);
	route_print(&route);

	return 0;
}