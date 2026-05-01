/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putunbr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hyna <mel-hyna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 20:09:54 by mel-hyna          #+#    #+#             */
/*   Updated: 2025/11/20 20:22:45 by mel-hyna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	count_d(unsigned int d)
{
	int	count;

	count = 0;
	if (d == 0)
		return (1);
	while (d)
	{
		d = d / 10;
		count++;
	}
	return (count);
}

int	ft_putunbr(unsigned int n)
{
	char	c;

	if (n >= 10)
		ft_putunbr(n / 10);
	c = n % 10 + '0';
	write(1, &c, 1);
	return (count_d(n));
}
