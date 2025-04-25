/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_structs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damedina <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 17:40:41 by damedina          #+#    #+#             */
/*   Updated: 2025/03/23 17:40:45 by damedina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers.h"

pthread_mutex_t	*init_forks(int num_philos)
{
	pthread_mutex_t	*forks;
	int				i;

	forks = (pthread_mutex_t *)malloc(num_philos * sizeof(pthread_mutex_t));
	if (!forks)
		return (0);
	i = -1;
	while (++i < num_philos)
		pthread_mutex_init(&forks[i], NULL);
	return (forks);
}

t_philosopher	*init_philosophers(t_table *table)
{
	t_philosopher	*philos;
	int				num_philos;
	int				i;

	num_philos = table->num_philosophers;
	philos = (t_philosopher *)malloc((num_philos) * sizeof(t_philosopher));
	if (!philos)
		return (0);
	i = -1;
	while (++i < num_philos)
	{
		philos[i].id = i + 1;
		philos[i].meals = 0;
		philos[i].last_meal_time = current_time();
		philos[i].left_fork = i;
		philos[i].right_fork = (i + 1) % num_philos;
		philos[i].table = table;
		pthread_mutex_init(&philos[i].meal_mutex, NULL);
	}
	return (philos);
}

t_table	*init_table(char **argv)
{
	t_table	*table;

	table = (t_table *)malloc(sizeof(t_table));
	if (!table)
		return (0);
	table->num_philosophers = ft_atoi(argv[1]);
	table->time_to_die = ft_atoi(argv[2]);
	table->time_to_eat = ft_atoi(argv[3]);
	table->time_to_sleep = ft_atoi(argv[4]);
	table->start_time = current_time();
	table->death_flag = 0;
	table->ended_threads = 0;
	pthread_mutex_init(&table->print_mutex, NULL);
	pthread_mutex_init(&table->end_mutex, NULL);
	if (argv[5])
		table->num_meals = ft_atoi(argv[5]);
	else
		table->num_meals = -1;
	table->forks = init_forks(table->num_philosophers);
	if (!table->forks)
		return (table);
	table->philosophers = init_philosophers(table);
	if (!table->philosophers)
		free(table->forks);
	return (table);
}
