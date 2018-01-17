#ifndef _SET_H
#define _SET_H

#define SETSIZE 1000

struct Set{
    int elem[SETSIZE];
};

void s_init(struct Set *s);
void s_insert(struct Set *s, int e);
void s_print(struct Set *s);
void s_erase(struct Set *s, int e);
int s_has(struct Set *s, int e);
void s_plus(struct Set *s1, struct Set *s2);
void s_minus(struct Set *s1, struct Set *s2);
void s_clone(struct Set **new, struct Set *old);
struct Set *s_newSet(struct Set *s);
int s_isEmpty(struct Set *s);
int s_equal(struct Set *s1, struct Set *s2);

#endif