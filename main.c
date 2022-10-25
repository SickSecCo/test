 #include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct s_zone
{
	int w;
	int h;
	char b;
} t_zone;

typedef	struct s_circle
{
	char type;
	float x;
	float y;
	float radius;
	char c;
}	t_circle;

int main(int argc, char **argv)
{
	FILE *f;
	t_zone zone;
	t_circle circle;
	int scan;
	int i;
	int j;
	char **board;

	if (argc != 2)
	{
		write(1, "Error: argument\n", 16);
		return (0);
	}
	f = fopen(argv[1], "r");
	if (!f)
	{
		write(1, "Error: Operation file corrupted\n", 32);
		return (0);
	}
	if (fscanf(f, "%i %i %c\n", &zone.w, &zone.h, &zone.b) != 3)
	{
		write(1, "Error: Operation file corrupted\n", 32);
		return (0);
	}
	if (zone.h < 1 || zone.h > 300 || zone.w < 1 || zone.w > 300)
	{
		write(1, "Error: Operation file corrupted\n", 32);
		return (0);
	}
	board = malloc(sizeof(char *) * (zone.h));
	if (!board)
	{
		write(1, "Error: Operation file corrupted\n", 32);
		return (0);
	}
	i = 0;
	while (i < zone.h)
	{
		board[i] = malloc(sizeof(char) * zone.w);
		if (!board[i])
		{
			write(1, "Error: Operation file corrupted\n", 32);
			return (1);
		}
		j = 0;
		while (j < zone.w)
		{
			board[i][j] = zone.b;
			j++;
		}
		i++;
	}
	scan = fscanf(f, "%c %f %f %f %c\n", &circle.type, &circle.x, &circle.y, &circle.radius, &circle.c);
	while (scan == 5)
	{
		i = 0;
		if (circle.type != 'c' && circle.type != 'C')
		{
			write(1, "Error: Operation file corrupted\n", 32);
			return (0);
		}
		if (circle.radius <= 0)
		{
			write(1, "Error: Operation file corrupted\n", 32);
			return (0);
		}
		while (i < zone.h)
		{
			j = 0;
			while (j < zone.w)
			{
				if (circle.type == 'c')
				{
					if (sqrt((j - circle.x) * (j - circle.x) + (i - circle.y) * (i - circle.y)) - circle.radius <= 0.0 &&
						sqrt(((float)j - circle.x) * ((float)j - circle.x) + ((float)i - circle.y) * ((float)i - circle.y)) - circle.radius > -1.0)
						board[i][j] = circle.c;
				}
				else if (circle.type == 'C')
				{
					if (sqrt((j - circle.x) * (j - circle.x) + (i - circle.y) * (i - circle.y)) - circle.radius <= 0.0)
						board[i][j] = circle.c;
				}
				j++;
			}
			i++;
		}
		scan = fscanf(f, "%c %f %f %f %c\n", &circle.type, &circle.x, &circle.y, &circle.radius, &circle.c);
	}
	if (scan != -1)
	{
		write(1, "Error: Operation file corrupted\n", 32);
		return (0);
	}
	i = 0;
	while (i < zone.h)
	{
		j = 0;
		while (j < zone.w)
		{
			write(1, board[i] + j, 1);
			j++;
		}
		free(board[i]);
		write(1, "\n", 1);
		i++;
	}
	free(board);
	return (0);
}
