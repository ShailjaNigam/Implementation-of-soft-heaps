#include<stdio.h>
#include<stdlib.h>
#include<math.h>
int softsize = 10;
struct list{
    int key;
    struct list *next;
};
struct node {
    int ckey;
    int size;
    struct list *ofkeys;
    struct node *left;
    struct node *right;
};
struct listoftrees{
    int rank;
    struct node *x;
    struct listoftrees *next;
};

int leaf(struct node *t){
    if(t->left==NULL && t->right==NULL) return 1;
    else return 0;
}

void concatenate(struct list *t1,struct list *t2){
    struct list * temp = t2;
    while(temp->next != NULL) temp = temp->next;
    temp->next = t1;
    t1=t2;
}

//inserting in simple linked list
void insert_in_simplell(struct list *l,int k){
    if(l==NULL){
        l=(struct list *)malloc(sizeof(struct list));
        l->key = k;
        l->next=NULL;
    }
    else{
        struct list * temp = NULL;
        temp = (struct list *)malloc(sizeof(struct list));
        temp->key = k;
        temp->next = l;
        l = temp;
    }
}

void initialize_listoftrees(struct listoftrees *t ,int key1,int ckey1){
    t = (struct listoftrees *)malloc(sizeof(struct listoftrees));
    t->rank = 1;
    t->x = (struct node*)malloc(sizeof(struct node));
    t->x->ckey = ckey1;
    t->x->left = NULL;
    t->x->right = NULL;
    t->x->ofkeys = (struct list *)malloc(sizeof(struct list));
    t->x->ofkeys->key=key1;
    t->x->ofkeys->next = NULL;
    t->next = NULL;
}

void shiftoperation(struct node *t){
    while(t->size<softsize && !(leaf(t))){
        if(t->left==NULL || (t->right==NULL && t->left->ckey > t->right->ckey)){
            struct node *temp=t->left;
            t->left = t->right;
            t->right = temp;
        }
        concatenate(t->ofkeys,t->left->ofkeys);
        t->ckey = t->left->ckey;
        t->left->ofkeys=NULL;
        if(leaf(t->left)) t->left=NULL;
        else shiftoperation(t->left);
    }
}

int main(){
    
}