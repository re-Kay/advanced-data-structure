# Advanced data structure

This project aims to build several advanced data structures that are covered in lecture. Topic includes b+ tree, interval tree, seqment tree, kd-tree, etc.

Throughout the project, we will build on top of C++ STL, and hopefully learn some concepts like template, class, etc.

List of data structure:
* [B+ Tree](#B+-Tree)
* [Interval tree](#Interval-Tree-(1D))

## B+ Tree
B+ tree is a tree whose nodes have m/2 to m children, while all data are stored at the leaf nodes. B+ tree currently supports
* insert, O(log n) time
* erase, O(log n) time
* search, O(log n) time
* range query, O(log n + k) time, where k is the output size
* ~~predecessor, O(log n) time~~
* ~~successor, O(log n) time~~
* join, O(log n) time
* ~~split, O(log n + |T|), where |T| is the size of resulting tree~~

[Go top](#Advanced-data-structure)
## Interval Tree (1D)
Static interval tree is implemented by binary search tree in array representation, together with sorted interval array inside the nodes of the tree. Static interval tree currently supports:
* construct, O(n log n) time
* query, O(log n + k) time, where k is the output size

Dynamic interval tree is implemented by AVL tree, together with red-black tree(by STL::map) inside the nodes of the tree storing intervals. Dynamic interval tree currently supports:
* construct, O(n log n) time
* insert, O(log n) time
* query, O(log n +k) time, where k is the output size
* ~~erase, O(log n)~~

[Go top](#Advanced-data-structure)