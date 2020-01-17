#ifndef DITVTREE_H
#define DITVTREE_H

#include <vector>
#include <algorithm>
#include <iostream>

class DynamicIntervalTree{
    private:
        typedef std::pair<int, int> Interval;
        class Node{
            public:
                int val, height;
                Node *left, *right;
                std::vector<Interval> xsorted, ysorted;
                Node(): val(0), left(NULL), right(NULL), height(1){};
                Node(int v): val(v), left(NULL), right(NULL), height(1){};
        };
        Node* root;
        Node* treeBuilder(std::vector<int> sortedarray, int l, int u);
        void insertIntervalIntoNode(Interval s);
        void sortList(Node *r);
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

DynamicIntervalTree::DynamicIntervalTree(std::vector<Interval> intervals){
    std::vector<int> xylist;
    for (auto s:intervals){
        xylist.push_back(s.first);
        xylist.push_back(s.second);
    }
    std::sort(xylist.begin(), xylist.end());
    root = treeBuilder(xylist, 0, xylist.size()-1);

    for (auto s: intervals)
        this->insertIntervalIntoNode(s);
    
    sortList(this->root);
};

DynamicIntervalTree::Node* DynamicIntervalTree::treeBuilder(std::vector<int> sortedarray, int l, int u){
    if (u<l) return NULL;
    int mid = l + (u-l+1)/2;
    Node* temp = new Node(sortedarray[mid]);
    temp->left = treeBuilder(sortedarray, l, mid-1);
    temp->right = treeBuilder(sortedarray, mid+1, u);
    int height = 0;
    if (temp->left) height = temp->left->height;
    if (temp->right && temp->right->height > height) height = temp->right->height; 
    temp->height += height;
    return temp;
};

void DynamicIntervalTree::sortList(Node *r){
    if (!r) return;
    std::sort(r->xsorted.begin(), 
              r->xsorted.end(), 
              [](Interval s1, Interval s2){return s1.first < s2.first;});
    std::sort(r->ysorted.begin(), 
             r->ysorted.end(), 
             [](Interval s1, Interval s2){return s1.second > s2.second;});
    sortList(r->left);
    sortList(r->right);
}

void DynamicIntervalTree::insertIntervalIntoNode(Interval s){
    bool notAssigned = true;
    int x=s.first;
    int y=s.second;
    Node* runner = this->root;
    while(notAssigned){
        int w = runner->val;
        if (x<=w && w<=y){
            runner->xsorted.push_back(s);
            runner->ysorted.push_back(s);
            notAssigned = false;
        }
        if (w < x) runner = runner->left;
        if (y < w) runner = runner->right;
    }
}

void DynamicIntervalTree::insert(Interval s){
    // update BST
    root = insertEndPointIntoBST(this->root, s.first);
    root = insertEndPointIntoBST(this->root, s.second);
    // insert the interval into BST node
    this->insertIntervalIntoNode(s);
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
    int lh = getHeight(r->left);
    int rh = getHeight(r->right);
    r->height = (lh>rh)? lh+1 : rh+1;
    lh = getHeight(temp->left);
    rh = getHeight(temp->right);
    temp->height = (lh>rh)? lh+1 : rh+1;
    return temp;
}

std::vector<DynamicIntervalTree::Interval> DynamicIntervalTree::query(int q){
    std::vector<Interval> temp, s;
    int i=0, w;
    Node* runner = this->root;
    while(runner){
        w = runner->val;
        if (q<w){
            s = runner->xsorted;
            for (int j=0; j<s.size() && s[j].first <= q; j++) temp.push_back(s[j]);
            runner = runner->left;
        }
        if (q>=w){
            s = runner->ysorted;
            for (int j=0; j<s.size() && s[j].second >= q; j++) temp.push_back(s[j]);
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
            for (int j=0; j<runner->xsorted.size(); j++)
                std::cout<< "("<<runner->xsorted[j].first<<", "<<runner->xsorted[j].second<<")";
            std::cout << std::endl;
            break;    
        }
        else if (q<runner->val) runner = runner->left;
        else runner = runner->right;
    }
    std::cout << "Point " << q << " is not an endpoint of any inputed seqment.";
}
#endif