/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvigne <bvigne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/01 11:41:19 by bvigne            #+#    #+#             */
/*   Updated: 2018/05/26 11:41:19 by bvigne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int			find_endline(char **endline, char **line, char **buf, int *lu)
{
	if ((*endline = ft_strchr(*buf, '\n')) != NULL)
	{
		**endline = '\0';
		*line = ft_strdup(*buf);
		ft_memmove(*buf, *endline + 1, ft_strlen(*endline + 1) + 1);
		return (1);
	}
	else if (*lu == 0 && **buf)
	{
		*line = ft_strdup(*buf);
		*buf = "\0";
		return (1);
	}
	return (0);
}

char		*fill_line(int fd, char **buf, char **line, int *error)
{
	int			lu;
	char		add[BUFF_SIZE + 1];
	char		*tmp;
	char		*endline;

	lu = 1;
	if (buf[0] == '\0')
		*buf = ft_strdup("");
	while (lu > 0)
	{
		if ((lu = read(fd, add, BUFF_SIZE)) == -1)
		{
			*error = 1;
			return (NULL);
		}
		add[lu] = '\0';
		tmp = *buf;
		*buf = ft_strjoin(*buf, add);
		if (*buf[0] == 0)
			return (NULL);
		ft_strdel(&tmp);
		if (find_endline(&endline, line, buf, &lu))
			return (*buf);
	}
	return (NULL);
}

void		add_file(t_file **file_list, t_file **current)
{
	t_file	*lst;

	lst = *file_list;
	while (lst->next)
		lst = lst->next;
	lst->next = *current;
}

t_file		*ft_filenew(int fd)
{
	t_file	*tmp;

	if (!(tmp = (t_file *)malloc(sizeof(t_file))))
		return (NULL);
	tmp->fd = fd;
	tmp->buf = NULL;
	tmp->next = NULL;
	return (tmp);
}

int			get_next_line(const int fd, char **line)
{
	static t_file	*file_list = NULL;
	t_file			*current;
	int				error;

	if (fd < 0 || line == NULL)
		return (-1);
	error = 0;
	if (file_list == NULL)
		file_list = ft_filenew(fd);
	current = file_list;
	while (current)
	{
		if (current->fd == fd)
		{
			if (!fill_line(fd, &current->buf, line, &error))
				return (error) ? -1 : 0;
			return (1);
		}
		current = current->next;
	}
	current = ft_filenew(fd);
	add_file(&file_list, &current);
	if (!fill_line(fd, &current->buf, line, &error))
		return (error) ? -1 : 0;
	return (1);
}
