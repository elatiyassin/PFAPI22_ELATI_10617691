#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
// #include <time.h>

#define DIM 20

int dimension;
unsigned int cur_words;
char *new_string;
struct main_tree *main_t_structure;
struct main_new_tree *new_tree;


enum nodeColor {
    RED,
    BLACK
};

struct main_tree {
    int color;
    struct main_tree *link[2];
    char string[DIM];
};

struct main_new_tree {
    int color;
    char *string;
    bool active;
    struct main_new_tree *link[2];
};

struct no_char_tree {
    char c;
    struct no_char_tree *next;
};

struct node {
    char c;
    int n;
    struct node *next;
};

struct composite_node {
    char c;
    int n;
    int pos;
    struct composite_node *next;
};


struct node *insert_node(struct node *root, char c, int n) {
    struct node *l = root;
    while(l != NULL){
        if(l->c == c && l->n == n){
            return root;
        }
        l = l->next;
    }
    struct node *new_node = malloc(sizeof(struct node));
    new_node->n = n;
    new_node->c = c;
    new_node->next = root;
    return new_node;
}

struct composite_node *insert_node2(struct composite_node *root, char c, int n, int pos) {
    struct composite_node *l = root;
    while(l != NULL){
        if(l->c == c && l->n == n && l->pos == pos){
            return root;
        }
        l = l->next;
    }
    struct composite_node *new_node = malloc(sizeof(struct composite_node));
    new_node->n = n;
    new_node->pos = pos;
    new_node->c = c;
    new_node->next = root;
    return new_node;
}


struct no_char_tree *insert_no_char_tree(struct no_char_tree *root, char c) {
    struct no_char_tree *l = root;
    while(l != NULL){
        if(l->c == c){
            return root;   
        }
        l = l->next;
    }
    struct no_char_tree *new_node = malloc(sizeof(struct no_char_tree));
    new_node->c = c;
    new_node->next = root;
    return new_node;
}

void main_insert(char *string) {
    if (!main_t_structure) {
        struct main_tree *new_main_node = malloc(sizeof(struct main_tree));
        strcpy(new_main_node->string, string);
        new_main_node->color = RED;
        new_main_node->link[0] = new_main_node->link[1] = NULL;
        main_t_structure = new_main_node;
        return;
    }
    struct main_tree *ptr, *xPtr, *yPtr;
    int dir[98], ht = 0, index;
    ptr = main_t_structure;
    struct main_tree *stack[98];
    stack[ht] = main_t_structure;
    dir[ht++] = 0;
    while (ptr != NULL) {
        if (strcmp(string, ptr->string) > 0)
            index = 1;
        else
            index = 0;
        stack[ht] = ptr;
        ptr = ptr->link[index];
        dir[ht++] = index;
    }
    struct main_tree *new_main_node = malloc(sizeof(struct main_tree));
    strcpy(new_main_node->string, string);
    new_main_node->color = RED;
    new_main_node->link[0] = new_main_node->link[1] = NULL;
    stack[ht - 1]->link[index] = new_main_node;
    while ((ht >= 3) && (stack[ht - 1]->color == RED)) {
        if (dir[ht - 2] == 0) {
            yPtr = stack[ht - 2]->link[1];
            if (yPtr != NULL && yPtr->color == RED) {
                stack[ht - 2]->color = RED;
                stack[ht - 1]->color = yPtr->color = BLACK;
                ht = ht - 2;
            } else {
                if (dir[ht - 1] == 0) {
                    yPtr = stack[ht - 1];
                } else {
                    xPtr = stack[ht - 1];
                    yPtr = xPtr->link[1];
                    xPtr->link[1] = yPtr->link[0];
                    yPtr->link[0] = xPtr;
                    stack[ht - 2]->link[0] = yPtr;
                }
                xPtr = stack[ht - 2];
                xPtr->color = RED;
                yPtr->color = BLACK;
                xPtr->link[0] = yPtr->link[1];
                yPtr->link[1] = xPtr;
                if (xPtr == main_t_structure) {
                    main_t_structure = yPtr;
                } else {
                    stack[ht - 3]->link[dir[ht - 3]] = yPtr;
                }
                break;
            }
        } else {
            yPtr = stack[ht - 2]->link[0];
            if ((yPtr != NULL) && (yPtr->color == RED)) {
                stack[ht - 2]->color = RED;
                stack[ht - 1]->color = yPtr->color = BLACK;
                ht = ht - 2;
            } else {
                if (dir[ht - 1] == 1) {
                    yPtr = stack[ht - 1];
                } else {
                    xPtr = stack[ht - 1];
                    yPtr = xPtr->link[0];
                    xPtr->link[0] = yPtr->link[1];
                    yPtr->link[1] = xPtr;
                    stack[ht - 2]->link[1] = yPtr;
                }
                xPtr = stack[ht - 2];
                yPtr->color = BLACK;
                xPtr->color = RED;
                xPtr->link[1] = yPtr->link[0];
                yPtr->link[0] = xPtr;
                if (xPtr == main_t_structure) {
                    main_t_structure = yPtr;
                } else {
                    stack[ht - 3]->link[dir[ht - 3]] = yPtr;
                }
                break;
            }
        }
    }
    main_t_structure->color = BLACK;
}


void main_insert2(char *string) {
    struct main_tree *ptr, *xPtr, *yPtr;
    int dir[98], ht = 0, index;
    ptr = main_t_structure;
    if (!main_t_structure) {
        struct main_tree *new_main_node = malloc(sizeof(struct main_tree));
        strcpy(new_main_node->string, string);
        new_string = new_main_node->string;
        new_main_node->color = RED;
        new_main_node->link[0] = new_main_node->link[1] = NULL;
        main_t_structure = new_main_node;
        return;
    }
    struct main_tree *stack[98];
    stack[ht] = main_t_structure;
    dir[ht++] = 0;
    while (ptr != NULL) {
        if (strcmp(string, ptr->string) > 0)
            index = 1;
        else
            index = 0;
        stack[ht] = ptr;
        ptr = ptr->link[index];
        dir[ht++] = index;
    }
    struct main_tree *new_main_node = malloc(sizeof(struct main_tree));
    strcpy(new_main_node->string, string);
    new_string = new_main_node->string;
    new_main_node->color = RED;
    new_main_node->link[0] = new_main_node->link[1] = NULL;
    stack[ht - 1]->link[index] = new_main_node;
    while ((ht >= 3) && (stack[ht - 1]->color == RED)) {
        if (dir[ht - 2] == 0) {
            yPtr = stack[ht - 2]->link[1];
            if (yPtr != NULL && yPtr->color == RED) {
                stack[ht - 2]->color = RED;
                stack[ht - 1]->color = yPtr->color = BLACK;
                ht = ht - 2;
            } else {
                if (dir[ht - 1] == 0) {
                    yPtr = stack[ht - 1];
                } else {
                    xPtr = stack[ht - 1];
                    yPtr = xPtr->link[1];
                    xPtr->link[1] = yPtr->link[0];
                    yPtr->link[0] = xPtr;
                    stack[ht - 2]->link[0] = yPtr;
                }
                xPtr = stack[ht - 2];
                xPtr->color = RED;
                yPtr->color = BLACK;
                xPtr->link[0] = yPtr->link[1];
                yPtr->link[1] = xPtr;
                if (xPtr == main_t_structure) {
                    main_t_structure = yPtr;
                } else {
                    stack[ht - 3]->link[dir[ht - 3]] = yPtr;
                }
                break;
            }
        } else {
            yPtr = stack[ht - 2]->link[0];
            if ((yPtr != NULL) && (yPtr->color == RED)) {
                stack[ht - 2]->color = RED;
                stack[ht - 1]->color = yPtr->color = BLACK;
                ht = ht - 2;
            } else {
                if (dir[ht - 1] == 1) {
                    yPtr = stack[ht - 1];
                } else {
                    xPtr = stack[ht - 1];
                    yPtr = xPtr->link[0];
                    xPtr->link[0] = yPtr->link[1];
                    yPtr->link[1] = xPtr;
                    stack[ht - 2]->link[1] = yPtr;
                }
                xPtr = stack[ht - 2];
                yPtr->color = BLACK;
                xPtr->color = RED;
                xPtr->link[1] = yPtr->link[0];
                yPtr->link[0] = xPtr;
                if (xPtr == main_t_structure) {
                    main_t_structure = yPtr;
                } else {
                    stack[ht - 3]->link[dir[ht - 3]] = yPtr;
                }
                break;
            }
        }
    }
    main_t_structure->color = BLACK;
}

void main_new_insert(char *string) {
    struct main_new_tree *ptr, *xPtr, *yPtr;
    int dir[98], ht = 0, index;
    ptr = new_tree;
    if (!new_tree) {
        struct main_new_tree *new_main_node = malloc(sizeof(struct main_new_tree));
        new_main_node->string = string;
        new_main_node->color = RED;
        new_main_node->active = true;
        new_main_node->link[0] = new_main_node->link[1] = NULL;
        new_tree = new_main_node;
        return;
    }
    struct main_new_tree *stack[98];
    stack[ht] = new_tree;
    dir[ht++] = 0;
    while (ptr != NULL) {
        if (strcmp(string, ptr->string) > 0)
            index = 1;
        else
            index = 0;
        stack[ht] = ptr;
        ptr = ptr->link[index];
        dir[ht++] = index;
    }
    struct main_new_tree *new_main_node = malloc(sizeof(struct main_new_tree));
    new_main_node->string = string;
    new_main_node->color = RED;
    new_main_node->active = true;
    new_main_node->link[0] = new_main_node->link[1] = NULL;
    stack[ht - 1]->link[index] = new_main_node;
    while ((ht >= 3) && (stack[ht - 1]->color == RED)) {
        if (dir[ht - 2] == 0) {
            yPtr = stack[ht - 2]->link[1];
            if (yPtr != NULL && yPtr->color == RED) {
                stack[ht - 2]->color = RED;
                stack[ht - 1]->color = yPtr->color = BLACK;
                ht = ht - 2;
            } else {
                if (dir[ht - 1] == 0) {
                    yPtr = stack[ht - 1];
                } else {
                    xPtr = stack[ht - 1];
                    yPtr = xPtr->link[1];
                    xPtr->link[1] = yPtr->link[0];
                    yPtr->link[0] = xPtr;
                    stack[ht - 2]->link[0] = yPtr;
                }
                xPtr = stack[ht - 2];
                xPtr->color = RED;
                yPtr->color = BLACK;
                xPtr->link[0] = yPtr->link[1];
                yPtr->link[1] = xPtr;
                if (xPtr == new_tree) {
                    new_tree = yPtr;
                } else {
                    stack[ht - 3]->link[dir[ht - 3]] = yPtr;
                }
                break;
            }
        } else {
            yPtr = stack[ht - 2]->link[0];
            if ((yPtr != NULL) && (yPtr->color == RED)) {
                stack[ht - 2]->color = RED;
                stack[ht - 1]->color = yPtr->color = BLACK;
                ht = ht - 2;
            } else {
                if (dir[ht - 1] == 1) {
                    yPtr = stack[ht - 1];
                } else {
                    xPtr = stack[ht - 1];
                    yPtr = xPtr->link[0];
                    xPtr->link[0] = yPtr->link[1];
                    yPtr->link[1] = xPtr;
                    stack[ht - 2]->link[1] = yPtr;
                }
                xPtr = stack[ht - 2];
                yPtr->color = BLACK;
                xPtr->color = RED;
                xPtr->link[1] = yPtr->link[0];
                yPtr->link[0] = xPtr;
                if (xPtr == new_tree) {
                    new_tree = yPtr;
                } else {
                    stack[ht - 3]->link[dir[ht - 3]] = yPtr;
                }
                break;
            }
        }
    }
    new_tree->color = BLACK;
}

bool main_search(struct main_tree *l, char s[DIM]) {
    if (l != NULL) {
        int comparison = strcmp(s, l->string);
        if (comparison == 0)
            return true;
        else if (comparison > 0)
            return main_search(l->link[1], s);
        else
            return main_search(l->link[0], s);
    }
    return false;
}


void match_print_active(struct main_new_tree *l) {
    if (l != NULL) {
        match_print_active(l->link[0]);
        if (l->active) {
            fwrite(l->string, 1, dimension, stdout);
            putchar('\n');
        }
        match_print_active(l->link[1]);
    }
}

void match_print_active_old(struct main_tree *l) {
    if (l != NULL) {
        match_print_active_old(l->link[0]);
        fwrite(l->string, 1, dimension, stdout);
        putchar('\n');
        match_print_active_old(l->link[1]);
    }
}

char *string_read(char *s) {
    return fgets(s, DIM, stdin);
}

bool string_search_char(char x, const char ref[DIM]) {
    for (int i = 0; i < dimension; i++) {
        if (ref[i] == x)
            return true;
    }
    return false;
}

int string_count_char(char c, const char ref[DIM]) {
    int n = 0;
    for (int i = 0; i < dimension; ++i) {
        if (ref[i] == c)
            n++;
    }
    return n;
}

bool filter1(char *s, struct no_char_tree *l) {
    while (l != NULL) {
        for (int i = 0; i < dimension; i++) {
            if (s[i] == l->c){
                return true;
            }
        }
        l = l->next;
    }
    return false;
}

bool filter2(char *s, struct node *l) {
    while (l != NULL) {
        if(s[l->n] != l->c){
            return true;
        }
        l = l->next;
    }
    return false;
}


bool filter4(char *s, struct composite_node *l) {
    while (l != NULL) {
        if(s[l->pos] == l->c){
            return true;
        }
        if(string_count_char(l->c, s)<(l->n)){
            return true;
        }
        l = l->next;
    }
    return false;
}

bool filter5(char *s, struct composite_node *l) {
    while (l != NULL) {
        if(s[l->pos] == l->c){
            return true;
        }
        if(string_count_char(l->c, s)>(l->n)){
            return true;
        }
        l = l->next;
    }
    return false;
}


bool to_filter(char *s, struct no_char_tree *t1, struct node *t2, struct composite_node *t4, struct composite_node *t5) {
    if (filter2(s, t2))
        return true;
    else if (filter4(s, t4))
        return true;
    else if (filter5(s, t5))
        return true;
    else if (filter1(s, t1))
        return true;
    return false;
}

void filter_main(struct main_new_tree *t, struct no_char_tree *t1, struct node *t2,
                                  struct composite_node *t4, struct composite_node *t5) {
    if (t != NULL) {
        if (t->active) {
            if (to_filter(t->string, t1, t2, t4, t5) == true) {
                t->active = false;
                cur_words--;
            }
        }

        filter_main(t->link[0], t1, t2, t4, t5);
        filter_main(t->link[1], t1, t2, t4, t5);
    }
}

void insert_new_tree(struct main_tree *t, struct no_char_tree *t1, struct node *t2, struct composite_node *t4, struct composite_node *t5) {
    if (t != NULL) {
        insert_new_tree(t->link[1], t1, t2, t4, t5);
        insert_new_tree(t->link[0], t1, t2, t4, t5);
        if (to_filter(t->string, t1, t2, t4, t5) == false) {
            main_new_insert(t->string);
            cur_words++;
        }
    }
}

void free_tree(struct main_new_tree *t) {
    if (t != NULL) {
        free_tree(t->link[1]);
        free_tree(t->link[0]);
        free(t);
    }
}

void free_tree_old(struct main_tree *t) {
    if (t != NULL) {
        free_tree_old(t->link[1]);
        free_tree_old(t->link[0]);
        free(t);
    }
}

void free_t1(struct no_char_tree *l) {
    if(l!=NULL){
        free_t1(l->next);
        free(l);
    }
}

void free_t2(struct node *l) {
    if(l!=NULL){
        free_t2(l->next);
        free(l);
    }
}

void free_t4_t5(struct composite_node *l) {
    if(l!=NULL){
        free_t4_t5(l->next);
        free(l);
    }
}

void match_new() {
    cur_words = 0;
    char ref[DIM];
    string_read(ref);
    char str_in[DIM];
    string_read(str_in);
    int max_words = atoi(str_in);
    struct no_char_tree *t1 = NULL;
    struct node *t2 = NULL;
    struct composite_node *t4 = NULL;
    struct composite_node *t5 = NULL;
    new_tree = NULL;
    string_read(str_in);
    while (new_tree == NULL) {
        if (str_in[0] == '+' && str_in[1] == 'n') {
            free_tree(new_tree);
            match_new();
            return;
        } else if (str_in[0] == '+' && str_in[1] == 's') {
            match_print_active_old(main_t_structure);
            string_read(str_in);
        } else if (str_in[0] == '+' && str_in[1] == 'i') {
            string_read(str_in);
            while (!(str_in[0] == '+' && str_in[1] == 'i')) {
                main_insert(str_in);
                string_read(str_in);
            }
            string_read(str_in);
        } else if (strcmp(str_in, ref) == 0) {
            fwrite("ok\n", 1, 3, stdout);
            free_t1(t1);
            free_t2(t2);
            free_t4_t5(t4);
            free_t4_t5(t5);
            free_tree(new_tree);
            return;
        } else if (main_search(main_t_structure, str_in) == true && max_words != 0) {
            max_words--;
            for (int i = 0; i < dimension; ++i) {
                if (str_in[i] == '\0' || str_in[i] == '\n') {
                    break;
                } else {
                    if (string_search_char(str_in[i], ref) == false) {
                        putchar('/');
                        t1 = insert_no_char_tree(t1, str_in[i]);
                    } else {
                        if (str_in[i] == ref[i]) {
                            putchar('+');
                            t2 = insert_node(t2, str_in[i], i);
                        } else {
                            int x = 0, y = 0, z = 0;
                            for (int j = 0; j < dimension; ++j) {
                                if (ref[j] == str_in[i])
                                    x++;
                                if (str_in[i] == str_in[j])
                                    if (str_in[j] == ref[j])
                                        y++;
                                if (j <= i)
                                    if (str_in[i] == str_in[j])
                                        if (str_in[j] != ref[j])
                                            z++;
                            }
                            if (y <= x - z) {
                                putchar('|');
                                t4 = insert_node2(t4, str_in[i], z + y, i);
                            } else {
                                putchar('/');
                                t5 = insert_node2(t5, str_in[i], x, i);
                            }
                        }
                    }
                }
            }
            insert_new_tree(main_t_structure, t1, t2, t4, t5);
            printf("\n%d\n", cur_words);
            if(max_words == 0){
                fwrite("ko\n", 1, 3, stdout);
                free_t1(t1);
                free_t2(t2);
                free_t4_t5(t4);
                free_t4_t5(t5);
                free_tree(new_tree);
                return;
            }
        } else if (max_words == 0){
                fwrite("ko\n", 1, 3, stdout);
                free_t1(t1);
                free_t2(t2);
                free_t4_t5(t4);
                free_t4_t5(t5);
                free_tree(new_tree);
                return;
        } else {
            fwrite("not_exists\n", 1, 11, stdout);
            string_read(str_in);
        }
    }
    while (fgets(str_in, DIM, stdin) != NULL) {
         if (str_in[0] == '+' && str_in[1] == 'n') {
            free_tree(new_tree);
            match_new();
            break;
        } else if (str_in[0] == '+' && str_in[1] == 's') {
            match_print_active(new_tree);
        } else if (str_in[0] == '+' && str_in[1] == 'i') {
            string_read(str_in);
            while (!(str_in[0] == '+' && str_in[1] == 'i')) {
                main_insert2(str_in);
                if (to_filter(str_in, t1, t2, t4, t5) == false) {
                    main_new_insert(new_string);
                    cur_words++;
                }
                string_read(str_in);
            }
        } else if (strcmp(str_in, ref) == 0) {
                fwrite("ok\n", 1, 3, stdout);
                break;
        } else if (main_search(main_t_structure, str_in) == true) {
            max_words--;
            for (int i = 0; i < dimension; ++i) {
                if (str_in[i] == '\0' || str_in[i] == '\n') {
                    break;
                } else {
                    if (string_search_char(str_in[i], ref) == false) {
                        putchar('/');
                        t1 = insert_no_char_tree(t1, str_in[i]);
                    } else {
                        if (str_in[i] == ref[i]) {
                            putchar('+');
                            t2 = insert_node(t2, str_in[i], i);
                        } else {
                            int x = 0, y = 0, z = 0;
                            for (int j = 0; j < dimension; ++j) {
                                if (ref[j] == str_in[i])
                                    x++;
                                if (str_in[i] == str_in[j])
                                    if (str_in[j] == ref[j])
                                        y++;
                                if (j <= i)
                                    if (str_in[i] == str_in[j])
                                        if (str_in[j] != ref[j])
                                            z++;
                            }
                            if (y <= x - z) {
                                putchar('|');
                                t4 = insert_node2(t4, str_in[i], z + y, i);
                            } else {
                                putchar('/');
                                t5 = insert_node2(t5, str_in[i], x, i);
                            }
                        }
                    }
                }
            }
            filter_main(new_tree, t1, t2, t4, t5);
            printf("\n%d\n", cur_words);
            if (max_words == 0) {
                fwrite("ko\n", 1, 3, stdout);
                break;
            }
        } else {
            fwrite("not_exists\n", 1, 11, stdout);
        }
    }
    free_t1(t1);
    free_t2(t2);
    free_t4_t5(t4);
    free_t4_t5(t5);
    free_tree(new_tree);
}


int main() {
    // clock_t begin = clock();
    main_t_structure = NULL;
    char str_in[DIM];
    string_read(str_in);
    dimension = atoi(str_in);
    string_read(str_in);
    while (str_in[0] != '+') {
        main_insert(str_in);
        string_read(str_in);
    }
    do{
        if (str_in[0] == '+' && str_in[1] == 'n') {
            match_new();
        } else if (str_in[0] == '+' && str_in[1] == 'i'){
                string_read(str_in);
                while (!(str_in[0] == '+' && str_in[1] == 'i')) {
                    main_insert(str_in);
                    string_read(str_in);
                }
        } else if(str_in[0] == '+' && str_in[1] == 's') {
            match_print_active_old(main_t_structure);
        } 
    }while (fgets(str_in, DIM, stdin) != NULL);

    free_tree_old(main_t_structure);
    // clock_t end = clock();
    // double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    // printf("%f",time_spent);
    return 0;
}

