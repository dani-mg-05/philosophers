/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damedina <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 00:27:53 by damedina          #+#    #+#             */
/*   Updated: 2025/04/21 00:28:03 by damedina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers_bonus.h"

long long	current_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

int	ft_atoi(const char *str)
{
	int		i;
	int		num;
	char	sign;

	i = 0;
	num = 0;
	sign = 1;
	while (str[i] == ' ' || str[i] == '\f' || str[i] == '\n'
		|| str[i] == '\r' || str[i] == '\t' || str[i] == '\v')
		i++;
	if (str[i] != '\0' && (str[i] == '-' || str[i] == '+'))
	{
		if (str[i++] == '-')
			sign *= -1;
	}
	while (str[i] >= '0' && str[i] <= '9')
		num = num * 10 + (int)(str[i++] - '0');
	return (num * sign);
}

int	print_usage(void)
{
	printf("Usage: ./philo number_of_philosophers time_to_die ");
	printf("time_to_eat time_to_sleep ");
	printf("[number_of_times_each_philosopher_must_eat]\n");
	return (1);
}

int	print_message(int id, char *msg, t_table *table)
{
	sem_wait(table->print_lock);
	printf("%lld %d %s\n", (current_time() - table->start_time), id, msg);
	sem_post(table->print_lock);
	return (0);
}

int	exit_philo(t_table *table)
{
	sem_close(table->forks);
	sem_close(table->print_lock);
	sem_close(table->death_lock);
	sem_unlink("forks");
	sem_unlink("print");
	sem_unlink("death");
	free(table->pids);
	free(table->philosophers);
	free(table);
	return (0);
}
