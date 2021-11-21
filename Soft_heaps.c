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

//Finds whether a node is leaf or not
int leaf(struct node *t){
    if(t->left==NULL && t->right==NULL) return 1;
    else return 0;
}
//Returns the size of linkedlist present at the node.
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
//Returns number of node in th tree.
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
//It takes two linked lists and returns the concatenated linked list.
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

//It takes a key value and pointer to the linked list and inserts in the same.
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

//This function initializes the list of trees which is called as soft heaps.
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

//This function checks the minimum keys in the node if not present it 
//will shift the keys which are in linked list of left node to the parent node.
void shiftfromleft(struct node *t){
    if(leaf(t)) return ;
    else if(sizeofn(t) < softsize/2){
        if(t->left==NULL ){
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
        
        if(t->left!=NULL) shiftfromleft(t->left);
    }
}

//This function checks the minimum keys in the node if not present it 
//will shift the keys which are in linked list of right node to the parent node.
void shiftfromright(struct node *t){
    if(leaf(t)) return ;
    else if(sizeofn(t) < softsize/2){
        if(t->right==NULL ){
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
        
        if(t->right!=NULL) shiftfromright(t->right);
    }
}

//It combines the two nodes of same rank into one node of rank+1.
//It calls the two values shiftfromleft and shiftfromright .
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

//It takes two list of trees and make them into one list of tree.
//It calles combine operation if ranks are equal.
struct listoftrees* meld(struct listoftrees *t1,struct listoftrees* t2){
    if(t1==NULL&& t2==NULL) return NULL;
    else if(t1==NULL || t2==NULL){
        return (t1==NULL)? t2 : t1;
    }
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

//It is a simple delete function in the linked ist.
void deletefrmll(struct list ** head_ref, int key){
    struct list *temp = *head_ref, *prev;

    if (temp != NULL && temp->key == key) {
        *head_ref = temp->next;
        return;
    }
    while (temp != NULL && temp->key != key) {
        prev = temp;
        temp = temp->next;
    }
    if (temp == NULL)
        return;
    prev->next = temp->next;
}

//It takes the value of the key to be deleted and node.
//It calls the deletefrmll function if key is found.
struct node* deletefrmnode(struct node*t,int n,int *p){
    struct list*s=t->ofkeys;
    while(s!=NULL){
        if(s->key==n){
            deletefrmll(&(t->ofkeys),n);
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

//This is the main delete function uses above two functions to delete 
//the key entered.
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
    if(temp==NULL && k==0) printf("Data not found.\n");
    return t;
}

//It is an inorder traversal just as done in the binary trees.
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

//This function goes through all the trees in the list and 
//traverses each node by calling above function.
void traverse(struct listoftrees *t){
    printf("The list of trees is of the size : %d\n",lengthoftree(t));
    int x = lengthoftree(t);
    for(int i=0;i<x;i++){
        printf("\n****Rank of the tree is : %d ****\n",t->rank);
        inordertraverse(t->x);
        t = t->next;
    }
}

//This function adds a nodes ckey value if it is a leaf.
void addifleaf(struct node*t,struct list**ofleaves){
    if(t!=NULL && leaf(t)){
        (*ofleaves) = insert_in_simplell(*ofleaves,t->ckey);
        return;
    }
    addifleaf(t->left,ofleaves);
    addifleaf(t->right,ofleaves);
}

//As the minimum ckeys are present at the leaves
//This function calls addifleaf function and lastly finds the minimum from the linked list.
void minckey(struct listoftrees*t){
    struct list * ofleaves = NULL;
    int g = lengthoftree(t);
    for(int i=0;i<g;i++){
        addifleaf(t->x,&ofleaves);
        t=t->next;
    }
    int min=2147483647;
    while(ofleaves!=NULL){
        if(min>ofleaves->key) min = ofleaves->key;
        ofleaves = ofleaves->next;
    }
    if(min!=2147483647) printf("Minimum ckey is %d\n",min);
}

int main(){
    char c = '\0';
    printf("Enter 'E' for operations on existing tree and 'C' for creating a tree - ");
    scanf(" %c",&c);
    //User can enter E or C based on his choice.
    if(c=='C'){
    int x=-1;
    struct listoftrees *temp = NULL;
    struct listoftrees *tree = NULL;
    while(x!=0){
        printf("Enter the number based on the choice.\n");
        printf("1 - for inserting in the tree\n");
        printf("2 - for deleting the element.\n");
        printf("3 - for printing the tree or traversal.\n");
        printf("4 - for getting minimum ckey.\n");
        printf("0 - to exit\n");
        scanf(" %d",&x);
        if(x==1) {
            //if x is one operation to be performed is inserting.
            printf("Enter the maximum limit of the key(ckey) and key value\n");
            int c = 0,k=0;
            scanf(" %d %d",&c,&k);
            temp = NULL;
            temp = initialize_listoftrees(temp,k,c);
            tree = meld(tree,temp);
        }
        else if(x==2){
            //if x is 2 operation to be performed is deleting.
            printf("Enter the data to delete  ");
            int x=0;scanf(" %d",&x);
            tree = delete(tree,x);
        }
        else if(x==3){
            //if x is 3 operation to be performed is traversing.
            traverse(tree);
        }
        else if(x==4) minckey(tree); 
        //if x is 4 operation to be performed is finding minimum ckey.
        else if(x==0) break;
        //if x is 0 it breaks the while loop.
    }
    }
    else if(c=='E'){
    printf("The below tree is a tree of ranks 3,2\n");
    struct listoftrees *temp = NULL;
    struct listoftrees *tree = NULL;
    int max =100,key=10;
    int i=2;
    //From lines 401 to 442 it creates a tree of rank 4 and 3.
    while(i--){
        temp = NULL;
        temp = initialize_listoftrees(temp,(key++),(max++));
        tree = meld(tree,temp);
    }
    struct listoftrees *tree1 = NULL;i=2;
    while(i--){
        temp = NULL;
        temp = initialize_listoftrees(temp,(key++),(max++));
        tree1 = meld(tree1,temp);
    }
    tree = meld(tree,tree1);
    struct listoftrees *tempo = NULL;
    struct listoftrees *treeo = NULL;
    max =200;key=20;
    i=2;
    while(i--){
        tempo = NULL;
        tempo = initialize_listoftrees(tempo,(key++),(max++));
        treeo = meld(treeo,temp);
    }
    struct listoftrees *treeo1 = NULL;i=2;
    while(i--){
        tempo = NULL;
        tempo = initialize_listoftrees(tempo,(key++),(max++));
        treeo1 = meld(treeo1,tempo);
    }
    treeo = meld(treeo,treeo1);
    tree = meld(tree,treeo);
    i=2;
    while(i--){
        temp = NULL;
        temp = initialize_listoftrees(temp,(key++),(max++));
        tree = meld(tree,temp);
    }
    tree1 = NULL;i=2;
    while(i--){
        temp = NULL;
        temp = initialize_listoftrees(temp,(key++),(max++));
        tree1 = meld(tree1,temp);
    }
    tree = meld(tree,tree1);
    i=2;
    traverse(tree);  
    int x=-1;
    while(x!=0){
        printf("Enter the number based on the choice.\n");
        printf("1 - for deleting the element.\n");
        printf("2 - for printing the tree or traversal.\n");
        printf("3 - for getting minimum ckey.\n");
        printf("0 - to exit\n");
        scanf(" %d",&x);
        if(x==1){
            //if x is one operation to be performed is deleting.
            printf("Enter the data to delete ");
            int x=0;scanf(" %d",&x);
            tree = delete(tree,x);
        }
        else if(x==2){
            //if x is 2 operation to be performed is traversing.
            traverse(tree);
        }
        else if(x==3) minckey(tree);
        //if x is 3 operation to be performed is finding minimum ckey.
        else if(x==0) break;
        //if x is 0 it breaks the while loop.
    }
    }
}
