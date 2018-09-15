/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solver.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obibik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/14 12:26:40 by obibik            #+#    #+#             */
/*   Updated: 2018/09/14 12:26:42 by obibik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "fillit.h"

/*
** Backtracking implementation of the solver.
** get tetrimino from current linked list item
** while current tetrimino's height is equal or smaller than result matrix size
** while current tetrimino's width is equal or smaller than result matrix size
** try to put current tetrimino starting from x,y coordinate in result matrix
** it returns "1" if there are no collissions or "0" otherwise
** if current terimino was successfully placed lets try to place next tetrimino
*/

int		ft_solve_map(t_map *map, t_list *list)
{
	int		x;
	int		y;
	t_etris	*tetri;

	y = 0;
	if (list == NULL)
		return (1);
	tetri = (t_etris *)(list->content);
	while (y < map->size - tetri->height + 1)
	{
		x = 0;
		while (x < map->size - tetri->width + 1)
		{
			if (place(tetri, map, x, y))
			{
				if (ft_solve_map(map, list->next))
					return (1);
				else
					set_piece(tetri, map, new_size(x, y), '.');
			}
			x++;
		}
		y++;
	}
	return (0);
}

/*
** Gets the rounded up sqrt of a number. Equivalent to ceil(sqrt(n)).
** Check if there is enough space on th map for solve_map(creates a minimal
** space  for tetr).
*/

int		high_sqrt(int i)
{
	int	size;

	size = 2;
	while (size * size < i)
		size++;
	return (size);
}

/*
** Solving map starting from the smallest possible size using recursive function
**
** calculate initial size of result matrix, e.g 3x3 or 4x4 or 5x5 and so on
** if there is only 1 tetrimino the calculation will be:
** sqrt(1*4) + 1 = 3 which is 3x3
** if there are for example 3 tetriminos the calculation will be:
** sqrt(3*4) + 1 = 4 which is 4x4
**
** init result matrix and fill it with dots
** e.g with 3x3 it will be:
** ...
** ...
** ...
**
** run while until it returns "1"
** otherwise - increase result matrix to 1 more
** e.g if there is not solution for 3x3 -
** the result matrix will be increased to 4x4 to try again and so on
*/

t_map	*solve_map(t_list *list)
{
	t_map	*map;
	int		size;

	size = high_sqrt(ft_lstcount(list) * 4);
	map = map_new(size);
	while (!ft_solve_map(map, list))
	{
		size++;
		free_map(map);
		map = map_new(size);
	}
	return (map);
}
