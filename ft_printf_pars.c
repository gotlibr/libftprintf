/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_pars.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlriabts <hlriabts@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/22 17:06:14 by hlriabts          #+#    #+#             */
/*   Updated: 2018/01/24 23:31:39 by hlriabts         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

t_printf * list_init (t_printf *list)
{
	list = (t_printf *) malloc(sizeof(t_printf));
	(list)->next = NULL;
	list->c_fl = '0';
	list->min_width = 0;
	list->precision = -1;
	list->pre_content = NULL;
	list->content = NULL;
	list->dop_ind = 0;
	return (list);
}
int pre_content_copy(const char  *format,t_printf **list, int *start)
{
	int i;

	i = *start;
	while (format[i] != '%' && format[i] !='\0')
		i++;
	if (ft_strlen(format) == 1 && format[i] == '%')
		return(0);
	(*list)->pre_content = ft_memalloc(sizeof(char) * i + 1);
	if (i > *start)
		(*list)->pre_content = ft_strsub(format, *start, i - *start);
	// if (format[*start + 1] == '\0')
	// 	(*list)->pre_content = "%\0";

	*start = i + 1;
	return(1);
}

int find_conv_prec_width(const char *format, int start, t_printf *list)
{
	int i;
	char *not_cons;

	not_cons = "#0-+ hljz.123456789";
	i = -1;
	while (format[start + ++i] != '\0')
		if (ft_strchr(not_cons, format[start + i]) == NULL)
		{
			if (format[start + i] == 'i')
				list->con = 'd';
			else
				list->con = format[start+i];
			break;
		}
	format = &(format[start + i - 1]);
	while (*format != '%' &&  (ft_isdigit(*format) || *format == '.'))
		format--;
	format++;
	list->min_width = ABS(ft_atoi(format));
	while (ft_isdigit(*format))
		format++;
	format++;
	if (*(format - 1) == '.')
		list->precision = ABS(ft_atoi(format));
	return(start + i);
}

void find_flags(const char *format, int start, int end, t_printf *list)
{
	int 	i;
	int 	count;
	char 	*flags;
	char	*c_fls;

	i = -1;
	count = 0;
	flags = "#0-+ ";
	c_fls = "hljz";
	while (++i + start < end)
		count += (ft_strchr(flags, format[start + i]) != NULL) ? 1 : 0;
	i = -1;
	list->flags = (char*)ft_memalloc(sizeof(char) * count + 1);
	while (++i + start < end)
	{
		if (ft_strchr(flags, format[start + i]) != NULL && format[start + i - 1] != '.'
			&& (format[start + i] != '0' || !ft_isdigit(format[start + i - 1])))
			list->flags[ft_strlen(list->flags)] = format[start + i];
		if (ft_strchr(c_fls, format[start + i]) != NULL && list->c_fl == '0')
		{
			list->c_fl = format[start + i];
			list->c_fl = (format[start + i] == 'h' && format[start + i + 1] == 'h') ? 'H' : list->c_fl;
			list->c_fl = (format[start + i] == 'l' && format[start + i + 1] == 'l') ? 'L' : list->c_fl;
		}
	}
}