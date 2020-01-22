# Advanced data structure

This project aims to build several advanced data structures that are covered in lecture. Topic includes b+ tree, interval tree, segment tree, kd-tree, etc.

Throughout the project, we will build on top of C++ STL, and hopefully learn some concepts like template, class, etc.

List of data structure:
* [B+ Tree](#b-tree)
* [Interval tree](#interval-tree-1d)
* [Segment tree](#segment-tree)
* [Kd tree](#kd-tree)
* [Priority search tree]()

## B+ Tree
B+ tree is a tree whose nodes have m/2 to m children, while all data are stored at the leaf nodes. It takes O(n) space. B+ tree currently supports
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
Static interval tree is implemented by binary search tree in array representation, together with sorted interval array inside the nodes of the tree. It takes O(n) space. Static interval tree currently supports:
* construct, O(n log n) time
* query, O(log n + k) time, where k is the output size

Dynamic interval tree is implemented by AVL tree, together with red-black tree(by STL::map) inside the nodes of the tree storing intervals. It takes O(n) space. Dynamic interval tree currently supports:
* construct, O(n log n) time
* insert, O(log n) time
* query, O(log n +k) time, where k is the output size
* ~~erase, O(log n)~~

[Go top](#Advanced-data-structure)
## Segment Tree
Static segment tree is a balanced BST where each interval [x, y) is stored in at most O(log n) nodes. It can quickly report the intervals that contains the query point. It takes O(n log n) space. Static segment tree currently supports:
* construct, O(n log n) time
* query, O(log n + k) time, where k is the output size

Dynamic segment tree is implemented via logarithmic method. It takes O(n log<sup>2</sup> n) space. Dynamic segment tree currently supports:
* construct, O(n log n) time
* insert, O(log^2 n) amortized time
* ~~deletion, O(log n) amortized time~~
* query, O(log<sup>2</sup> n + k) time

[Go top](#Advanced-data-structure)
## K-d Tree
K-d tree stores high dimension points in O(n) space and answers orthogonal range query efficiently. It is a binary tree where points are separated evenly to store in the subtree. Static k-d tree currently supports:
Suppose there are n points in k-dimensional space.
* (In progress to optimize) construct, O(n log<sup>2</sup> n)
* query, O($$\sqrt{n}$$)

Dynamic k-d tree is implemented via logarithmic method. It takes O(n) space. Dynamic segment tree currently supports:
* construct, O(n log n) time
* insert, O(log<sup>2</sup> n) amortized time
* ~~deletion, O(log<sup>2</sup> n) amortized time~~
* query, O(log<sup>2</sup> n + k) time
[Go top](#Advanced-data-structure)