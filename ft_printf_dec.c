/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_dec.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlriabts <hlriabts@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/18 00:59:54 by hlriabts          #+#    #+#             */
/*   Updated: 2018/01/25 01:46:05 by hlriabts         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int memory_calc(t_printf *l, int count, int base, int sign)
{
	int size;

	size = 0;
	if ((base == 10 && ft_strchr("dD", l->con) &&
			((sign < 0) || (ft_strchr(l->flags, ' ')) 
				|| ft_strchr(l->flags, '+'))))
		count++;
	if (base == 8  && ft_strchr(l->flags, '#'))
		count++;
	if (l->min_width >= l->precision && l->min_width > count)
		size += (l->min_width);
	else if (l->min_width < l->precision && l->precision > count)
	{
		size = l->precision;
		if ((base == 10 && ft_strchr("dD", l->con) &&
			((sign < 0) || (ft_strchr(l->flags, ' ')) 
				|| ft_strchr(l->flags, '+'))))
		size++;
	}
	else 
		size = count;
	if (size > l->min_width)
		size += (base == 16 && ft_strchr(l->flags, '#')) ? 2 :0;
	l->content = ft_memalloc((size + 1) * sizeof(char));
	ft_memset(l->content, '_',  size);
	return(size);
}

void printf_itoa (t_printf *l, size_t n,  int base, int first_s)
{
	char hex;
	int i;
	size_t tmp;
	int count ;

	count = (l->precision == 0 && n == 0) ? 0 : 1;
	i = ft_strlen(l->content) - 1;
	tmp = n;
	hex = (l->con == 'X') ? 55 : 87;
	while(tmp /= base)
		count++;
	if(ft_strchr(l->flags, '-') && l->min_width > l->precision)
	{
		i = first_s + count -1;
		i += (l->content[0] == '+' || l->content[0] == '-') ? 1 : 0;
		i += (l->precision > count) ? l->precision - count : 0;
	}
	while (count-- != 0)
	{
		l->content[i--] = (base == 16 && (n % base) > 9)
		? (n % base) + hex : (n % base) + '0';
		n /= base;
	}
}
int width_count (t_printf *l, int sign, int count, int base)
{
	int i;

	i = 0;
	if ((base == 10 && ft_strchr("dD", l->con) &&
			((sign < 0) || (ft_strchr(l->flags, ' ')) 
				|| ft_strchr(l->flags, '+'))))
		i++;
	if (l->precision != 0)
		i += (l->precision > count) ? l->precision - count : 0;
	if (base == 16 && ft_strchr(l->flags, '#'))
		i += 2;
	return(i);
}
int pre_width(t_printf *l, int size, int count)
{
	int i;

	i = (ft_strchr(l->flags, '-')) ? size - 1 : 0;
	while(l->min_width > count)
	{
		if (ft_strchr(l->flags, '0') && l->precision == -1
			&& !ft_strchr(l->flags, '-'))
			l->content[i] = '0';
		else 
			l->content[i] = ' ';
		count++;
		i += (ft_strchr(l->flags, '-')) ? -1 : 1;
	}
	return(i);
}

int sign_hash(t_printf *l, int base, int sign, int count)
{
	int i;

	i = 0;
	count += (base == 16 && ft_strchr(l->flags, '#')) ? 2 : 1;
	if (!ft_strchr(l->flags, '-'))
		while (l->content[i] == ' ' && i < (int) ft_strlen(l->content) - count)
				i++;
	if (base % 8 == 0 && ft_strchr(l->flags, '#'))
		l->content[i++] = '0';
	if (base == 16 && ft_strchr(l->flags, '#'))
		l->content[i++] =  l->con;
	else if (base == 10 && sign > 0 && ft_strchr("dD", l->con))
	{
		if (ft_strchr(l->flags, ' '))
			l->content[i] =  ' ';
		if(ft_strchr(l->flags, '+'))
			l->content[i] = '+';
	}
	else if (base == 10 && sign < 0 && ft_strchr("dD", l->con))
		l->content[i] = '-';
	return(i);
}
void precision_check(t_printf *l)
{
	int i;

	i = 0;
	while (l->content[i] != '\0')
	{
		if (l->content[i] == '_')
				l->content[i] = '0';
		i++;
	}
}
void int_printf(t_printf *l, size_t n, int sign)
{
	size_t tmp;
	int    size;
	int 	base;
	int count;

	count = (l->precision == 0 && n == 0) ? 0 : 1;
	base = (ft_strchr("oO", l->con)) ? 8 : 10;
	base = (ft_strchr("xX", l->con)) ? 16 : base;	
	tmp = n;
	while(tmp /= base)
		count++;		
	if (n == 0 && ft_strchr("oOxX", l->con) && !ft_strchr(l->flags, 'p'))
	{
		size = (ft_strchr(l->flags, '#') && ft_strchr("oO", l->con)
			&& l->precision != -1) ? count + 1 : count;
		l->content = ft_memalloc((size + 1) * sizeof(char));
		ft_memset(l->content, '_',  size);
	}
	else
		size = memory_calc(l, count,  base, sign);
	tmp = pre_width(l, size, width_count(l, sign, count, base) + count);
	if (!(n == 0 && ft_strchr("oOxX", l->con) && !ft_strchr(l->flags, 'p')))
		tmp = sign_hash(l, base, sign, count);
	printf_itoa(l, n,  base, tmp);
	precision_check(l);
}

void pre_int(t_printf *l, va_list ap)
{
	size_t buf;
	long long buf2;
	int sign;

	if (ft_strchr("DOU", l->con))
		l->c_fl = (l->c_fl == '0') ? 'l' : 'L';
	l->con = (ft_strchr("DOU", l->con)) ? ft_tolower(l->con) : l->con;
	(l->con == 'd' && l->c_fl == 'H') ? buf2 =(char) va_arg(ap,int): 1; 
	(l->con != 'd' && l->c_fl == 'H') ? buf = (unsigned char) va_arg(ap, int): 1;
	(l->con != 'd' && l->c_fl == 'h') ? buf = (unsigned short) va_arg(ap, int): 1;
	(l->con == 'd' && l->c_fl == 'h') ? buf2 = (short) va_arg(ap, int): 1;
	(l->con != 'd' && l->c_fl == 'l') ? buf = va_arg(ap, unsigned long): 1;
	(l->con == 'd' && l->c_fl == 'l') ? buf2 = va_arg(ap, long): 1;
	(l->con != 'd' && l->c_fl == 'L') ? buf = va_arg(ap, unsigned long long): 1;
	(l->con == 'd' && l->c_fl == 'L') ? buf2 = va_arg(ap, long long): 1;
	(l->con != 'd' && l->c_fl == 'z') ? buf = va_arg(ap, size_t): 1;
	(l->con == 'd' && l->c_fl == 'z') ? buf2 = va_arg(ap,  size_t): 1;
	(l->con == 'd' && l->c_fl == 'j') ? buf2 = va_arg(ap, intmax_t): 1;
	(l->con != 'd' && l->c_fl == 'j') ? buf = va_arg(ap, uintmax_t): 1;
	(l->c_fl == '0' && (l->con == 'd')) ? buf2 = (va_arg(ap, int)) : 1;
	(l->c_fl == '0' && (ft_strchr("ouxX",l->con))) ? buf = (unsigned int)(va_arg(ap, int)) : 1;
	sign = ( (l->con == 'd' || l->con == 'D') && buf2 < 0) ? -1 : 1;
	buf = ((l->con == 'd' || l->con == 'D')) ? (buf2 * sign) : buf;
	int_printf(l, buf, sign);
}