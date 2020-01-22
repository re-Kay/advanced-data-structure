#ifndef DKDTREE_H
#define DKDTREE_H

#include "staticKDtree.hpp"
#include <vector>
#include <list>

class KDTree{
    private:
        typedef std::vector<double> Point;
        typedef std::vector<Point> Points;
        class Record{
            public:
                std::list<Point> pointSet;
                StaticKDTree kdtree;
                const int MAXSIZE;
                bool isempty;
                Record(int s): MAXSIZE(s), isempty(true){};
                int size(){ if (isempty) return 0; return pointSet.size(); }
        };
        std::vector<Record> tree;
    public:
        KDTree() = default;
        KDTree(Points pl);
        void insert(Point p);
        Points query(const Point &bl, const Point &tr);
};

KDTree::KDTree(Points pl){
    int size = 1;
    while (pl.size()>size){
        Record temp(size);
        tree.push_back(temp);
        size <<= 1;
    }
    Record temp(size);
    temp.pointSet.assign(pl.begin(), pl.end());
    temp.kdtree = StaticKDTree(pl);
    temp.isempty = false;
    tree.push_back(temp);
}

void KDTree::insert(Point s){
    // find the proper kdtree tree to rebuild
    int i;
    int newSize = 0;
    std::list<Point> toBeMoved;
    // lazy delete
    for (i=0; i<tree.size(); i++){
        newSize += tree[i].size();
        if (1+newSize > tree[i].MAXSIZE){
            if (!tree[i].isempty){
                toBeMoved.splice(toBeMoved.end(), tree[i].pointSet);
                tree[i].pointSet.clear();
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
    tree[i].pointSet.splice(tree[i].pointSet.end(), toBeMoved);
    Points pointVec(tree[i].pointSet.begin(), tree[i].pointSet.end());
    tree[i].kdtree = StaticKDTree(pointVec);
}

KDTree::Points KDTree::query(const Point &bl, const Point &tr){
    Points ans;
    for (auto r: tree){
        if (!r.isempty){
            Points temp = r.kdtree.query(bl, tr);
            ans.reserve(ans.size()+temp.size());
            ans.insert(ans.end(), temp.begin(), temp.end());
        }
    }
    return ans;
};

#endif