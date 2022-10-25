#include <stdlib.h>
#include <stdio.h>
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
	float radius;
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

char *take_params_and_zone(FILE *file, t_zone *zone)
{
	int i = -1;
	char *temp;
	if ((fscanf(file, "%d %d %c\n", &zone->width, &zone->height, &zone->background)) != 3)
		return NULL;
	if (zone->width <= 0 || zone->width > 300 || zone->height <= 0 || zone->height > 300)
		return NULL;
	int k = zone->width * zone->height;
	if (!(temp = malloc(sizeof(char) * k)))
		return NULL;
	while (++i < k)
		temp[i] = zone->background;
	return temp;
}

int	is_in_circle(float x, float y, t_shape *shape)
{
	float dist;
	dist = sqrtf(powf(x - shape->x, 2.00) + powf(y - shape->y, 2.00));
	if (dist <= shape->radius)
	{
		if (shape->radius - dist < 1.00000000)
			return 2;
		return 1;
	}
	return 0;
}

void draw_shape(t_zone *zone, char *painting_table, t_shape *shape)
{
	int x = -1, y = -1, is_in;
	while (++y < zone->height)
	{
		x = -1;
		while (++x < zone->width)
		{
			is_in = is_in_circle((float)x, (float)y, shape);
			if ((shape->type == 'c' && is_in == 2) || (shape->type == 'C' && is_in))
				painting_table[(y * zone->width) + x] = shape->character;
		}
	}
}

int draw__shapes(FILE *file, t_zone *zone, char *painting_table)
{
	t_shape temp;
	int return_value;
	while ((return_value = fscanf(file, "%c %f %f %f %c\n", &temp.type, &temp.x, &temp.y, &temp.radius, &temp.character)) == 5)
	{
		if (temp.radius <= 0.00000000 || (temp.type != 'c' && temp.type != 'C'))
			return 0;
		draw_shape(zone, painting_table, &temp);
	}
	if (return_value != -1)
		return 0;
	return 1;
}


void draw_(t_zone *zone, char *painting_table)
{
	int i = -1;
	while (++i < zone->height)
	{
		write(1, painting_table + i * zone->width, zone->width);
		write(1, "\n", 1);
	}
}

int print_error(char  const *str)
{
	if (str)
		write(1, str, ft_strlen(str));
	return 1;
}

int clear_all(FILE *file, char *painting_table, char const *str)
{
	if (file)
		fclose(file);
	if (painting_table)
		free(painting_table);
	if (str)
		print_error(str);
	return 1;
}

int main(int argc, char **argv)
{
	FILE *file;
	t_zone zone;
	char *painting_table;

	zone.width = 0;
	zone.height = 0;
	zone.background = 0;
	if (argc != 2)
		return (print_error("Error: argument\n"));
	if (!(file = fopen(argv[1], "r")))
		return (print_error("Error: Operation file corrupted\n"));
	if (!(painting_table = take_params_and_zone(file, &zone)))
		return (clear_all(file, NULL, "Error: Operation file corrupted\n"));
	if (!(draw__shapes(file, &zone, painting_table)))
		return (clear_all(file, painting_table, "Error: Operation file corrupted\n"));
	draw_(&zone, painting_table);
	clear_all(file, painting_table, NULL);
	return 0;
}