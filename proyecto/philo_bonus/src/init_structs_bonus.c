/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_structs_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damedina <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 00:27:31 by damedina          #+#    #+#             */
/*   Updated: 2025/04/21 00:27:37 by damedina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers_bonus.h"

t_philosopher	*init_philosophers(t_table *table)
{
	t_philosopher	*philos;
	int				num_philos;
	int				i;

	num_philos = table->num_philosophers;
	philos = (t_philosopher *)malloc((num_philos) * sizeof(t_philosopher));
	if (!philos)
	{
		free(table);
		return (0);
	}
	i = -1;
	while (++i < num_philos)
	{
		philos[i].id = i + 1;
		philos[i].meals = 0;
		philos[i].last_meal_time = current_time();
		philos[i].table = table;
	}
	return (philos);
}

static int	init_semaphores(t_table *table)
{
	int	status;

	status = 0;
	sem_unlink("forks");
	sem_unlink("print");
	sem_unlink("death");
	table->forks = sem_open("forks", O_CREAT, 0644, table->num_philosophers);
	if (table->forks == SEM_FAILED)
		status = 1;
	table->print_lock = sem_open("print", O_CREAT, 0644, 1);
	if (!status && table->print_lock == SEM_FAILED)
	{
		sem_close(table->forks);
		status = 1;
	}
	table->death_lock = sem_open("death", O_CREAT, 0644, 0);
	if (!status && table->death_lock == SEM_FAILED)
	{
		sem_close(table->forks);
		sem_close(table->print_lock);
		status = 1;
	}
	if (status)
		return (free(table), 1);
	return (0);
}

t_table	*init_table(char **argv)
{
	t_table	*table;

	table = (t_table *)malloc(sizeof(t_table));
	if (!table)
		return (0);
	table->num_philosophers = ft_atoi(argv[1]);
	table->pids = (pid_t *)malloc(table->num_philosophers * sizeof(pid_t));
	if (!table->pids)
	{
		free(table);
		return (0);
	}
	table->time_to_die = ft_atoi(argv[2]);
	table->time_to_eat = ft_atoi(argv[3]);
	table->time_to_sleep = ft_atoi(argv[4]);
	table->death_check = 1;
	table->start_time = current_time();
	if (argv[5])
		table->num_meals = ft_atoi(argv[5]);
	else
		table->num_meals = -1;
	if (init_semaphores(table))
		return (0);
	table->philosophers = init_philosophers(table);
	return (table);
}
