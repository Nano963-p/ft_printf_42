/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hyna <mel-hyna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 20:10:35 by mel-hyna          #+#    #+#             */
/*   Updated: 2025/11/20 20:17:50 by mel-hyna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	count_d(int d)
{
	int	count;

	count = 0;
	if (d <= 0)
		count++;
	while (d)
	{
		d = d / 10;
		count++;
	}
	return (count);
}

int	ft_putnbr(int d)
{
	long	nb;

	nb = d;
	if (nb < 0)
	{
		ft_putchar('-');
		nb = -nb;
	}
	if (nb >= 10)
	{
		ft_putnbr(nb / 10);
	}
	ft_putchar((nb % 10) + '0');
	return (count_d(d));
}
