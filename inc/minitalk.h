/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minitalk.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiroaki <hiroaki@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 15:18:37 by hiroaki           #+#    #+#             */
/*   Updated: 2024/11/16 15:18:38 by hiroaki          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINITALK_H
# define MINITALK_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# endif

# include <signal.h>
# include <stdlib.h>
# include <unistd.h>

int		ft_atoi(char *str);
void	ft_putnbr(int n);
void	ft_putchar(char c);

#endif
