/* This file consists of a basic list struct 
   and a few functions to control it.
*/

#include "list_struct.h"

#include <stdio.h>
#include <stdlib.h>

/* This function is used to store movie ratings on the list in descending order in a linear time. 
*/

typedef struct list list;

void add_movie (long long movie_rate, list **films) {

    list *tmp = NULL;
    list *movies = *films, *head = *films;

    while (movies && movies->movie_rate > movie_rate) {
        tmp = movies;
        movies = movies->next;
    }

    if (movies && movies->movie_rate == movie_rate) {
        fprintf(stderr, "ERROR\n");
        return;
    }

    list *new = malloc(sizeof(list));

    if (!new) {

        exit(EXIT_FAILURE);
    }

    new->movie_rate = movie_rate;
    new->next = movies;

    if (tmp != NULL) {
        
        tmp->next = new;
        *films = head;
    } else {

        *films = new;
    }

    printf("OK\n");
    return;
}

// This is a function to add movies to a list while executing the marathon command.

void add_movie_to_marathon (long long movie_rate, list **films, int *exit_code) {

    list *tmp = NULL;
    list *movies = *films, *head = *films;

    while (movies && movies->movie_rate < movie_rate) {
        tmp = movies;
        movies = movies->next;
    }

    if (movies && movies->movie_rate == movie_rate) {
        *exit_code = 1;
        return;
    }

    list *new = malloc(sizeof(list));

    if (!new) {

        exit(EXIT_FAILURE);
    }

    new->movie_rate = movie_rate;
    new->next = movies;

    if (tmp != NULL) {
        
        tmp->next = new;
        *films = head;
    } else {

        *films = new;
    }

    return;
}

// This function deletes allt he movies that are left at the end.

void delete_movies (list *movies) {

    list *tmp = movies;

    while (movies) {
        tmp = movies->next;
        free(movies);
        movies = tmp;
    }    
}

// delete_movie is used to delete a single movie.

void delete_movie (long long movie_rate, list **films) {

    list *tmp = NULL;
    list *movies = *films, *head = *films;

    while (movies && movies->movie_rate != movie_rate) {
        tmp = movies;
        movies = movies->next;
    }

    // We have to check if we found the given movie_rate.

    if (!movies) {
        fprintf(stderr, "ERROR\n");
        return;
    }

    if (tmp != NULL) {

        tmp->next = movies->next;
        *films = head;
    } else {

        *films = head->next;
    }

    free(movies);
    printf("OK\n");
    return;
}