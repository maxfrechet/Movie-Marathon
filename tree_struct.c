
#include "tree_struct.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MINUS -1
#define MIN_RATE 0
#define MIN_ID 0
#define USER_NUM 65536

typedef struct user User;
typedef struct list list;

// We will be using an array of pointers to users.

User *Users[USER_NUM] = {NULL};
User *tree = NULL;

User * getUser (int id) {

    return Users[id];
}

bool isUserNULL (int id) {

    return Users[id] == NULL;
}

void set_NULLs (User *id) {

    id->movies = NULL;
    id->left = NULL;
    id->right = NULL;
    id->son = NULL;
    id->last_son = NULL;
    id->parent = NULL;
}

void alloc_tree (void) {

    Users[MIN_ID] = malloc(sizeof(User));
    tree = Users[MIN_ID];
    set_NULLs(getUser(MIN_ID));
}

void addUser (int parent_id, int id) {

    if (getUser(id) && getUser(id) == tree) {

        // Which means it is the top of the tree.

        fprintf(stderr, "ERROR\n");
        return;
    }

    if ((parent_id == id) || isUserNULL(parent_id) || getUser(id)) {

        fprintf(stderr, "ERROR\n");
        return;
    }

    // We create a new node for parent_id.

    Users[id] = malloc(sizeof(User));
    
    // We have to set the last son.

    set_NULLs(getUser(id));

    if (getUser(parent_id)->son == NULL) {

        getUser(parent_id)->last_son = getUser(id);
        getUser(parent_id)->son = getUser(id);
    }
    else {

        getUser(id)->right = getUser(parent_id)->son;
        getUser(parent_id)->son->left = getUser(id);

        getUser(parent_id)->son = getUser(id);
    }

    getUser(id)->parent = getUser(parent_id);

    printf("OK\n");
    return;
}


// Doing another version of delUser 

void delUser (int id) {

    // We cannot delete the user going by the id 0 or a user that is not added to the tree.

    if (!getUser(id) || getUser(id) == tree) {

        fprintf(stderr, "ERROR\n");
        return;
    }

    // We might need to add something here.

    if (getUser(id)->left == NULL && getUser(id)->right == NULL) {

        getUser(id)->parent->son =  getUser(id)->son;
        getUser(id)->parent->last_son = getUser(id)->last_son;

        if (getUser(id)->son) {

            getUser(id)->last_son->parent = getUser(id)->parent;
            getUser(id)->son->parent = getUser(id)->parent;
        }


    } else if (getUser(id)->left == NULL) {

        if (getUser(id)->son) {

            getUser(id)->right->left = getUser(id)->last_son;
            getUser(id)->last_son->right = getUser(id)->right;
            getUser(id)->parent->son = getUser(id)->son;
            getUser(id)->son->parent = getUser(id)->parent;
        } else {

            getUser(id)->parent->son = getUser(id)->right;
            getUser(id)->right->parent = getUser(id)->parent;
            getUser(id)->right->left = NULL;
        }
    } else if (getUser(id)->right == NULL) {

        if (getUser(id)->son) {

            getUser(id)->left->right = getUser(id)->son;
            getUser(id)->son->left = getUser(id)->left;

            getUser(id)->parent->last_son = getUser(id)->last_son;            
            getUser(id)->last_son->parent = getUser(id)->parent;
        } else {

            getUser(id)->left->right = NULL;
            getUser(id)->parent->last_son = getUser(id)->left;

            getUser(id)->left->parent = getUser(id)->parent;    
        }
    } else {

        if (getUser(id)->son) {

            getUser(id)->left->right = getUser(id)->son;
            getUser(id)->son->left = getUser(id)->left;
            getUser(id)->right->left = getUser(id)->last_son;
            getUser(id)->last_son->right = getUser(id)->right;
        }
        else {

            getUser(id)->left->right = getUser(id)->right;
            getUser(id)->right->left = getUser(id)->left;

        }
    }

    delete_movies(Users[id]->movies);
    free(Users[id]);
    Users[id] = NULL;

    printf("OK\n");
    return;
}

void addMovie (int id, long long movie_rate) {

    // If the user is not assigned to the tree then we have return an ERROR.

    if (!getUser(id)) {

        fprintf(stderr, "ERROR\n");
        return;
    }

    // Using a premade function from the list.h modul

    add_movie(movie_rate, &(Users[id]->movies));
    return;
}

void delMovie (int id, long long movie_rate) {

    // If the user is not assigned to the tree then we have return an ERROR.

    if (!getUser(id)) {

        fprintf(stderr, "ERROR\n");
        return;
    }

    delete_movie(movie_rate, &(Users[id]->movies));
    return;
}

long long min (long long num1, long long num2) {

    if (num1 <= num2)
        return num1;
    return num2;
}

long long max (long long num1, long long num2) {

    if (num1 <= num2)
        return num2;
    return num1;
}

// This is an additional function which is used while executing the marathon command.

void marathon_run (long long min_rating, list **movies, User *id, long long k, long long *count, long long min_value) {

    User *tmp = id->son;
    long long rating = min_rating;

    if (id->movies) {

        rating = max(id->movies->movie_rate, min_rating);
    }

    long long minimum = min_value;
    int exit_code = 0;
    list *tmp_list = id->movies;

    while (*count < k && tmp_list && tmp_list->movie_rate > min_rating) {

        exit_code = 0;
        add_movie_to_marathon(tmp_list->movie_rate, movies, &exit_code);

        if (!exit_code) {
            
            (*count)++;

            if (minimum == MINUS) {

                minimum = tmp_list->movie_rate;
            }
            if (tmp_list->movie_rate < minimum) {

                minimum = tmp_list->movie_rate;
            }
        }
        tmp_list = tmp_list->next;
    }

    while (tmp_list && tmp_list->movie_rate > minimum && *count == k) {

        if (k > 1) {

            minimum = min(tmp_list->movie_rate, (*movies)->next->movie_rate);
        } else {

            minimum = tmp_list->movie_rate;
        }

        list *var = (*movies)->next;
        free(*movies);
        *movies = var;

        add_movie_to_marathon(tmp_list->movie_rate, movies, &exit_code);
        tmp_list = tmp_list->next;
    }


    while (tmp) {

        marathon_run(rating, movies, tmp, k, count, minimum);
        tmp = tmp->right;
    }
}

long long size_list (list *movies) {

    long long size = 0;

    while (movies) {

        size++;
        movies = movies->next;
    }
    return size;
}

void marathon (int id, long long k) {

    if (!Users[id]) {

        fprintf(stderr, "ERROR\n");
        return;
    }

    if (k == MIN_RATE) {

        printf("NONE\n");
        return;
    }

    long long min_rating = MINUS;

    if (getUser(id)->movies) {

        min_rating = getUser(id)->movies->movie_rate;
    }

    list *movies = NULL;
    long long count = 0;
    int exit_code = 0;

    marathon_run(min_rating, &movies, getUser(id), k, &count, MINUS);

    // Adding the movies from the first list if count < k.

    list *movie_list = getUser(id)->movies;

    while (count < k && movie_list) {

        add_movie_to_marathon(movie_list->movie_rate, &movies, &exit_code);
        movie_list = movie_list->next;

        count++;
    }

    list *var = NULL;
    long long number_of_changes = 0;

    while (number_of_changes < k && movie_list && movies->movie_rate < movie_list->movie_rate) {

        var = movies->next;
        free(movies);
        movies = var;

        add_movie_to_marathon(movie_list->movie_rate, &movies, &exit_code);
        movie_list = movie_list->next;
        number_of_changes++;
    }

    // Printing the list or NONE to the screen.

    if (movies) {
        
        long long size = size_list(movies), i = 0;
        long long array[size];

        list *head = movies;

        while (movies) {
    
            array[i++] = movies->movie_rate;
            movies = movies->next;
        }

        i--;

        while (i > MIN_RATE) {

            printf("%lld ", array[i--]);
        }

        printf("%lld", array[i]);   
        delete_movies(head);
    } else {

        printf("NONE");
    }
    printf("\n");
    return;
}

// Writing an additional function to clear the movie tree.

void delete_tree (User *tree) {

    User *tmp = tree->son;
    User *var  = tmp;

    while (tmp) {
        var = tmp->right;
        delete_tree(tmp);
        tmp = var;
    }
    
    delete_movies(tree->movies);
    free(tree);
}

void clear_data (void) {

    if (!tree) {
        
        return;
    }

    delete_tree(tree);
    return;
}
