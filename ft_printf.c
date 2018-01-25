/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlriabts <hlriabts@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/06 19:43:36 by hlriabts          #+#    #+#             */
/*   Updated: 2018/01/24 23:56:32 by hlriabts         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int print_list(t_printf *list)
{
	int i;

	i = 0;
	while (list)
	{
		if (list->pre_content )
		{
			write(1,list->pre_content,ft_strlen(list->pre_content));
			i += ft_strlen(list->pre_content);
		}
		if (list->content )
		{
			write(1,list->content,ft_strlen(list->content));
			i += ft_strlen(list->content);
			if ((list->con == 'c' || list->con == 'C') && list->dop_ind != 0)
			{
				write(1,&(list->content[ft_strlen(list->content)]),1);
				i++;
			}
		}
		list = list->next;
	}
	return(i);
}
void function_find(t_printf *list, va_list arguments)
{
	char *buf;

	buf = NULL;
	// list->precision = (list->precision == -1 &&
	// 	!ft_strchr("pdiouxXDOU", list->con)) ? 0 : list->precision;
	if (list->con == 'p')
	{
		list->con = 'x';
		list->c_fl = 'l';
		list->flags = ft_strjoin(list->flags, "#p");
	}
	if (ft_strchr("pdiouxXDOU", list->con))
		pre_int(list,arguments);
	else if (ft_strchr("csCS", list->con))
	{
	 	if (list->con == 's' && list->c_fl !='l')
	 		if(!(buf =  va_arg(arguments, char *)))
	 			buf = "(null)";
	 	pre_char(list,arguments,buf, list->con);
	 	str_post_flag(list);
	}
	else
	{
		pre_char(list,arguments,buf, list->con);
		str_post_flag(list);
	}
}
int	ft_printf (const char * format, ...)
{
	t_printf *arg_list;
	t_printf *head;
	va_list		ap;
	int start;
	int end;

	arg_list = NULL;
	va_start(ap, format);
	start = 0;
	arg_list = list_init(arg_list);
	head = arg_list;
	while (format[start] != '\0')
	{
		if (pre_content_copy(format, &arg_list, &start) == 0 || format[start - 1] == '\0')
			break;
		end = find_conv_prec_width(format, start, arg_list);	
		find_flags(format, start, end, arg_list);
		//printf("\ncon=%c flags=%s width=%d prec=%d conv_fl=%c precontent=%s, content=%s",arg_list->con,arg_list->flags,arg_list->min_width,arg_list->precision,arg_list->c_fl,arg_list->pre_content,arg_list->content );
		function_find(arg_list, ap);
		
		arg_list->next = list_init(arg_list);
		arg_list = arg_list->next;
		start = end + 1;	
	}
	va_end(ap);
	return (print_list(head));
}
/*функцию каста в нужный формат для чисел.*/
