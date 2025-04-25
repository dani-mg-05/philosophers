/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tasks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damedina <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 17:57:27 by damedina          #+#    #+#             */
/*   Updated: 2025/03/23 17:57:29 by damedina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers.h"

static void	eat_aux(t_philosopher *philosopher)
{
	t_table	*table;

	table = philosopher->table;
	if (print_message(philosopher->id, FORK, table))
		return ;
	if (print_message(philosopher->id, EAT, table))
		return ;
	philosopher->last_meal_time = current_time();
	usleep(table->time_to_eat * 1000);
	pthread_mutex_unlock(&table->forks[philosopher->left_fork]);
	pthread_mutex_unlock(&table->forks[philosopher->right_fork]);
}

static void	eat(t_philosopher *philosopher)
{
	t_table	*table;

	table = philosopher->table;
	if (table->num_philosophers % 2 == 0)
	{
		if (philosopher->id % 2 == 0)
			pthread_mutex_lock(&table->forks[philosopher->left_fork]);
		else
			pthread_mutex_lock(&table->forks[philosopher->right_fork]);
		if (print_message(philosopher->id, FORK, table))
			return ;
		if (philosopher->id % 2 == 0)
			pthread_mutex_lock(&table->forks[philosopher->right_fork]);
		else
			pthread_mutex_lock(&table->forks[philosopher->left_fork]);
	}
	else
	{
		pthread_mutex_lock(&table->forks[philosopher->left_fork]);
		if (print_message(philosopher->id, FORK, table))
			return ;
		pthread_mutex_lock(&table->forks[philosopher->right_fork]);
	}
	eat_aux(philosopher);
}

void	*philosopher_life(void *arg)
{
	t_philosopher	*philosopher;

	philosopher = (t_philosopher *)arg;
	while (!philosopher->table->death_flag)
	{
		if (print_message(philosopher->id, THINK, philosopher->table))
			break ;
		eat(philosopher);
		pthread_mutex_lock(&philosopher->meal_mutex);
		philosopher->meals++;
		pthread_mutex_unlock(&philosopher->meal_mutex);
		if (philosopher->table->num_meals != -1
			&& philosopher->meals >= philosopher->table->num_meals)
			break ;
		if (print_message(philosopher->id, SLEEP, philosopher->table))
			break ;
		usleep(philosopher->table->time_to_sleep * 1000);
	}
	pthread_mutex_lock(&philosopher->table->end_mutex);
	philosopher->table->ended_threads++;
	pthread_mutex_unlock(&philosopher->table->end_mutex);
	return (0);
}

void	*deathcheck_life(void *arg)
{
	t_table	*table;
	int		i;

	table = (t_table *)arg;
	while (1)
	{
		if (table->ended_threads == table->num_philosophers)
			return (0);
		i = -1;
		while (++i < table->num_philosophers)
		{
			pthread_mutex_lock(&table->philosophers[i].meal_mutex);
			if ((current_time() - table->philosophers[i].last_meal_time)
				>= table->time_to_die)
			{
				table->death_flag = 1;
				printf("%lld %d %s\n", (current_time() - table->start_time),
					table->philosophers[i].id, DIE);
				pthread_mutex_unlock(&table->philosophers[i].meal_mutex);
				return (0);
			}
			pthread_mutex_unlock(&table->philosophers[i].meal_mutex);
		}
		usleep(10);
	}
}
