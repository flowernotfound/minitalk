/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiroaki <hiroaki@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 15:18:16 by hiroaki           #+#    #+#             */
/*   Updated: 2024/11/16 19:35:09 by hiroaki          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minitalk_bonus.h"

static volatile sig_atomic_t	g_response_info[2] = {};

static void	accumulate_string(unsigned char c)
{
	static unsigned char	buffer[BUFFER_SIZE];
	static int				i = 0;

	buffer[i] = c;
	i++;
	if (i == BUFFER_SIZE)
	{
		write(1, buffer, i);
		i = 0;
	}
	if (c == '\0')
	{
		write(1, buffer, i);
		write(1, "\n", 1);
		i = 0;
	}
}

static void	receive_signal(int sig, siginfo_t *siginfo, void *context)
{
	static unsigned char	current_byte;
	static int				i;

	(void)context;
	if (sig == SIGUSR1)
		current_byte |= (1 << i);
	i++;
	if (i == 8)
	{
		accumulate_string(current_byte);
		current_byte = 0;
		i = 0;
	}
	g_response_info[0] = 1;
	g_response_info[1] = siginfo->si_pid;
}

static void	print_pid(pid_t pid_server)
{
	write(1, "PID: ", 5);
	ft_putnbr(pid_server);
	write(1, "\n", 1);
}

int	main(void)
{
	struct sigaction	sa;
	pid_t				pid_server;

	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = receive_signal;
	sigemptyset(&sa.sa_mask);
	sigaddset(&sa.sa_mask, SIGUSR1);
	sigaddset(&sa.sa_mask, SIGUSR2);
	if (sigaction(SIGUSR1, &sa, NULL) == -1
		|| sigaction(SIGUSR2, &sa, NULL) == -1)
		return (1);
	pid_server = getpid();
	print_pid(pid_server);
	while (1)
	{
		while (g_response_info[0] != 1)
			usleep(50);
		g_response_info[0] = 0;
		kill(g_response_info[1], SIGUSR1);
	}
}
