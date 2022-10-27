#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

typedef struct s_zone
{
	int width;
	int height;
	char background;
}	t_zone;

typedef struct s_shape
{
	float x;
	float y;
	float width;
	float height;
	char type;
	char character;
}	t_shape;


int ft_strlen(const char *str)
{
	int i = -1;
	while (str[++i]);
	return i;
}

int put_error(const char *str)
{
	if (str)
		write(1, str, ft_strlen(str));
	return 1;
}

int clear_all(FILE *file, char *arr, char *str)
{
	if (file)
		fclose(file);
	if (arr)
		free(arr);
	if (str)
		put_error(str);
	return 1;
}

int check_shape(t_shape *shape)
{return (shape->width > 0.00000000 && shape->height > 0.00000000 && (shape->type == 'r' || shape->type == 'R'));}

int check_zone(t_zone *zone)
{return (zone->width > 0 && zone->width <= 300 && zone->height > 0 && zone->height <= 300);}

int check_if_inside(t_shape *shape, float x, float y)
{
	if (x < shape->x || x > shape->x + shape->y || y < shape->y || y > shape->y + shape->height)
		return 0;
	if (x < shape->x + 1.00000000 || x > shape->x + shape->y - 1.00000000 || y < shape->y + 1.00000000 || y > shape->y + shape->height - 1.00000000)
		return 2;
	return 1;
}

void draw_(char *arr, t_zone *zone)
{
	int z = -1;
	while (++z < zone->height)
	{
		write (1, arr[z * zone->width], zone->width);
		write (1, "\n", 1);
	}
}

int draw__shapes(FILE *file, t_zone *zone, char *arr)
{
	int scan_res;
	t_shape shape;
	while ((scan_res = fscanf(file, "%c %f %f %f %f %c\n", &shape.type, &shape.width, &shape.height, &shape.x, &shape.y, &shape.character)) == 6)
	{
		if (!check_shape(&shape))
			return 0;
		draw_shape(arr, zone, &shape);
	}
	if (scan_res != -1)
		return 1;
	return 0;
}


int draw_shape(char *arr, t_zone *zone, t_shape *shape)
{
	int y = -1, x, check;
	while (++y < zone->height)
	{
		x = -1;
		while (++x < zone->width)
		{
			check = check_if_inside(shape, x, y);
			if ((shape->type == 'r' && check == 2) || (shape->type == 'R' && check))
				arr[y * zone->width + x] = shape->character;
		}
	}
}

char *take_params(FILE *file, t_zone *zone)
{
	if (fscanf(file, "%d %d %c\n", &zone->width, &zone->height, &zone->background) != 3)
		return NULL;
	if (!check_zone(zone))
		return NULL;
	int k = zone->width * zone->height;
	char *temp;
	temp = malloc(sizeof(char) * k);
	int i = -1;
	while (++i < k)
		temp[i] = zone->background;
	return temp;
}

int main(int argc, char **argv)
{
	FILE *file;
	char *arr;
	t_zone zone;
	zone.width = 0;
	zone.height = 0;
	zone.background = 0;
	if (argc != 2)
		return(put_error("Error: argument\n"));
	if (!(file = fopen(argv[1], "r")))
		return (put_error("Error: Operation file corrupted\n"));
	if (!(arr = take_params(file, &zone)))
		return (clear_all(file, NULL, "Error: Operation file corrupted\n"));
	if (!draw__shapes(file, &zone, arr))
		return (clear_all(file, arr, "Error: Operation file corrupted\n"));
	draw_(arr, &zone);
	clear_all(file, arr, NULL);
	return 0;
}