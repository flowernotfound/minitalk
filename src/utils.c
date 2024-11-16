#include "../inc/minitalk.h"

int	ft_atoi(char *str) // only positive numbers
{
	int	i;
	int	num;

	i = 0;
	num = 0;
	while (str[i] != '\0')
	{
		if (48 <= str[i] && str[i] <= 57)
		{
			num = num * 10 + str[i] - 48;
			i++;
		}
		else
			exit(EXIT_FAILURE);
	}
	return (num);
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
