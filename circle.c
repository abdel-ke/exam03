#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <stdlib.h>

typedef	struct	s_zone
{
	float	width;
	float	height;
	char	ch;
}				t_zone;

typedef struct	s_circle
{
	char	type;
	float	x;
	float	y;
	float	raduis;
	char	c;
}				t_circle;

void	ft_putchar(char c)
{
	write (1, &c, 1);
}

void	ft_putstr(char *str)
{
	while (*str)
		ft_putchar(*str++);
}

char	*get_zone(FILE *file, t_zone *zone)
{
	char	*draw;
	int		ret,
			i;

	if ((ret = fscanf(file, "%f %f %c\n", &zone->width, &zone->height, &zone->ch) != 3))
		return (NULL);
	if (zone->width <= 0 || zone->width > 300 || zone->height <= 0 || zone->height > 300)
		return (NULL);
	if (!(draw = malloc(sizeof(char) * zone->width * zone->height)))
		return 	NULL;
	i = -1;
	while (++i < zone->width * zone->height)
		draw[i] = zone->ch;
	return draw;
}

int		ft_free(FILE *file, char *draw, char *msg)
{
	fclose(file);
	if (draw)
	{
		free(draw);
		draw = NULL;
	}
	if (msg)
		ft_putstr(msg);
	return 1;
}

int		is_in_circle(int x, int y, t_circle *circle)
{
	float	dis;

	dis = sqrtf(powf(x - circle->x, 2.) + powf(y - circle->y, 2.));
	if (dis <= circle->raduis)
	{
		if (circle->raduis - dis < 1.)
			return 2;
		return 1;
	}
	return 0;
}

int		drawing_shape(FILE *file, t_zone *zone, char *draw)
{
	int ret,
		ret1,
		i,
		j;
	t_circle circle;

	while ((ret = fscanf(file, "%c %f %f %f %c\n", &circle.type, &circle.x, &circle.y, &circle.raduis, &circle.c)) == 5)
	{
		if (circle.raduis <= 0.00000000  || (circle.type != 'c' && circle.type != 'C'))
			return 0;
		j = 0;
		while (j < zone->height)
		{
			i = 0;
			while (i < zone->width)
			{
				ret1 = is_in_circle(i, j, &circle);
				if ((circle.type == 'c' && ret1 == 2) || (circle.type = 'C' && ret1))
					draw[j * (int)zone->width + i] = circle.c;
				i++;
			}
			j++;
		}
	}
	if (ret != -1)
		return 0;
	return 1;
}

int main(int ac, char **av)
{
	FILE	*file;
	t_zone	zone;
	char	*draw;
	int		i;

	if (ac != 2)
	{
		ft_putstr("Error: argument\n");
		return (1);
	}
	if (!(file = fopen(av[1], "r")))
	{
		ft_putstr("Error: Operation file corrupted\n");
		return (1);
	}
	if (!(draw = get_zone(file, &zone)))
		return ft_free(file, NULL, "Error: Operation file corrupted\n");
	if (!drawing_shape(file, &zone, draw))
		return ft_free(file, draw, "Error: Operation file corrupted\n");
	i = 0;
	while (i < zone.height)
	{
		write(1, draw + (i * (int)zone.width), zone.width);
		write(1, "\n", 1);
		i++;
	}
	ft_free(file, draw, NULL);
}