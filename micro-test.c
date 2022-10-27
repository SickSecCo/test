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


int ft_strlen(char const *str)
{
	int i = -1;
	while (str[++i]);
	return i;
}

int put_error(char const *str)
{
	if (str)
		write(1, str, ft_strlen(str));
	return 1;
}

int clear_all(FILE *file, char *paint_table, char *str)
{
	if (file)
		fclose(file);
	if (paint_table)
		free(paint_table);
	if (str)
		put_error(str);
	return 1;
}

int check_zone(t_zone *zone)
{return (zone->width > 0 && zone->width <= 300 && zone->height > 0 && zone->height <= 300);}

int check_shape(t_shape *shape)
{return (shape->width > 0.00000000 && shape->height > 0.00000000 && (shape->type == 'r' || shape->type == 'R'));}

char *take_params(FILE *file, t_zone *zone)
{
	int scan_return;
	if ((scan_return = fscanf(file, "%d %d %c\n", &zone->width, &zone->height, &zone->background) != 3))
		return NULL;
	if (!check_zone(zone))
		return NULL;
	if (scan_return == -1)
		return NULL;
	char *temp;
	int k = zone->width * zone->height;
	int i = -1;
	if (!(temp = malloc(sizeof(char) * k)))
		return NULL;
	while (++i < k)
		temp[i] = zone->background;
	return temp;
}

int is_in(t_shape *shape, float x, float y)
{
	if (x < shape->x || x > shape->x + shape->width || y < shape->y || y > shape->y + shape->height)
		return 0;
	if (x < shape->x + 1.00000000 || x > shape->x + shape->width + 1.00000000 || y < shape->y + 1.00000000 || y > shape->y + shape->height + 1.00000000)
		return 2;
	return 1;
}

int draw_shape(t_shape *shape, t_zone *zone, char *paint_table)
{
	int j = -1, i, check;
	while (++j < zone->height)
	{
		i = -1;
		while(++i < zone->width)
		{
			check = is_in(shape, i, j);
			if ((shape->type == 'r' && check == 2) || (shape->type == 'R' && check))
				paint_table[j * zone->width + i] = shape->character;
		}
	}
}

int draw__shapes(FILE *file, t_zone *zone, char *paint_table)
{
	t_shape *shape;
	int scan_return;
	while((scan_return = fscanf(file, "%c %f %f %f %c\n", &shape->type, &shape->width, &shape->height, &shape->x, &shape->y, &shape->character)) == 6)
	{
		if (!check_shape(shape))
			return 0;
		draw_shape(shape, zone, paint_table);
	}
	if (scan_return == -1)
		return 0;
	return 1;
}

void draw_(char *paint_table, t_zone *zone)
{
	int i = -1;
	while (++i < zone->height)
	{
		write(1, paint_table + i * zone->width, zone->width);
		write(1, "\n", 1);
	}
}


int main(int argc, char **argv)
{
	FILE *file;
	t_zone zone;
	char *paint_table;
	
	zone.width = 0;
	zone.height = 0;
	zone.background = 0;
	if (argc != 2)
		return(put_error("Error: Argument\n"));
	if (!(file = fopen(argv[1], 'r')))
		return (put_error("Error: Operation file corrupted\n"));
	if (!(paint_table = take_params(file, &zone)))
		return (clear_all(file, NULL, "Error: Operation file corrupted\n"));
	if (!draw__shapes(file, &zone, paint_table))
		return (clear_all(file, paint_table, "Error: Operation file corrupted\n"));
	draw_(paint_table, &zone);
	clear_all(file, paint_table, NULL);
	return 0;
}
