

#ifndef __LIST_H__
#define __LIST_H__

/*
    The list struct
*/

struct list {
    
    long long movie_rate;
    struct list *next;
};

void add_movie (long long movie_rate, struct list **movies);

void add_movie_to_marathon (long long movie_rate, struct list **movies, int *exit_code);

void delete_movies (struct list *movies);

void delete_movie (long long movie_rate, struct list **movies);

#endif /*__LIST_H__*/