# Soft-heaps-implementation-and-analysis



                                                       Soft Heap Implementation



Tummala Sanjana Reddy-2020csb1137                                                                                             
Shruti Gangwar - 2020csb1127
Shailja Nigam-2020csb1124


Instructor:
Dr. Anil Shukla

Teaching Assistent:
Ravi Bhatt


Summary-
Soft heap is a simple varient of priority queue.It suppots insert,meld,minckey,delete and inorder traversal operations.It consists of linked list of trees. 
Each tree consists of node containing ckey value,rank and it's left and right pointers pointing to it's left and right child node.Each node has a linked list
which contains key less than or equal to node's ckey value .The size of linked list is constraint by fixed softsize.Each tree is heap ordered with respect to
ckey values of nodes such that ckey(of node)<=ckey(of left child) & ckey(of node)<=ckey(of left child) if exist.If tree rank is z then it's left and right
child if exists will have rank z-1.


