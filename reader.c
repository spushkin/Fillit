/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reader.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obibik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/14 12:26:28 by obibik            #+#    #+#             */
/*   Updated: 2018/09/14 12:26:30 by obibik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "fillit.h"
#define OLD t_list *list;t_etris *tetris
#define INT char *buf;int bl_count;int old_n;char letter

/*
** Utility function to get min-max values for get_piece.
** Find the smallest x coordinate and biggest y coordinate of tetrimino.
** Find min x, y and max x,y from current tetrimino:
** e.g for:
** .##.
** .#..
** .#..
** ....
** result will be: min.x = 1, min.y = 0, max.x = 2, max.y = 2
*/

void		ft_min_max(char *str, t_point *min, t_point *max)
{
	int i;

	i = 0;
	while (i < 20)
	{
		if (str[i] == '#')
		{
			if (i / 5 < min->y)
				min->y = i / 5;
			if (i / 5 > max->y)
				max->y = i / 5;
			if (i % 5 < min->x)
				min->x = i % 5;
			if (i % 5 > max->x)
				max->x = i % 5;
		}
		i++;
	}
}

/*
** Reads a piece from a valid chunk, allocates a structure and populates it.
** Converts tetrimino from e.g.:
** .##.
** .#..
** .#..
** ....
**
** to
**
** ##
** #.
** #.
** It goes through each row in the terimino and copies only
** needed characters to "pos" array.
** Init new tetris structure, e.g:
** pos =
** 		 ##
** 		 #.
** 		 #.
** width = 2, heighr = 3, value = 'A'
*/

t_etris		*get_piece(char *str, char value)
{
	t_point		*min;
	t_point		*max;
	char		**pos;
	int			i;
	t_etris		*tetri;

	min = new_size(3, 3);
	max = new_size(0, 0);
	ft_min_max(str, min, max);
	pos = ft_memalloc(sizeof(char *) * (max->y - min->y + 1));
	i = 0;
	while (i < max->y - min->y + 1)
	{
		pos[i] = ft_strnew(max->x - min->x + 1);
		ft_strncpy(pos[i], str + (min->x) + (i + min->y) * 5,
				max->x - min->x + 1);
		i++;
	}
	tetri = tetris_new(pos, max->x - min->x + 1, max->y - min->y + 1, value);
	ft_memdel((void **)&min);
	ft_memdel((void **)&max);
	return (tetri);
}

/*
** Checks connection counts (connections between blocks of single tetrim),
** if we have 6 or 8 connections, the tetrimino is valid.
** Otherwise, our tetrimino is not contiguous.
*/

int			check_connection(char *str)
{
	int	block;
	int i;

	block = 0;
	i = 0;
	while (i < 20)
	{
		if (str[i] == '#')
		{
			if ((i + 1) < 20 && str[i + 1] == '#')
				block++;
			if ((i - 1) >= 0 && str[i - 1] == '#')
				block++;
			if ((i + 5) < 20 && str[i + 5] == '#')
				block++;
			if ((i - 5) >= 0 && str[i - 5] == '#')
				block++;
		}
		i++;
	}
	return (block == 6 || block == 8);
}

/*
** Checks character counts, hash amount,dot positions and new lines
*/

int			check_counts(char *str, int count)
{
	int i;
	int blocs;

	blocs = 0;
	i = 0;
	while (i < 20)
	{
		if (i % 5 < 4)
		{
			if (!(str[i] == '#' || str[i] == '.'))
				return (1);
			if (str[i] == '#' && ++blocs > 4)
				return (2);
		}
		else if (str[i] != '\n')
			return (3);
		i++;
	}
	if (count == 20 && str[19] != '\n' && str[20] != '\0')
		return (4);
	if (!check_connection(str))
		return (5);
	return (0);
}

/*
** Read tetriminos from fd and put them in a list.
** We use 21 sized reads to read piece by piece since there are
** 4 lines made of 4 chars (+ newline) = 20 chars + sep. newline = 21 chars
*/

t_list		*read_tetri(int fd)
{
	OLD;
	INT;
	letter = 'A';
	buf = ft_strnew(BASE_SIZE + 1);
	list = NULL;
	while ((bl_count = read(fd, buf, (BASE_SIZE + 1))) >= 20)
	{
		if (check_counts(buf, bl_count) || !(tetris = get_piece(buf, letter++)))
		{
			ft_memdel((void **)&buf);
			return (free_list(list));
		}
		ft_lstadd(&list, ft_lstnew(tetris, sizeof(t_etris)));
		ft_memdel((void **)&tetris);
		old_n = bl_count;
	}
	ft_memdel((void **)&buf);
	if (bl_count != 0 || old_n != BASE_SIZE)
		return (free_list(list));
	ft_lstrev(&list);
	return (list);
}
