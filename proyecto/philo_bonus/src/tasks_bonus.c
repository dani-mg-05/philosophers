/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tasks_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damedina <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 00:28:37 by damedina          #+#    #+#             */
/*   Updated: 2025/04/21 00:28:45 by damedina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers_bonus.h"

static void	exit_child(t_philosopher *philosopher, int status)
{
	t_table	*table;

	table = philosopher->table;
	sem_close(table->forks);
	sem_close(table->print_lock);
	sem_close(table->death_lock);
	sem_unlink("forks");
	sem_unlink("print");
	sem_unlink("death");
	pthread_join(philosopher->death_thread, NULL);
	free(table->pids);
	free(table->philosophers);
	free(table);
	exit(status);
}

static void	eat(t_philosopher *philosopher)
{
	t_table	*table;

	table = philosopher->table;
	sem_wait(table->forks);
	if (print_message(philosopher->id, FORK, table))
		return ;
	sem_wait(table->forks);
	if (print_message(philosopher->id, FORK, table))
		return ;
	if (print_message(philosopher->id, EAT, table))
		return ;
	philosopher->last_meal_time = current_time();
	usleep(table->time_to_eat * 1000);
	philosopher->meals++;
	sem_post(table->forks);
	sem_post(table->forks);
}

static void	*check_death(void *arg)
{
	t_philosopher	*philosopher;

	philosopher = (t_philosopher *)arg;
	while (1)
	{
		if (philosopher->table->num_meals != -1
			&& philosopher->meals >= philosopher->table->num_meals)
			break ;
		if (current_time() - philosopher->last_meal_time
			>= philosopher->table->time_to_die)
		{
			sem_post(philosopher->table->death_lock);
			sem_wait(philosopher->table->print_lock);
			printf("%lld %d %s\n", (current_time()
					- philosopher->table->start_time), philosopher->id, DIE);
		}
		usleep(10);
	}
	return (0);
}

void	philosopher_life(t_philosopher *philosopher)
{
	t_table		*table;

	table = philosopher->table;
	pthread_create(&philosopher->death_thread, NULL, check_death,
		(void *)philosopher);
	while (1)
	{
		if (philosopher->id % 2 != 0)
			usleep(100);
		if (print_message(philosopher->id, THINK, table))
			break ;
		eat(philosopher);
		if (table->num_meals != -1 && philosopher->meals >= table->num_meals)
			exit_child(philosopher, 0);
		if (print_message(philosopher->id, SLEEP, table))
			break ;
		usleep(table->time_to_sleep * 1000);
	}
	exit_child(philosopher, 1);
}
