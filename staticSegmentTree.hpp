#ifndef SSEGTREE_H
#define SSEGTREE_H

#include<vector>
#include<algorithm>

class StaticSegmentTree{
    private:
        typedef std::pair<int, int> Segment;
        struct Node{
            int val;
            bool isLeaf;
            Segment slab;
            std::vector<Segment> segments;
        };
        std::vector<Node> bsttree;
        void treeBuilder(const std::vector<int>& xy, int lb, int ub, int i, Segment s);
        void insertHelper(const Segment &s, Segment cs, int i);
    public:
        StaticSegmentTree();
        StaticSegmentTree(std::vector<Segment> segments);
        std::vector<Segment> query(int q);
};

StaticSegmentTree::StaticSegmentTree(){
};

StaticSegmentTree::StaticSegmentTree(std::vector<Segment> segments){
    std::vector<int> xylist;
    for (auto s:segments){
        xylist.push_back(s.first);
        xylist.push_back(s.second);
    }
    std::sort(xylist.begin(), xylist.end());
    auto it = std::unique(xylist.begin(),xylist.end());
    xylist.erase(it, xylist.end());
    this->bsttree.resize(4*xylist.size());
    treeBuilder(xylist, 0, xylist.size()-1, 0, {INT_MIN, INT_MAX});

    for (auto s:segments){
        insertHelper(s, s, 0);
    }
};

void StaticSegmentTree::insertHelper(const Segment &s, Segment cs, int i){
    if (s.first==s.second){
        return ;
    }
    if (cs.first == bsttree[i].slab.first && bsttree[i].slab.second == cs.second){
        bsttree[i].segments.push_back(s);
        return;
    }
    if (cs.first >= bsttree[i].val){
        insertHelper(s, cs, 2*i+2);
        return;
    }
    if (cs.second <= bsttree[i].val){
        insertHelper(s, cs, 2*i+1);
        return;
    }
    insertHelper(s, {cs.first, bsttree[i].val}, 2*i+1);
    insertHelper(s, {bsttree[i].val, cs.second}, 2*i+2);
}

void StaticSegmentTree::treeBuilder(const std::vector<int>& xy, int lb, int ub, int i, Segment s){
    if (ub<lb) {
        bsttree[i].isLeaf = true;
        bsttree[i].slab = s;
        return;
    }
    int mid = lb + (ub-lb+1)/2;
    this->bsttree[i].val= xy[mid];
    bsttree[i].slab = s;
    bsttree[i].isLeaf = false;
    treeBuilder(xy, lb, mid-1, 2*i+1, {s.first, xy[mid]});
    treeBuilder(xy, mid+1, ub, 2*i+2, {xy[mid], s.second});
};


std::vector<StaticSegmentTree::Segment> StaticSegmentTree::query(int q){
    std::vector<Segment> ans;
    int i=0;
    while(!bsttree[i].isLeaf){
        for (Segment s: bsttree[i].segments) 
            ans.push_back(s);
        i = (q<bsttree[i].val)? 2*i+1: 2*i+2;
    }
    for(Segment s: bsttree[i].segments) ans.push_back(s);
    return ans;
}

#endif