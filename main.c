#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define DIM 30

int dimension;
int max;
int cur_words;

enum nodeColor {
    RED,
    BLACK
};

struct main_tree {
    int color;
    char *string;
    bool active;
    struct main_tree *link[2];
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
    struct node *new_node = malloc(sizeof(struct node));
    new_node->n = n;
    new_node->c = c;
    new_node->next = root;
    return new_node;

}

struct composite_node *insert_node2(struct composite_node *root, char c, int n, int pos) {
    struct composite_node *new_node = malloc(sizeof(struct composite_node));
    new_node->n = n;
    new_node->pos = pos;
    new_node->c = c;
    new_node->next = root;
    return new_node;

}


struct no_char_tree *insert_no_char_tree(struct no_char_tree *root, char c) {
    struct no_char_tree *new_node = malloc(sizeof(struct no_char_tree));
    new_node->c = c;
    new_node->next = root;
    return new_node;
}


struct main_tree *main_reset(struct main_tree *l) {
    if (l != NULL) {
        main_reset(l->link[0]);
        l->active = true;
        main_reset(l->link[1]);
    }
    return l;
}


int string_comparison(const char *s0, const char *s1) {
    int fast = dimension / sizeof(size_t) + 1;
    int offset = (fast - 1) * sizeof(size_t);
    int current_block = 0;
    if (dimension <= sizeof(size_t)) { fast = 0; }
    size_t *lptr0 = (size_t *) s0;
    size_t *lptr1 = (size_t *) s1;
    while (current_block < fast) {
        if ((lptr0[current_block] ^ lptr1[current_block])) {
            int pos;
            for (pos = current_block * sizeof(size_t); pos < dimension; ++pos) {
                if ((s0[pos] ^ s1[pos]) || (s0[pos] == 0) || (s1[pos] == 0)) {
                    return (int) ((unsigned char) s0[pos] - (unsigned char) s1[pos]);
                }
            }
        }
        ++current_block;
    }
    while (dimension > offset) {
        if ((s0[offset] ^ s1[offset])) {
            return (int) ((unsigned char) s0[offset] - (unsigned char) s1[offset]);
        }
        ++offset;
    }
    return 0;
}


struct main_tree *main_insert(struct main_tree *root, char *string, bool active) {
    struct main_tree *ptr, *xPtr, *yPtr;
    int dir[98], ht = 0, index;
    ptr = root;
    if (!root) {
        struct main_tree *new_node = malloc(sizeof(struct main_tree));
        new_node->string = malloc(dimension * sizeof(char));
        memcpy(new_node->string, string, dimension);
        new_node->color = RED;
        new_node->active = active;
        new_node->link[0] = new_node->link[1] = NULL;
        return new_node;
    }
    struct main_tree *stack[98];
    stack[ht] = root;
    dir[ht++] = 0;
    while (ptr != NULL) {
        if (string_comparison(string, ptr->string) > 0)
            index = 1;
        else
            index = 0;
        stack[ht] = ptr;
        ptr = ptr->link[index];
        dir[ht++] = index;
    }
    struct main_tree *new_node = malloc(sizeof(struct main_tree));
    new_node->string = malloc(dimension * sizeof(char));
    memcpy(new_node->string, string, dimension);
    new_node->color = RED;
    new_node->active = active;
    new_node->link[0] = new_node->link[1] = NULL;
    stack[ht - 1]->link[index] = new_node;
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
                if (xPtr == root) {
                    root = yPtr;
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
                if (xPtr == root) {
                    root = yPtr;
                } else {
                    stack[ht - 3]->link[dir[ht - 3]] = yPtr;
                }
                break;
            }
        }
    }
    root->color = BLACK;
    return root;
}

void string_print(char *a) {
    fwrite_unlocked(a, 1, dimension, stdout);
}


bool string_search_char(char x, const char ref[DIM]) {
    for (int i = 0; i < dimension; ++i) {
        if (ref[i] == x)
            return true;
    }
    return false;
}


bool main_search(struct main_tree *l, char s[DIM]) {
    if (l != NULL) {
        int comparison = string_comparison(s, l->string);
        if (comparison == 0)
            return true;
        else if (comparison > 0)
            return main_search(l->link[1], s);
        else
            return main_search(l->link[0], s);
    }
    return false;
}


void match_print_active(struct main_tree *l) {
    if (l) {
        match_print_active(l->link[0]);
        if (l->active == true) {
            string_print(l->string);
            printf("\n");
        }
        match_print_active(l->link[1]);
    }
}


char *string_read(char *s) {
    return fgets(s, DIM, stdin);
}


int string_count_char(char c, const char ref[DIM]) {
    int n = 0;
    for (int i = 0; i < dimension; ++i) {
        if (ref[i] == c)
            n++;
    }
    return n;
}


int string_atoi(const char *s) {
    int result = 0;
    for (int i = 0; s[i] != '\0' && s[i] != '\n'; i++) {
        result = result * 10 + (s[i] - '0');
    }
    return result;
}

bool f1(char c, char *s) {
    return string_search_char(c, s);
}

bool f2(char c, const char *s, int n) {
    return s[n] != c;
}

bool f3(char c, const char *s, int pos) {
    return s[pos] == c;
}

bool f4(char c, char *s, int n) {
    return string_count_char(c, s) < n;
}

bool f5(char c, char *s, int n) {
    return string_count_char(c, s) > n;
}

struct main_tree *f1_c(struct main_tree *t, char c) {
    if (t != NULL) {
        if (t->active)
            if (f1(c, t->string)) {
                t->active = false;
                cur_words--;
            }
        t->link[0] = f1_c(t->link[0], c);
        t->link[1] = f1_c(t->link[1], c);
    }
    return t;
}

struct main_tree *f2_c(struct main_tree *t, char c, int n) {
    if (t != NULL) {
        if (t->active)
            if (f2(c, t->string, n)) {
                t->active = false;
                cur_words--;
            }
        t->link[0] = f2_c(t->link[0], c, n);
        t->link[1] = f2_c(t->link[1], c, n);
    }
    return t;
}

struct main_tree *f4_c(struct main_tree *t, char c, int n, int pos) {
    if (t != NULL) {
        if (t->active) {
            if (f3(c, t->string, pos)) {
                t->active = false;
                cur_words--;
            } else if (f4(c, t->string, n)) {
                t->active = false;
                cur_words--;
            }
        }
        t->link[0] = f4_c(t->link[0], c, n, pos);
        t->link[1] = f4_c(t->link[1], c, n, pos);
    }
    return t;
}

struct main_tree *f5_c(struct main_tree *t, char c, int n, int pos) {
    if (t != NULL) {
        if (t->active) {
            if (f3(c, t->string, pos)) {
                t->active = false;
                cur_words--;
            } else if (f5(c, t->string, n)) {
                t->active = false;
                cur_words--;
            }
        }
        t->link[0] = f5_c(t->link[0], c, n, pos);
        t->link[1] = f5_c(t->link[1], c, n, pos);
    }
    return t;
}


bool filter1(char *s, struct no_char_tree *l) {
    while (l != NULL) {
        if (f1(l->c, s)) {
            return true;
        }
        l = l->next;
    }
    return false;
}

bool filter2(char *s, struct node *l) {
    while (l != NULL) {
        if (f2(l->c, s, l->n)) {
            return true;
        }
        l = l->next;
    }
    return false;
}


bool filter4(char *s, struct composite_node *l) {
    while (l != NULL) {
        if (f3(l->c, s, l->pos))
            return true;
        if (f4(l->c, s, l->n)) {
            return true;
        }
        l = l->next;
    }
    return false;
}

bool filter5(char *s, struct composite_node *l) {
    while (l != NULL) {
        if (f3(l->c, s, l->pos))
            return true;
        if (f5(l->c, s, l->n)) {
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

bool filter1_numbered(char *s, struct no_char_tree *l, int v) {
    while (v != 0) {
        if (f1(l->c, s)) {
            return true;
        }
        v--;
        l = l->next;
    }
    return false;
}

bool filter2_numbered(char *s, struct node *l, int v) {
    while (v != 0) {
        if (f2(l->c, s, l->n)) {
            return true;
        }
        v--;
        l = l->next;
    }
    return false;
}


bool filter4_numbered(char *s, struct composite_node *l, int v) {
    while (v != 0) {
        if (f3(l->c, s, l->pos))
            return true;
        if (f4(l->c, s, l->n)) {
            return true;
        }
        v--;
        l = l->next;
    }
    return false;
}

bool filter5_numbered(char *s, struct composite_node *l, int v) {
    while (v != 0) {
        if (f3(l->c, s, l->pos))
            return true;
        if (f5(l->c, s, l->n)) {
            return true;
        }
        v--;
        l = l->next;
    }
    return false;
}

bool to_filter_numbered(char *s, struct no_char_tree *t1, struct node *t2, struct composite_node *t4, struct composite_node *t5,
                        int v1, int v2, int v4, int v5) {
    if (filter2_numbered(s, t2, v2))
        return true;
    if (filter4_numbered(s, t4, v4))
        return true;
    if (filter5_numbered(s, t5, v5))
        return true;
    if (filter1_numbered(s, t1, v1))
        return true;
    return false;
}

struct main_tree *filter_main(struct main_tree *t, struct no_char_tree *t1, struct node *t2,
                              struct composite_node *t4, struct composite_node *t5, int v1, int v2, int v4, int v5) {
    if (t != NULL) {
        if (t->active)
            if (to_filter_numbered(t->string, t1, t2, t4, t5, v1, v2, v4, v5) == true) {
                t->active = false;
                cur_words--;
            }
        t->link[0] = filter_main(t->link[0], t1, t2, t4, t5, v1, v2, v4, v5);
        t->link[1] = filter_main(t->link[1], t1, t2, t4, t5, v1, v2, v4, v5);
    }
    return t;
}

struct main_tree *new_insert(struct main_tree *t, struct no_char_tree *t1, struct node *t2, struct composite_node *t4, struct composite_node *t5) {
    char str_in[DIM];
    string_read(str_in);
    while (str_in[0] != '+' && str_in[1] != 'i') {
        if (to_filter(str_in, t1, t2, t4, t5) == true) {
            t = main_insert(t, str_in, false);
        } else {
            t = main_insert(t, str_in, true);
            cur_words++;
        }
        max++;
        string_read(str_in);
    }
    return t;
}

struct main_tree *new_insert_true(struct main_tree *t) {
    char str_in[DIM];
    string_read(str_in);
    while (str_in[0] != '+' && str_in[1] != 'i') {
        t = main_insert(t, str_in, true);
        max++;
        string_read(str_in);
    }
    return t;
}


struct main_tree *match_new(struct main_tree *t) {
    cur_words = max;
    t = main_reset(t);
    char ref[DIM];
    string_read(ref);
    char str_in[DIM];
    string_read(str_in);
    int max_words = string_atoi(str_in);
    struct no_char_tree *t1 = NULL;
    struct node *t2 = NULL;
    struct composite_node *t4 = NULL;
    struct composite_node *t5 = NULL;
    int v1 = 0, v2 = 0, v4 = 0, v5 = 0;
    while (string_read(str_in) != NULL) {
        if (str_in[0] == '+' && str_in[1] == 'n') {
            t = match_new(t);
            break;
        }
        if (str_in[0] == '+' && str_in[1] == 'i') {
            t = new_insert(t, t1, t2, t4, t5);
        } else if (str_in[0] == '+' && str_in[1] == 's') {
            match_print_active(t);
        } else {
            if (string_comparison(str_in, ref) == 0) {
                printf("ok\n");
                return t;
            } else if (main_search(t, str_in) == true) {
                max_words--;
                int i;
                for (i = 0; i < dimension; ++i) {
                    if (str_in[i] == '\0' || str_in[i] == '\n') {
                        break;
                    } else {
                        if (string_search_char(str_in[i], ref) == false) {
                            printf("/");
                            t1 = insert_no_char_tree(t1, str_in[i]);
                            v1++;
                        } else {
                            if (str_in[i] == ref[i]) {
                                printf("+");
                                t2 = insert_node(t2, str_in[i], i);
                                v2++;
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
                                    printf("|");
                                    t4 = insert_node2(t4, str_in[i], z + y, i);
                                    v4++;
                                } else {
                                    printf("/");
                                    t5 = insert_node2(t5, str_in[i], x, i);
                                    v5++;
                                }
                            }
                        }
                    }
                }
                t = filter_main(t, t1, t2, t4, t5, v1, v2, v4, v5);
                printf("\n%d\n", cur_words);
            } else {
                printf("not_exists\n");
            }
            if (max_words == 0) {
                printf("ko\n");
                break;
            }
        }
    }
    return t;
}


int main() {
    max = 0;
    struct main_tree *t = NULL;
    char str_in[DIM];
    string_read(str_in);
    dimension = string_atoi(str_in);
    while (string_read(str_in) != NULL) {
        if (str_in[0] == '+')
            break;
        t = main_insert(t, str_in, true);
        max++;
    }
    t = match_new(t);
    while (string_read(str_in) != NULL) {
        if (str_in[0] == '+') {
            if (str_in[1] == 'n') {
                t = match_new(t);
            } else if (str_in[1] == 'i')
                t = new_insert_true(t);
            else if (str_in[1] == 's')
                match_print_active(t);
        }
    }
    return 0;
}