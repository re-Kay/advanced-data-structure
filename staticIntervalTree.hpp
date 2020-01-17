#ifndef ITVTREE_H
#define ITVTREE_H

#include <vector>
#include <algorithm>
#include <iostream>

class IntervalTree{
    private:
        typedef std::pair<int, int> Point;
        class Node{
            public:
                int val;
                std::vector<IntervalTree::Point> xsorted, ysorted;
        };
        std::vector<Node> bsttree;
        void helper(std::vector<int> xy, int l, int u, int i);
    public:
        IntervalTree();
        IntervalTree(std::vector<Point> intervals);
        std::vector<Point> query(int q);
        void printStabbingSet(int q);
};

IntervalTree::IntervalTree(){
    bsttree = {};
};

IntervalTree::IntervalTree(std::vector<Point> intervals){
    std::vector<int> xylist;
    for (auto s:intervals){
        xylist.push_back(s.first);
        xylist.push_back(s.second);
    }
    std::sort(xylist.begin(), xylist.end());
    this->bsttree.resize(xylist.size());
    helper(xylist, 0, xylist.size()-1, 0);

    for (auto s: intervals){
        bool notAssigned = true;
        int i=0;
        int x=s.first;
        int y=s.second;
        while(notAssigned){
            int w = bsttree[i].val;
            if (x<=w && w<=y){
                bsttree[i].xsorted.push_back(s);
                bsttree[i].ysorted.push_back(s);
                notAssigned = false;
            }
            if (w < x) i=2*i+2;
            if (y < w) i=2*i+1;
        }
    }
    for (int i=0; i<bsttree.size(); i++){
        std::sort(bsttree[i].xsorted.begin(), 
             bsttree[i].xsorted.end(), 
             [](Point s1, Point s2){return s1.first < s2.first;});
        std::sort(bsttree[i].ysorted.begin(), 
              bsttree[i].ysorted.end(), 
              [](Point s1, Point s2){return s1.second > s2.second;});
    }
};

void IntervalTree::helper(std::vector<int> xy, int l, int u, int i){
    if (u<l) return;
    int mid = l + (u-l+1)/2;
    this->bsttree[i].val= xy[mid];
    helper(xy, l, mid-1, 2*i+1);
    helper(xy, mid+1, u, 2*i+2);
};

std::vector<IntervalTree::Point> IntervalTree::query(int q){
    std::vector<Point> temp, s;
    int i=0, w;
    while(i < bsttree.size()){
        w = bsttree[i].val;
        if (q<w){
            s = bsttree[i].xsorted;
            for (int j=0; j<s.size() && s[j].first <= q; j++) temp.push_back(s[j]);
            i = 2*i+1;
        }
        if (q>=w){
            s = bsttree[i].ysorted;
            for (int j=0; j<s.size() && s[j].second >= q; j++) temp.push_back(s[j]);
            i = 2*i+2;
        }
    }
    return temp;
}

void IntervalTree::printStabbingSet(int q){
    int i=0;
    while(i<this->bsttree.size()){
        if (q==bsttree[i].val){
            for (int j=0; j<bsttree[i].xsorted.size(); j++){
                std::cout<< "("<<bsttree[i].xsorted[j].first<<", "<<bsttree[i].xsorted[j].second<<")";
            }
            std::cout << std::endl;
            break;    
        }
        else if (q<bsttree[i].val) i=2*i+1;
        else i=2*i+2;
    }
}
#endif