#ifndef __TREE_H__
#define __TREE_H__


/*
    The user struct consists of pointers to the left and right wing siblings and a pointer to the first and last son.
    It also has a list of movie preferences.
*/

#include "list_struct.h"

struct user {

    struct user *parent;
    struct user *son, *last_son;
    struct user *left, *right;
    struct list *movies;
};


void addUser (int parent_id, int id);

void delUser (int id);

void alloc_tree (void);

void addMovie (int id, long long movie_rate);

void delMovie (int id, long long movie_rate);

void marathon (int id, long long k);

void delete_tree (struct user *tree);

void clear_data (void);


#endif /*__TREE_H__*/
