/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiroaki <hiroaki@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 15:18:27 by hiroaki           #+#    #+#             */
/*   Updated: 2024/11/16 16:17:25 by hiroaki          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minitalk_bonus.h"

static int	validate_and_convert(char *str, int *i, int *sign)
{
	*i = 0;
	*sign = 1;
	if (str[*i] == '-')
	{
		*sign = -1;
		(*i)++;
	}
	return (0);
}

static long	process_digits(char *str, int *i)
{
	long	num;

	num = 0;
	while (str[*i] != '\0')
	{
		if (str[*i] >= '0' && str[*i] <= '9')
		{
			if ((num * 10 + (str[*i] - '0')) > INT_MAX)
				exit(EXIT_FAILURE);
			num = num * 10 + (str[*i] - '0');
			(*i)++;
		}
		else
			exit(EXIT_FAILURE);
	}
	return (num);
}

int	ft_atoi(char *str)
{
	int		i;
	int		sign;
	long	num;

	validate_and_convert(str, &i, &sign);
	num = process_digits(str, &i);
	num *= sign;
	if (num > INT_MAX || num < INT_MIN)
		exit(EXIT_FAILURE);
	return ((int)num);
}

void	ft_putchar(char c)
{
	write(1, &c, 1);
}

void	ft_putnbr(int n)
{
	unsigned int	nb;

	nb = n;
	if (nb >= 10)
		ft_putnbr(nb / 10);
	ft_putchar(nb % 10 + 48);
}
