#ifndef DITVTREE_H
#define DITVTREE_H

#include <vector>
#include <map>
#include <iostream>

class DynamicIntervalTree{
    private:
        typedef std::pair<int, int> Interval;
        struct xCmp{
            bool operator ()(const Interval& s1, const Interval& s2){ return s1.first < s2.first; };
        };
        struct yCmp{
            bool operator ()(const Interval& s1, const Interval& s2){ return s1.second > s2.second; };
        };
        class Node{
            public:
                int val, height;
                Node *left, *right;
                std::map<Interval, int, xCmp> xsorted;
                std::map<Interval, int, yCmp> ysorted;
                Node(): val(0), left(NULL), right(NULL), height(1){};
                Node(int v): val(v), left(NULL), right(NULL), height(1){};
        };
        Node* root;
        void insertIntervalIntoNode(Interval s);
        Node* insertEndPointIntoBST(Node* r, int v);
        Node* leftRotate(Node* r);
        Node* rightRotate(Node* r);
        int getHeight(Node* r);
    public:
        DynamicIntervalTree();
        DynamicIntervalTree(std::vector<Interval> intervals);
        void insert(Interval s);
        std::vector<Interval> query(int q);
        void printStabbingSet(int q);
};

DynamicIntervalTree::DynamicIntervalTree()
: root(NULL){
};
void DynamicIntervalTree::insert(Interval s){
    // insert endpoints into AVL
    root = insertEndPointIntoBST(root, s.first);
    root = insertEndPointIntoBST(root, s.second);
    // insert interval into upperest node
    this->insertIntervalIntoNode(s); 
};
DynamicIntervalTree::DynamicIntervalTree(std::vector<Interval> sl){
    root = NULL;
    for (Interval s : sl)
        this->insert(s);
};

std::vector<DynamicIntervalTree::Interval> DynamicIntervalTree::query(int q){
    std::vector<Interval> temp;
    int i=0, w;
    Node* runner = this->root;
    while(runner){
        w = runner->val;
        if (q<w){
            for (auto itr=runner->xsorted.begin(); itr!=runner->xsorted.end() && itr->first.first <= q; itr++)
                temp.push_back(itr->first);
            runner = runner->left;
        }
        if (q>=w){
            for (auto itr=runner->ysorted.begin(); itr!=runner->ysorted.end() && itr->first.second >= q; itr++)
                temp.push_back(itr->first);
            runner = runner->right;
        }
    }
    return temp;
}

void DynamicIntervalTree::printStabbingSet(int q){
    int i=0;
    Node* runner = this->root;
    while(runner){
        if (q==runner->val){
            for (auto itr=runner->xsorted.begin(); itr!=runner->xsorted.end(); itr++)
                std::cout<< "("<< itr->first.first<<", "<< itr->first.second<<")";
            std::cout << std::endl;
            return;  
        }
        else if (q<runner->val) runner = runner->left;
        else runner = runner->right;
    }
    std::cout << "Point " << q << " is not an endpoint of any inputed seqment.";
}

void DynamicIntervalTree::insertIntervalIntoNode(Interval s){
    int x=s.first;
    int y=s.second;
    Node* runner = this->root;
    while(runner){
        int w = runner->val;
        if (x<=w && w<=y){
            runner->xsorted.insert({s, 0});
            runner->ysorted.insert({s, 0});
            break;
        }
        if (w < x) runner = runner->right;
        if (y < w) runner = runner->left;
    }
}

int DynamicIntervalTree::getHeight(Node* r){
    if (!r) return 0;
    return r->height;
}

DynamicIntervalTree::Node* DynamicIntervalTree::insertEndPointIntoBST(Node* r, int v){
    if (!r) return new Node(v);
    if (r->val == v) return r; // inserted already
    if (r->val > v){
        r->left = insertEndPointIntoBST(r->left, v);
    } else r->right = insertEndPointIntoBST(r->right, v);
   
    int lh = getHeight(r->left);
    int rh = getHeight(r->right);
    r->height = (lh>rh)? lh+1 : rh+1;
    
    // rotation to balance
    int b = lh-rh;
    if (b>1 && v < r->left->val){ // left left
        return rightRotate(r);
    }
    if (b>1 && v > r->left->val){ // left right
        r->left = leftRotate(r->left);
        return rightRotate(r);
    }
    if (b<-1 && v < r->right->val){ // right left 
        r->right = rightRotate(r->right);
        return leftRotate(r);
    }
    if (b<-1 && v > r->right->val){ // right right
        return leftRotate(r);
    }
    return r;
}

DynamicIntervalTree::Node* DynamicIntervalTree::leftRotate(Node* r){
    // rotate
    Node* temp = r->right;
    r->right = temp->left;
    temp->left = r;
    // move interval upwards
    for (auto itr=r->ysorted.begin(); itr != r->ysorted.end(); itr++){
        if (itr->first.second >= temp->val){
            r->xsorted.erase(itr->first);
            r->ysorted.erase(itr->first);
            temp->xsorted.insert({itr->first, 0});
            temp->ysorted.insert({itr->first, 0});
        } else break;
    }

    // update height
    int lh = getHeight(r->left);
    int rh = getHeight(r->right);
    r->height = (lh>rh)? lh+1 : rh+1;
    lh = getHeight(temp->left);
    rh = getHeight(temp->right);
    temp->height = (lh>rh)? lh+1 : rh+1;
    return temp;
}

DynamicIntervalTree::Node* DynamicIntervalTree::rightRotate(Node* r){
    Node* temp = r->left;
    r->left = temp->right;
    temp->right = r;

    for (auto itr=r->xsorted.begin(); itr != r->xsorted.end(); itr++){
        if (itr->first.first <= temp->val){
            r->xsorted.erase(itr->first);
            r->ysorted.erase(itr->first);
            temp->xsorted.insert({itr->first, 0});
            temp->ysorted.insert({itr->first, 0});
        } else break;
    }

    int lh = getHeight(r->left);
    int rh = getHeight(r->right);
    r->height = (lh>rh)? lh+1 : rh+1;
    lh = getHeight(temp->left);
    rh = getHeight(temp->right);
    temp->height = (lh>rh)? lh+1 : rh+1;
    return temp;
}


#endif