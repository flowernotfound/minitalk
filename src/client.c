/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiroaki <hiroaki@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 15:18:23 by hiroaki           #+#    #+#             */
/*   Updated: 2024/11/16 15:45:40 by hiroaki          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minitalk_bonus.h"

static volatile sig_atomic_t	g_response = 0;

static void	set_flag(int sig, siginfo_t *siginfo, void *context)
{
	(void)sig;
	(void)siginfo;
	(void)context;
	g_response = 1;
}

static void	send_char(int pid_server, int c)
{
	int	bit_position;
	int	current_bit;

	bit_position = 0;
	while (bit_position <= 7)
	{
		current_bit = c >> bit_position;
		current_bit = current_bit & 1;
		if (current_bit == 1)
		{
			if (kill(pid_server, SIGUSR1) == -1)
				exit(EXIT_FAILURE);
		}
		else
		{
			if (kill(pid_server, SIGUSR2) == -1)
				exit(EXIT_FAILURE);
		}
		bit_position++;
		while (g_response != 1)
			usleep(50);
		g_response = 0;
	}
}

static void	send_string(pid_t pid_server, char *message)
{
	int	i;

	i = 0;
	while (1)
	{
		send_char(pid_server, message[i]);
		if (message[i] == '\0')
			break ;
		i++;
	}
	write(1, "Received!\n", 10);
}

int	main(int ac, char **av)
{
	struct sigaction	sa;
	pid_t				pid_server;
	char				current_char;

	if (ac != 3)
		return (0);
	pid_server = ft_atoi(av[1]);
	if (pid_server <= 0)
		return (0);
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = set_flag;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGUSR1, &sa, NULL) == -1)
		return (1);
	send_string(pid_server, av[2]);
	write(1, "Received!\n", 10);
	return (0);
}
