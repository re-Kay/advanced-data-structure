#ifndef DSEGTREE_H
#define DSEGTREE_H
#include "staticSegmentTree.hpp"
#include <vector>
#include <list>
#include <iostream>

class SegmentTree{
    private:
        typedef std::pair<int, int> Segment;
        typedef std::vector<Segment> Segments;
        class Record{
            public:
                std::list<SegmentTree::Segment> segmentSet;
                StaticSegmentTree segTree;
                const int MAXSIZE;
                bool isempty;
                Record(int s): MAXSIZE(s), isempty(true){};
                int size(){ if (isempty) return 0; return segmentSet.size(); }
        };
        std::vector<Record> tree;
    public:
        SegmentTree();
        SegmentTree(Segments sl);
        void insert(Segment s);
        Segments query(int q);
        void print();
};

SegmentTree::SegmentTree(){
};

SegmentTree::SegmentTree(Segments sl){
    int size = 1;
    while (sl.size()>size){
        Record temp(size);
        tree.push_back(temp);
        size <<= 1;
    }
    Record temp(size);
    temp.segmentSet.assign(sl.begin(), sl.end());
    temp.segTree = StaticSegmentTree(sl);
    temp.isempty = false;
    tree.push_back(temp);
};

SegmentTree::Segments SegmentTree::query(int q){
    Segments ans;
    for (auto r: tree){
        if (!r.isempty){
            Segments temp = r.segTree.query(q);
            ans.reserve(ans.size()+temp.size());
            ans.insert(ans.end(), temp.begin(), temp.end());
        }
    }
    return ans;
};

void SegmentTree::insert(Segment s){
    // find the proper segment tree to rebuild
    int i;
    int newSize = 0;
    std::list<Segment> toBeMoved;
    // lazy delete
    for (i=0; i<tree.size(); i++){
        newSize += tree[i].size();
        if (1+newSize > tree[i].MAXSIZE){
            if (!tree[i].isempty){
                toBeMoved.splice(toBeMoved.end(), tree[i].segmentSet);
                tree[i].segmentSet.clear();
            }
            tree[i].isempty = true;
        } else break;
    }
    toBeMoved.push_back(s);
    // rebuild
    int size;
    if (i==tree.size()){
        size = 1 << i;
        Record temp(size);
        if (i==tree.size()) tree.push_back(temp);
    }
    tree[i].isempty = false;
    tree[i].segmentSet.splice(tree[i].segmentSet.end(), toBeMoved);
    Segments segVec(tree[i].segmentSet.begin(), tree[i].segmentSet.end());
    tree[i].segTree = StaticSegmentTree(segVec);
}

void SegmentTree::print(){
    for (int i=0; i<tree.size(); i++){
        std::cout<< "Node "<< i<<" :[";
        for (auto s:tree[i].segmentSet){
            std::cout << "(" << s.first << ", "<< s.second << ")";
        }
        std::cout<< "]\n";
    }
}
#endif