#include <stdio.h>
#include <stdlib.h>

#include "set.h"
#include "setup.h"
#include "error.h"

int size = SETSIZE;

void s_insert(struct Set *s, int e){
    s->elem[e] = 1;
}

void s_erase(struct Set *s, int e){
    s->elem[e] = 0;
}

int s_has(struct Set *s, int e){
    return s->elem[e];
}

int s_equal(struct Set *s1, struct Set *s2){
    int i;
    for(i = 0; i < size; i++){
        if(s2->elem[i] != s1->elem[i]) return 0;
    }
    return 1;
}

void s_plus(struct Set *s1, struct Set *s2){
    int i;
    for(i = 0; i < size; i++){
        if(s2->elem[i]) s1->elem[i] = 1;
    }
}
void s_minus(struct Set *s1, struct Set *s2){
    int i;
    for(i = 0; i < size; i++){
        if(s2->elem[i] && s1->elem[i]) s1->elem[i] = 0;
    }
}

void s_clone(struct Set **new, struct Set *old){
    *new = (struct Set *)malloc(sizeof(struct Set));
    int i = 0;
    for(i = 0; i < size; i++){
        (*new)->elem[i] = old->elem[i];
    }
}

void s_print(struct Set *s){
    int i;
    int flag = 1;
    printf("[");
    for(i = 0; i < size; i++){
        if(s->elem[i]){
            if(flag){
                printf("%s", symbolMap[i]);
                flag = 0;
            }else{
                printf(", %s", symbolMap[i]);
            }
        }
    }
    printf("]\n");
}

void s_init(struct Set *s){
    int i;
    for(i = 0; i < size; i++) s->elem[i] = 0;
}

struct Set *s_newSet(struct Set *s){
    struct Set *new;
    int i = 0;

    new = (struct Set *)malloc(sizeof(struct Set));
    
    if(new == 0){
        fatal(3);
    }

    if(s == 0){
        s_init(new);
    }else{
        for(i = 0; i < size; i++){
            new->elem[i] = s->elem[i];
        }
    }
    return new;
}

int s_isEmpty(struct Set *s){
    int i;
    for(i = 0; i < size; i++){
        if(s->elem[i] != 0) return 0;
    }
    return 1;
}
