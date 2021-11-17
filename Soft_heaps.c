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
int sizeofn(struct node *t){
    if(t->ofkeys==NULL) return 0;
    else{
        struct list *s= t->ofkeys;
        int x=1;
        while(s->next!=NULL) {
            s=s->next;x++;
        }
        return x;
    }
}
int lengthoftree(struct listoftrees *t){
    if(t==NULL) return 0;
    else{
        int x=0;
        struct listoftrees *temp = t;
        while(temp!=NULL){
            x++;
            temp = temp->next;
        }
        return x;
    }
}
struct list * concatenate(struct list *t1,struct list *t2){
    if(t1==NULL||t2==NULL) {
        return (t1==NULL)? t2 : t1;
    }
    struct list * temp = t1;
    
    while(temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = t2;
    return t1;
}

struct list * insert_in_simplell(struct list *l,int k){
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
    return l;
}

struct listoftrees *initialize_listoftrees(struct listoftrees *t ,int key1,int ckey1){
    t = (struct listoftrees *)malloc(sizeof(struct listoftrees));
    t->rank = 1;
    t->x = (struct node*)malloc(sizeof(struct node));
    t->x->ckey = ckey1;
    t->x->left = NULL;
    t->x->right = NULL;
    t->x->ofkeys = insert_in_simplell(t->x->ofkeys,key1);
    t->x->ofkeys->next=NULL;
    t->next = NULL;
}

void shiftfromleft(struct node *t){
    if(leaf(t)) return ;
    else if(sizeofn(t) < softsize/2){
        if(t->left==NULL ){//|| (t->right==NULL && t->left->ckey > t->right->ckey)){
            struct node *temp=NULL;
            t->left = t->right;
            t->right = temp;
        }
        struct list * s = NULL;
        if(sizeofn(t->left) > softsize/2){
            int x = sizeofn(t->left) - (softsize/2);
            s = t->left->ofkeys;
            while(x--) s = s->next;
        }
        else s = t->left->ofkeys;
        t->ofkeys = concatenate(t->ofkeys,s);
        if(t->ckey < t->left->ckey) t->ckey = t->left->ckey;
        if(sizeofn(t->left) <= softsize/2) t->left->ofkeys=NULL;
        else{
            int x = sizeofn(t->left) - (softsize/2);
            x--;
            s = t->left->ofkeys;
            while(x--) s = s->next;
            s->next = NULL;
        }
        //if(leaf(t->left) && sizeofn(t->left)==0) t->left=NULL;
        if(t->left!=NULL) shiftfromleft(t->left);
    }
}

void shiftfromright(struct node *t){
    if(leaf(t)) return ;
    else if(sizeofn(t) < softsize/2){
        if(t->right==NULL ){//|| (t->right==NULL && t->left->ckey > t->right->ckey)){
            struct node *temp=NULL;
            t->right = t->left;
            t->left = temp;
        }
        struct list * s = NULL;
        if(sizeofn(t->right) > softsize/2){
            int x = sizeofn(t->right) - (softsize/2);
            s = t->right->ofkeys;
            while(x--) s = s->next;
        }
        else s = t->right->ofkeys;
        t->ofkeys = concatenate(t->ofkeys,s);
        if(t->ckey < t->right->ckey) t->ckey = t->right->ckey;
        if(sizeofn(t->right) <= softsize/2) t->right->ofkeys=NULL;
        else{
            int x = sizeofn(t->right) - (softsize/2);
            x--;
            s = t->right->ofkeys;
            while(x--) s = s->next;
            s->next = NULL;
        }
        //if(leaf(t->right) && sizeofn(t->right)==0) t->right=NULL;
        if(t->right!=NULL) shiftfromright(t->right);
    }
}

struct node * combineoperation(struct node *t1, struct node * t2){
    struct node *t = NULL;
    t = (struct node *)malloc(sizeof(struct node));
    t->left = t1;
    t->right = t2;
    t->ckey = (t1->ckey>=t2->ckey) ? t1->ckey : t2->ckey;
    t->ofkeys=NULL;
    shiftfromleft(t);
    shiftfromright(t);
    return t;
}
struct listoftrees* meld(struct listoftrees *t1,struct listoftrees* t2){
    struct listoftrees *t =NULL;
    t= (struct listoftrees *)malloc(sizeof(struct listoftrees));
    if(t1!=NULL && t2!=NULL && t1->rank>t2->rank ){
            t->rank = t1->rank;
            t->x = t1->x;
            t1 = t1->next;             
        }
        else if(t1!=NULL && t2!=NULL && t2->rank > t1->rank){
            t->rank = t2->rank;
            t->x = t2->x;
            t2 = t2->next;
        }
        else if(t1!=NULL && t2!=NULL && t1->rank==t2->rank){
            t->rank=t1->rank+1;
            t->x = combineoperation(t1->x,t2->x);
            t1 = t1->next;
            t2 = t2->next;
        }
        t->next = NULL;
    struct listoftrees *temp = NULL;
    temp = (struct listoftrees *)malloc(sizeof(struct listoftrees));
    while(t1!=NULL && t2!=NULL){
        if(t1->rank>t2->rank){
            temp->rank = t1->rank;
            temp->x = t1->x;
            t1 = t1->next;             
        }
        else if(t2->rank > t1->rank){
            temp->rank = t2->rank;
            temp->x = t2->x;
            t2 = t2->next;
        }
        else if(t1->rank==t2->rank){
            temp->rank=t1->rank+1;
            temp->x = combineoperation(t1->x,t2->x);
            t1 = t1->next;
            t2 = t2->next;
        }
        temp->next = t;
        t = temp;
    }
    if(t1==NULL){
        while(t2!=NULL){
            temp->rank = t2->rank;
            temp->x = t2->x;
            t2 = t2->next;
            temp->next = t;
            t = temp;
        }
    }
    else if(t2==NULL){
        while(t1!=NULL){
            temp->rank = t1->rank;
            temp->x = t1->x;
            t1 = t1->next;
            temp->next = t;
            t = temp;
        }
    }
    return t;
}

struct list * deletefrmll(struct list*t,int n){
    struct list * s1 = t;
    struct list * s2 = NULL;
    if(t!=NULL)  s2 = t->next;
    if(s1->key==n){
        s1->next=NULL;
        t=s2;
        return t;
    }
    while(s2!=NULL){
        if(s2->key==n){
            s1 = s2->next;
            break;
        }
        s1=s1->next;
        s2=s2->next;
    }
    return t;
}
struct node* deletefrmnode(struct node*t,int n,int *p){
    struct list*s=t->ofkeys;
    while(s!=NULL){
        if(s->key==n){
            t->ofkeys = deletefrmll(t->ofkeys,n);
            *p = 1;
        }
        s=s->next;
    }
    if((*p)==1){
        printf("Deleted %d\n",n);
        return t;
    }
    if((*p)!=1 && t->left!=NULL && t->left->ckey >= n) deletefrmnode(t->left,n,p);
    if((*p)!=1 && t->right!=NULL&&t->right->ckey >= n) deletefrmnode(t->right,n,p);
    return t;
}
struct listoftrees * delete (struct listoftrees *t,int n){
    struct listoftrees *temp = t;
    int k=0;
    while(k==0 && temp!=NULL){
        if(temp->x->ckey < n) temp = temp->next;
        else{
            deletefrmnode(temp->x,n,&k);
            if(k==1) break;
            else temp=temp->next;
        }
    }
    return t;
}

void inordertraverse(struct node*t){
    if(t!=NULL){
        printf("Value of ckey : %d\n",t->ckey);
        struct list * temp = t->ofkeys;
        printf("Data present at node is: ");
        if(temp==NULL) printf("-");
        while(temp!=NULL){
            printf(" %d,",temp->key);
            temp = temp->next;
        }
        printf("\n");
        inordertraverse(t->left);
        inordertraverse(t->right);
    }
}

void traverse(struct listoftrees *t){
    printf("The list of trees is of the size : %d\n",lengthoftree(t));
    int x = lengthoftree(t);
    for(int i=0;i<x;i++){
        printf("\n****Rank of the tree is : %d****\n",t->rank);
        inordertraverse(t->x);
        t = t->next;
    }
}
void addifleaf(struct node*t,struct list**ofleaves){
    if(t!=NULL && leaf(t)){
        (*ofleaves) = insert_in_simplell(*ofleaves,t->ckey);
        return;
    }
    addifleaf(t->left,ofleaves);
    addifleaf(t->right,ofleaves);
}
void minckey(struct listoftrees*t){
    struct list * ofleaves = NULL;
    //ofleaves = (struct list *)malloc(sizeof(struct list ));
    for(int i=0;i<lengthoftree(t);i++){
        addifleaf(t->x,&ofleaves);
        t=t->next;
    }
    int min=100000;
    while(ofleaves!=NULL){
        if(min>ofleaves->key) min = ofleaves->key;
        ofleaves = ofleaves->next;
    }
    printf("Minimum ckey is %d\n",min);
}

int main(){
    struct listoftrees *mytrees = NULL;
    mytrees = initialize_listoftrees(mytrees,25,30);
    
    struct listoftrees *other=NULL;
    other = initialize_listoftrees(other,8,32);
    mytrees = meld(mytrees,other);
    
    struct listoftrees *mytrees1 = NULL;
    mytrees1 = initialize_listoftrees(mytrees1,2,9);
    
    struct listoftrees *other1=NULL;
    other1 = initialize_listoftrees(other1,5,10);
    mytrees1 = meld(mytrees1,other1);

    struct listoftrees * tree = NULL;
    tree = meld(mytrees,mytrees1);
    traverse(tree);
       
}