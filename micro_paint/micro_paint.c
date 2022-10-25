#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

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

int	ft_strlen(char const *str)
{
	int i = 0;
	while (str[i])
		i++;
	return i;
}

int print_error(char const *str, int return_value)
{
	write(1, str, ft_strlen(str));
	return return_value;
}

int clear_all(FILE *file, char *painting_table)
{
	fclose(file);
	if (painting_table)
		free(painting_table);
	return 1;
}

int check_zone(t_zone *zone)
{return (zone->width > 0 && zone->width <= 300 && zone->height > 0 && zone->height <= 300);}

int check_shape(t_shape *shape)
{return (shape->width > 0.00000000 && shape->height > 0.00000000 && (shape->type == 'r' || shape->type == 'R'));}

int take_params(FILE *file, t_zone *zone)
{
	int scan_result;
	if ((scan_result = fscanf(file, "%d %d %c\n", &zone->width, &zone->height, &zone->background)) != 3)
		return 0;
	if (!check_zone(zone))
		return 0;
	if (scan_result == -1)
		return 0;
	return 1;
}

char *paint_background(t_zone *zone)
{
	char *painting_table;
	int i = -1;
	int zone_area = zone->height * zone->width;
	if (!(painting_table = malloc(sizeof(char) * zone_area)))
		return NULL;
	while (++i < zone_area)
		painting_table[i] = zone->background;
	return painting_table;
}

int check_if_inside(t_shape *shape, float x, float y)
{
	if (((x < shape->x || shape->x + shape->width < x) || y < shape->y) || shape->y + shape->height < y)
		return 0;
	if ((x - shape->x < 1.00000000 || shape->x + shape->width - x < 1.00000000) || (y - shape->y < 1.00000000 || shape->y + shape->height - y < 1.00000000))
		return 2;
	return 1;
}

void draw_shape(char **painting_table, t_shape *shape, t_zone *zone)
{
	int i = -1, j, check;
	while (++i < zone->height)
	{
		j = -1;
		while (++j < zone->width)
		{
			check = check_if_inside(shape, j, i);
			if ((shape->type == 'r' && check == 2) || (shape->type == 'R' && check))
				(*painting_table)[i * zone->width + j] = shape->character;
		}
	}
}

int draw__shapes(FILE *file, char **painting_table, t_zone *zone)
{
	t_shape temp;
	int scan_return;
	while ((scan_return = fscanf(file, "%c %f %f %f %f %c\n", &temp.type, &temp.x, &temp.y, &temp.width, &temp.height, &temp.character)) == 6)
	{
		if (!check_shape(&temp))
			return 0;
		draw_shape(painting_table, &temp, zone);
	}
	if (scan_return != -1)
		return 0;
	return 1;
}

void draw_(char *painting_table, t_zone *zone)
{
	int i = -1;
	while (++i < zone->height)
	{
		write(1, painting_table + i * zone->width, zone->width);
		write(1, "\n", 1);
	}
}

int main(int argc, char **argv)
{
	t_zone zone;
	char *painting_table;
	FILE *file;

	zone.width = 0;
	zone.height = 0;
	zone.background = 0;
	if (argc != 2)
		return (print_error("Error: argument\n", 1));
	if (!(file = fopen(argv[1], "r")))
		return (print_error("Error: Operation file corrupted\n", 1));
	if (!take_params(file, &zone))
		return (clear_all(file, painting_table) && print_error("Error: Operation file corrupted\n", 1));
	if (!(painting_table = paint_background(&zone)))
		return (clear_all(file, painting_table) && print_error("Error: malloc failed ;( \n", 1));
	if (!draw__shapes(file, &painting_table, &zone))
		return (clear_all(file, painting_table) && print_error("Error: Operation file corrupted\n", 1));
	draw_(painting_table, &zone);
	clear_all(file, painting_table);
	return 0;
}


