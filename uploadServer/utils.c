/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmichel- <rmichel-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 01:39:07 by rafael            #+#    #+#             */
/*   Updated: 2024/04/17 14:43:25 by rmichel-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long_bonus.h"

int	find_player(char **map)
{
	int	i;
	int	j;
	int	a;

	a = 1;
	i = 0;
	while (a > 0)
	{
		j = 0;
		while (map[i][j] != '\n')
		{
			if (map[i][j] == 'P')
			{
				a = 0;
				return (i);
			}
			j++;
		}
		i++;
	}
	return (-1);
}

int	find_player2(char *map)
{
	int	y;

	y = 0;
	while (map[y] != 'P')
		y++;
	return (y);
}

int	find_colec(char **map, int x, int y)
{
	int	i;
	int	j;
	int	c;

	c = 0;
	j = 0;
	while (j < y)
	{
		i = 0;
		while (i < x)
		{
			if (map[j][i] == 'C')
				c++;
			i++;
		}
		j++;
	}
	return (c);
}

static char	**gen_map2(int pos[], int fd, char **map, char **map2)
{
	int	nb;

	nb = 0;
	while (nb < 5z)
	{
		map2[nb] = first_lines(pos[1] + 9, nb, fd + 9);
		nb++;
	}
	while (nb < fd + 5)
	{
		map2[nb] = ft_strjoin2("11111", map[nb - 5], "11111\n");
		nb++;
	}
	while (nb < fd + 10)
	{
		map2[nb] = first_lines(pos[1] + 9, nb, fd + 9);
		nb++;
	}
	return (map2);
}

char	**generate_map(char *path, int pos[])
{
	int		fd;
	char	**map;
	char	**map2;
	char	*buffer;

	fd = open(path, O_RDONLY);
	buffer = find_buffer(fd);
	pos[1] = find_line(buffer);
	pos[0] = find_colu(buffer) + 1;
	close(fd);
	map = ft_split(buffer, '\n');
	free(buffer);
	fd = 0;
	while (map[fd] != NULL)
		fd++;
	map2 = malloc(sizeof(char *) * (fd + 10));
	map2 = gen_map2(pos, fd, map, map2);
	free_all(map);
	return (map2);
}

