#ifndef STREAP_H
#define STREAP_H

#include<vector>
#include<queue>
#include<algorithm>

class staticTreap{
    private:
        typedef std::pair<double, double> Point;
        typedef std::vector<Point> Points;
        Points &operator-=(Points &a){

        }
        void appendVector(Points &a, const Points &b){
            a.reserve(a.size()+b.size());
            a.insert(a.end(), b.begin(), b.begin());
        }

        class Node{
            public:
                double key;
                staticTreap::Point minPilot, maxPilot;
                bool hasMinPilot, hasMaxPilot, isConceptualLeaf;
                Node *left, *right;
                Node(): hasMaxPilot(false), hasMinPilot(false),isConceptualLeaf(false){};
                Node(double k):key(k), hasMaxPilot(false), hasMinPilot(false), isConceptualLeaf(false){};
        };
        Points reportSubtree(Node* r, double x, bool isRight);
        Node* BSTBuilder(const std::vector<double> &ys, int lb, int ub);
        Node *root;
    public:
        staticTreap();
        staticTreap(Points points);
        Points queryOpenRightward(double xl, double yl, double yu);
        Points queryOpenLeftward(double xu, double yl, double yu);  
};

staticTreap::staticTreap()
: root(NULL){
};

staticTreap::staticTreap(Points points){
    // BST on y value
    std::vector<double> ylist;
    for (Point p: points){
        ylist.push_back(p.second);
    }
    std:sort(ylist.begin(),ylist.end());
    auto it = std::unique(ylist.begin(), ylist.end());
    ylist.erase(it, ylist.end());
    root = BSTBuilder(ylist, 0, ylist.size()-1);

    // assign value to min/max heap
    
};

staticTreap::Node* staticTreap::BSTBuilder(const std::vector<double> &ys, int lb, int ub){
    if (ub>lb){
        Node* temp = new Node();
        temp->isConceptualLeaf = true;
        return temp;
    }
    int mid = lb + (ub-lb+1)/2;
    Node* temp = new Node(ys[mid]);
    temp->left = BSTBuilder(ys, lb, mid-1);
    temp->right = BSTBuilder(ys, mid+1, ub);
    return temp;
}

staticTreap::Points staticTreap::queryOpenRightward(double xl, double yl, double yu){
    Points ans, temp;
    Node *p1 = root, *p2 = root;
    Point pilot;
    // move along BST with y until yl < key <= yu
    while (p1 == p2){
        if (p1->hasMaxPilot){
            pilot = p1->maxPilot;
            if (pilot.first >= xl && yl <= pilot.second && pilot.second <= yu)
                ans.push_back(pilot);
        }
        p1 = (yl < p1->key)? p1->left: p1->right;
        p2 = (yu < p2->key)? p2->left: p2->right;
    }

    // handle the path for yl first
    while (p1){
        if (p1->hasMaxPilot){
            pilot = p1->maxPilot;
            if (pilot.first >= xl && yl <= pilot.second && pilot.second <= yu)
                ans.push_back(pilot);
        }
        if (yl <= p1->key){
            temp = reportSubtree(p1->right, xl, true);
            appendVector(ans, temp);
        }
        if (yl == p1->key) break;
        p1 = (yl < p1->key)? p1->left: p1->right;
    }

    while (p2){
        if (p2->hasMaxPilot){
            pilot = p2->maxPilot;
            if (pilot.first >= xl && yl <= pilot.second && pilot.second <= yu)
                ans.push_back(pilot);
        }
        if (yu >= p2->key){
            temp = reportSubtree(p2->left, xl, true);
            appendVector(ans, temp);
        }
        if (yu == p2->key) break;
        p2 = (yu < p2->key)? p2->left: p2->right;
    }
    return ans;
};

staticTreap::Points staticTreap::reportSubtree(Node* r, double x, bool isRight){
    if (!r) return ;
    Points ans;
    Node* temp;
    std::queue<Node*> q;
    q.push(r);
    if (!q.empty()){
        temp = q.front();
        q.pop();
        if (temp->hasMaxPilot && temp->maxPilot.first > x){
            ans.push_back(temp->maxPilot);
            if (!temp->isConceptualLeaf){
                q.push(r->left);
                q.push(r->right);
            }
        }        
    }
    return ans;
}


#endif