/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_puthex.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hyna <mel-hyna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 20:10:42 by mel-hyna          #+#    #+#             */
/*   Updated: 2025/11/20 20:20:35 by mel-hyna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	count_d(unsigned int n)
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

static char	*put_16(unsigned int n)
{
	int			nbr;
	const char	*s;
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

int	ft_puthex(unsigned int n, int uppercase)
{
	char	*s;
	int		d;
	int		i;

	s = put_16(n);
	if (!s)
		return (-1);
	d = count_d(n);
	i = 0;
	while (s[i] != '\0')
	{
		if (uppercase)
			if (s[i] >= 'a' && s[i] <= 'f')
				s[i] = s[i] - 32;
		write(1, &s[i], 1);
		i++;
	}
	free(s);
	return (d);
}
