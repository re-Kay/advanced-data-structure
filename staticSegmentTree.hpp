#ifndef SSEGTREE_H
#define SSEGTREE_H

#include<vector>
#include<algorithm>

class staticSegmentTree{
    private:
        typedef std::pair<int, int> Segment;
        class Node{
            public:
                int val;
                std::vector<Segment> segments;
        };
        std::vector<Node> bsttree;
        void treeBuilder(std::vector<int> xy, int l, int u, int i);
        void slabHelper(const Segment s, int x, int y, int i);
    public:
        staticSegmentTree();
        staticSegmentTree(std::vector<Segment> segments);
        std::vector<Segment> query(int q);
};

staticSegmentTree::staticSegmentTree(){
};

staticSegmentTree::staticSegmentTree(std::vector<Segment> segments){
    std::vector<int> xylist;
    for (auto s:segments){
        xylist.push_back(s.first);
        xylist.push_back(s.second);
    }
    std::sort(xylist.begin(), xylist.end());
    this->bsttree.resize(xylist.size());
    treeBuilder(xylist, 0, xylist.size()-1, 0);

    for (auto s:segments){
        slabHelper(s, INT_MIN, INT_MAX, 0);
    }
};

void staticSegmentTree::treeBuilder(std::vector<int> xy, int l, int u, int i){
    if (u<l) return;
    int mid = l + (u-l+1)/2;
    this->bsttree[i].val= xy[mid];
    treeBuilder(xy, l, mid-1, 2*i+1);
    treeBuilder(xy, mid+1, u, 2*i+2);
};

void staticSegmentTree::slabHelper(const Segment s, int x, int y, int i){
    if (i) return ;
    if (s.first == x && s.second == y) {
        bsttree[i].segments.push_back(s);
        return ;
    }
    int w = bsttree[i].val;
    if (y < w) {
        slabHelper(s, x, y, 2*i+1);
        return ;
    }
    if (x < w){
        slabHelper(s, x, y, 2*i+2);
        return ;
    }
    slabHelper(s, x, w, 2*i+1);
    slabHelper(s, w, y, 2*i+1);
};

std::vector<staticSegmentTree::Segment> staticSegmentTree::query(int q){
    std::vector<Segment> ans;
    int i=0;
    while(i < bsttree.size()){
        for (Segment s: bsttree[i].segments) 
            ans.push_back(s);
        i = (q<bsttree[i].val)? 2*i+1: 2*i+2;
    }
    return ans;
}

#endif