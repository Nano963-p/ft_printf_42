/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hyna <mel-hyna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 20:09:39 by mel-hyna          #+#    #+#             */
/*   Updated: 2025/11/20 20:10:10 by mel-hyna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	verif_type(char a, va_list args)
{
	if (a == '%')
		return (write(1, "%", 1));
	if (a == 'd' || a == 'i')
		return (ft_putnbr(va_arg(args, int)));
	if (a == 'c')
		return (ft_putchar((char)va_arg(args, int)));
	if (a == 's')
		return (ft_putstr(va_arg(args, char *)));
	if (a == 'p')
		return (ft_putptr(va_arg(args, void *)));
	if (a == 'u')
		return (ft_putunbr(va_arg(args, unsigned int)));
	if (a == 'x')
		return (ft_puthex(va_arg(args, unsigned int), 0));
	if (a == 'X')
		return (ft_puthex(va_arg(args, unsigned int), 1));
	return (write(1, "%", 1) + write(1, &a, 1));
}

int	ft_printf(const char *format, ...)
{
	va_list	args;
	int		i;
	int		total;

	i = 0;
	total = 0;
	if (!format || write(1, "", 0) < 0)
		return (-1);
	va_start(args, format);
	while (format[i])
	{
		if (format[i] == '%' && format[i + 1] != '\0')
			total += verif_type(format[++i], args);
		else if (format[i] != '%')
		{
			write(1, &format[i], 1);
			total++;
		}
		i++;
	}
	va_end(args);
	return (total);
}
