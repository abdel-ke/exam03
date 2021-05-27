#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>

typedef struct	s_zone
{
	float	width;
	float	height;
	char	ch;
}				t_zone;	

typedef struct	s_rect
{
	char	type;
	float	x;
	float	y;
	float	w;
	float	h;
	char	c;
}				t_rect;

void	ft_putchar(char c)
{
	write(1, &c, 1);
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

	if ((ret = fscanf(file, "%f %f %c\n", &zone->width, &zone->height, &zone->ch)) != 3)
		return NULL;
	if (zone->width < 0 || zone->width > 300 || zone->height <= 0 || zone->height > 300)
		return 	NULL;
	if (!(draw = malloc(sizeof(char) * zone->width * zone->height)))
		return NULL;
	i = 0;
	while (i < zone->width * zone->height)
		draw[i++] = zone->ch;
	return draw;
}

int		is_in_rect(int x, int y, t_rect *rect)
{
	if ((x < rect->x || rect->x + rect->w < x)
	 || (y < rect->y || rect->y + rect->h < y))
		return (0);
	if ((x - rect->x < 1.00000000 || (rect->x + rect->w) - x < 1.00000000)
	||  (y - rect->y < 1.00000000 || (rect->y + rect->h) - y < 1.00000000))
		return (2);
	return (1);
}

int		drawing_shape(FILE *file, t_zone *zone, char *draw)
{
	t_rect rect;
	int		ret,
			ret1,
			i,
			j;

	while ((ret = fscanf(file, "%c %f %f %f %f %c\n", &rect.type, &rect.x, &rect.y, &rect.w, &rect.h, &rect.c)) == 6)
	{
		if (rect.w <= 0.00000000 || rect.h <= 0.0000000 || (rect.type != 'R' && rect.type != 'r'))
			return (0);
		j = 0;
		while (j < zone->height)
		{
			i = 0;
			while (i < zone->width)
			{
				ret1 = is_in_rect(i, j, &rect);
				if ((rect.type == 'r' && ret1 == 2) || (rect.type == 'R' && ret1))
					draw[j * (int)zone->width + i] = rect.c;
				i++;
			}
			j++;
		}
	}
	if (ret != -1)
		return (0);
	return (1);
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
	return (1);
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
		return (ft_free(file, NULL, "Error: Operation file corrupted\n"));
	if (!(drawing_shape(file, &zone, draw)))
		return (ft_free(file, draw, "Error: Operation file corrupted\n"));
	i = 0;
	while (i < zone.height)
	{
		write (1, draw + (i * (int)zone.width), zone.width);
		ft_putchar('\n');
		i++;
	}
	ft_free(file, draw, NULL);
	return (0);
}