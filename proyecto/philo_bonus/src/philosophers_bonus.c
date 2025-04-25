/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damedina <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 00:28:15 by damedina          #+#    #+#             */
/*   Updated: 2025/04/21 00:28:24 by damedina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers_bonus.h"

static int	parse_args(int argc, char **argv)
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

static void	wait_children(t_table *table)
{
	int	status;
	int	i;

	i = -1;
	while (++i < table->num_philosophers)
		waitpid(table->pids[i], &status, 0);
	table->death_check = 0;
	sem_post(table->death_lock);
}

static void	*death_check(void *arg)
{
	t_table	*table;
	int		i;

	table = (t_table *)arg;
	sem_wait(table->death_lock);
	i = -1;
	while (++i < table->num_philosophers)
	{
		if (table->death_check)
			kill(table->pids[i], SIGKILL);
		else
			break ;
	}
	return (0);
}

int	start_philo(t_table *table)
{
	pthread_t	death_thread;
	pid_t		pid;
	int			i;

	pthread_create(&death_thread, NULL, death_check, (void *)table);
	i = -1;
	while (++i < table->num_philosophers)
	{
		table->philosophers[i].id = i + 1;
		pid = fork();
		if (pid == -1)
		{
			pthread_join(death_thread, NULL);
			return (1);
		}
		if (pid == 0)
			philosopher_life(&table->philosophers[i]);
		else
			table->pids[i] = pid;
	}
	wait_children(table);
	pthread_join(death_thread, NULL);
	return (0);
}

int	main(int argc, char **argv)
{
	t_table	*table;

	if (parse_args(argc, argv))
		return (1);
	table = init_table(argv);
	if (!table || !table->philosophers)
		return (1);
	start_philo(table);
	return (exit_philo(table));
}
