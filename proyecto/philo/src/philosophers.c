/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damedina <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 17:37:47 by damedina          #+#    #+#             */
/*   Updated: 2024/10/19 17:46:02 by damedina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers.h"

int	parse_args(int argc, char **argv)
{
	int	i;
	int	j;

	if (argc < 5 || argc > 6)
		return (print_usage());
	i = 0;
	while (++i < argc)
	{
		if (argv[i][0] == '\0')
			return (print_usage());
		j = -1;
		while (argv[i][++j] != '\0')
		{
			if (argv[i][j] < '0' || argv[i][j] > '9')
				return (print_usage());
		}
		if (ft_atoi(argv[i]) <= 0)
			return (print_usage());
	}
	return (0);
}

int	exit_philo(t_table *table)
{
	int	i;

	i = -1;
	while (++i < table->num_philosophers)
	{
		pthread_mutex_unlock(&table->forks[i]);
		pthread_mutex_destroy(&table->forks[i]);
		pthread_mutex_unlock(&table->philosophers[i].meal_mutex);
		pthread_mutex_destroy(&table->philosophers[i].meal_mutex);
	}
	pthread_mutex_unlock(&table->print_mutex);
	pthread_mutex_unlock(&table->print_mutex);
	pthread_mutex_destroy(&table->print_mutex);
	pthread_mutex_unlock(&table->end_mutex);
	pthread_mutex_destroy(&table->end_mutex);
	free(table->forks);
	free(table->philosophers);
	free(table);
	return (0);
}

int	start_philo(t_table *table)
{
	int	i;

	if (table->num_philosophers == 1)
	{
		print_message(1, THINK, table);
		pthread_mutex_lock(&table->forks[0]);
		print_message(1, FORK, table);
		usleep(table->time_to_die);
		print_message(1, DIE, table);
		pthread_mutex_unlock(&table->forks[0]);
	}
	else
	{
		i = -1;
		while (++i < table->num_philosophers)
			pthread_create(&table->philosophers[i].thread, NULL,
				philosopher_life, (void *)&table->philosophers[i]);
		pthread_create(&table->death_check, 0, deathcheck_life, (void *)table);
		pthread_join(table->death_check, NULL);
		i = -1;
		while (++i < table->num_philosophers)
			pthread_join(table->philosophers[i].thread, NULL);
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_table	*table;

	if (parse_args(argc, argv))
		return (1);
	table = init_table(argv);
	if (!table || !table->forks || !table->philosophers)
		return (1);
	start_philo(table);
	return (exit_philo(table));
}
