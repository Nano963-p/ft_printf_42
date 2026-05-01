/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putptr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hyna <mel-hyna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 20:10:25 by mel-hyna          #+#    #+#             */
/*   Updated: 2025/11/20 20:19:04 by mel-hyna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	count_d(unsigned long long n)
{
	int	count;

	count = 0;
	if (n == 0)
		return (1);
	while (n != 0)
	{
		n = n / 16;
		count++;
	}
	return (count);
}

static char	*put_16(unsigned long long n)
{
	int			nbr;
	const char	*s = "0123456789abcdef";
	char		*res;
	size_t		i;

	nbr = count_d(n);
	s = "0123456789abcdef";
	res = malloc(nbr + 1);
	if (!res)
		return (NULL);
	i = nbr;
	res[nbr] = '\0';
	if (n == 0)
		res[0] = '0';
	while (n)
	{
		res[--i] = s[n % 16];
		n = n / 16;
	}
	return (res);
}

int	ft_putptr(void *p)
{
	unsigned long long	a;
	char				*s;
	int					i;
	int					count;

	if (p == NULL)
	{
		write(1, "(nil)", 5);
		return (5);
	}
	a = (unsigned long long)p;
	s = put_16(a);
	if (!s)
		return (-1);
	write(1, "0x", 2);
	count = 2;
	i = 0;
	while (s[i] != '\0')
	{
		write(1, &s[i], 1);
		i++;
		count++;
	}
	free(s);
	return (count);
}
