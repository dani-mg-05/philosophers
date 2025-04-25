/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damedina <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 17:44:25 by damedina          #+#    #+#             */
/*   Updated: 2024/10/19 17:45:19 by damedina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <limits.h>
# include <sys/time.h>

# define THINK "is_thinking"
# define FORK "has taken a fork"
# define EAT "is eating"
# define SLEEP "is sleeping"
# define DIE "died"

typedef struct s_table	t_table;

typedef struct s_philosopher
{
	int				id;
	pthread_t		thread;
	int				left_fork;
	int				right_fork;
	pthread_mutex_t	meal_mutex;
	int				meals;
	long			last_meal_time;
	t_table			*table;
}	t_philosopher;

typedef struct s_table
{
	int				num_philosophers;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	int				num_meals;
	int				death_flag;
	int				ended_threads;
	long long		start_time;
	t_philosopher	*philosophers;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	end_mutex;
	pthread_t		death_check;
}	t_table;

pthread_mutex_t	*init_forks(int num_philos);
t_philosopher	*init_philosophers(t_table *table);
t_table			*init_table(char **argv);
void			*philosopher_life(void *arg);
void			*deathcheck_life(void *arg);
long long		current_time(void);
int				ft_atoi(const char *str);
int				print_usage(void);
int				print_message(int id, char *msg, t_table *table);

#endif
