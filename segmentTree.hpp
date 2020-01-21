#ifndef DSEGTREE_H
#define DSEGTREE_H
#include "staticSegmentTree.hpp"
#include <vector>
#include <list>

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
    int p;

    // lazy delete
    for (int i=0; i<p; i++){
        tree[i].isempty = true;
    }

    // rebuild
    int size;
    if (p==tree.size()){
        size = tree[tree.size()-1].MAXSIZE << 1;
        Record temp(size);
        if (p==tree.size) tree.push_back(temp);
    }
    tree[p].isempty = false;
    tree[p].segmentSet = {};
    tree[p].segTree = StaticSegmentTree();
}
#endif