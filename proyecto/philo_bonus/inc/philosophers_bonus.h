/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_bonus.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damedina <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 00:29:04 by damedina          #+#    #+#             */
/*   Updated: 2025/04/21 00:29:08 by damedina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_BONUS_H
# define PHILOSOPHERS_BONUS_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <limits.h>
# include <pthread.h>
# include <semaphore.h>
# include <fcntl.h>
# include <signal.h>
# include <sys/time.h>
# include <sys/wait.h>

# define THINK "is_thinking"
# define FORK "has taken a fork"
# define EAT "is eating"
# define SLEEP "is sleeping"
# define DIE "died"

typedef struct s_table	t_table;

typedef struct s_philosopher
{
	int			id;
	int			meals;
	long		last_meal_time;
	t_table		*table;
	pthread_t	death_thread;
}	t_philosopher;

typedef struct s_table
{
	int				num_philosophers;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	int				num_meals;
	int				death_check;
	long long		start_time;
	sem_t			*forks;
	sem_t			*print_lock;
	sem_t			*death_lock;
	pid_t			*pids;
	t_philosopher	*philosophers;
}	t_table;

t_philosopher	*init_philosophers(t_table *table);
t_table			*init_table(char **argv);
void			philosopher_life(t_philosopher *philosopher);
long long		current_time(void);
int				ft_atoi(const char *str);
int				print_usage(void);
int				print_message(int id, char *msg, t_table *table);
int				exit_philo(t_table *table);

#endif
