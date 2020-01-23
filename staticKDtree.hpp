#ifndef SStaticKDTree_H
#define SStaticKDTree_H

#include <vector>
#include <algorithm>

#ifdef _DEBUG
#include <iostream>
#include <queue>
#endif

class StaticKDTree{
    private:
        typedef std::vector<double> Point;
        typedef std::vector<Point> Points;
        class Node{
            public:
                Point data;
                bool hasData;
                double location;
                Node *left, *right;
                Point rectangleBound_bl;
                Point rectangleBound_tr;
                Node() : hasData(false){};
        };
        struct Comparator{
            int index;
            Comparator(int id) : index(id){};
            bool operator()(const StaticKDTree::Point &a , const StaticKDTree::Point &b) {
                return a[index]<b[index];
            };
        };
        Node* treeBuilder(Points::iterator &begin, Points::iterator &end, int length,int index);
        Node* root;
        Points::iterator partition(Points::iterator begin, Points::iterator end, int length, int index);
        Points queryHelper(Node* r, const Point &bl, const Point &tr);
        bool isOverlapped(const Point &bl1, const Point &tr1, const Point &bl2, const Point &tr2);
    public:
        StaticKDTree() = default;
        StaticKDTree(Points pl);
        Points query(const Point &bl, const Point &tr);
        void print();

};

StaticKDTree::StaticKDTree(Points pl){
    auto s = pl.begin();
    auto t = pl.end();
    root = treeBuilder(s, t, pl.size(), 0);
};

StaticKDTree::Node* StaticKDTree::treeBuilder(Points::iterator &begin, Points::iterator &end, int length, int index){
    if (length == 0){
        return NULL;
    }
    int dim = begin->size();
    Node* temp = new Node();
    if (length == 1){
        temp->data = *begin;
        temp->hasData = true;
        temp->rectangleBound_bl = temp->rectangleBound_tr = temp->data;
        return temp;
    }
    int attemp = 0;
    int dist;
    Points::iterator mid;
    while (length >= 10 && attemp < 6) { // in 6n time, with 95% success, in practice 99.95% success
        mid = partition(begin, end, length, index);
        dist = std::distance(begin, mid);
        if (dist>= 0.3* length && dist <= 0.7*length) break;
        attemp ++;
    }
    if (length < 10 || attemp == 6){ // worst case O(nlogn), if length<10, then O(1)
        std::sort(begin, end, Comparator(index));
        dist = length/2;
        mid = begin + dist;
    }
    double median = mid->at(index);
    temp->location = median;
    temp->left = treeBuilder(begin, mid, dist, (index+1)%dim);
    temp->right = treeBuilder(mid, end, length-dist, (index+1)%dim);
    for (int i=0; i<dim; i++){
        double v;
        if (temp->left->rectangleBound_bl[i] < temp->right->rectangleBound_bl[i])
            v = temp->left->rectangleBound_bl[i];
        else v = temp->right->rectangleBound_bl[i];    
        temp->rectangleBound_bl.push_back(v);
        if (temp->left->rectangleBound_tr[i] > temp->right->rectangleBound_tr[i])
            v = temp->left->rectangleBound_tr[i];
        else v = temp->right->rectangleBound_tr[i];    
        temp->rectangleBound_tr.push_back(v);        
    }
    return temp;
};

StaticKDTree::Points::iterator StaticKDTree::partition(Points::iterator begin, Points::iterator end, int length, int index){
    int p = rand() % length; // random pivot
    Points::iterator temp = begin;
    std::advance(temp, p);
    std::iter_swap(begin, temp); // make random pivot to first element
    Points::iterator l = begin;
    Points::iterator r = end;
    double pivot = l->at(index);
    --end;
    while(true){
        do {++l; } while(l->at(index) <= pivot && l<end);
        do {--r; } while(r->at(index) > pivot);
        if (l>=r) break;
        std::iter_swap(l,r);
    }  
    std::iter_swap(begin, r);
    return r; 
}


StaticKDTree::Points StaticKDTree::query(const Point &bl, const Point &tr){
    return queryHelper(root, bl, tr);
};

bool StaticKDTree::isOverlapped(const Point &bl1, const Point &tr1, const Point &bl2, const Point &tr2){
    bool t = true;
    for (int i=0; t && i<bl1.size(); i++){
        if (bl1[i]>tr2[i]) t = false;
        if (tr1[i]<bl2[i]) t = false;
    }
    return t;
};

StaticKDTree::Points StaticKDTree::queryHelper(Node* r, const Point &bl, const Point &tr){
    Points ans;
    if (isOverlapped(r->rectangleBound_bl, r->rectangleBound_tr, bl, tr)){
        if (r->hasData){
            ans.push_back(r->data);
        }else{
            Points temp;
            temp = queryHelper(r->left, bl, tr);
            ans.reserve(ans.size()+temp.size());
            ans.insert(ans.end(), temp.begin(), temp.end());
            temp = queryHelper(r->right, bl, tr);
            ans.reserve(ans.size()+temp.size());
            ans.insert(ans.end(), temp.begin(), temp.end());
        }
    }
    return ans;
};

#ifdef _DEBUG
void StaticKDTree::print(){
    std::queue<Node*> q;
    q.push(root);
    while(!q.empty()){
        Node* temp = q.front();
        q.pop();
        if (temp->hasData){
            std::cout << "(";
            for (double px: temp->data) std::cout << px << " ";
            std::cout << ")";
        }else{
            std::cout << "[" << temp->location << "]";
            q.push(temp->left);
            q.push(temp->right);
        }
    }
    std::cout << std::endl;
}
#endif

#endif