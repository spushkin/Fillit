/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tetrimino.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obibik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/14 12:26:51 by obibik            #+#    #+#             */
/*   Updated: 2018/09/14 12:26:53 by obibik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "fillit.h"

/*
** Frees a list made with read_tetri.
*/

t_list		*free_list(t_list *list)
{
	t_etris *tetris;
	t_list	*next;

	while (list)
	{
		tetris = (t_etris *)list->content;
		next = list->next;
		free_tetris(tetris);
		ft_memdel((void **)&list);
		list = next;
	}
	return (NULL);
}

/*
** Frees a tetrimino structure (e.g. ####).
*/

void		free_tetris(t_etris *tetri)
{
	int i;

	i = 0;
	while (i < tetri->height)
	{
		ft_memdel((void **)(&(tetri->pos[i])));
		i++;
	}
	ft_memdel((void **)(&(tetri->pos)));
	ft_memdel((void **)&tetri);
}

/*
** Creates a new tetrimino structure (e.g. #->#->#->#).
*/

t_etris		*tetris_new(char **pos, int width, int height, char value)
{
	t_etris	*tetri;

	tetri = ft_memalloc(sizeof(t_etris));
	tetri->pos = pos;
	tetri->width = width;
	tetri->height = height;
	tetri->value = value;
	return (tetri);
}

/*
** Creates a new point structure(base square size for dots).
*/

t_point		*new_size(int x, int y)
{
	t_point	*size;

	size = ft_memalloc(sizeof(t_point));
	size->x = x;
	size->y = y;
	return (size);
}
