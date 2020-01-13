#include<iostream>
#include<vector>
#include "bptree.hpp"

using namespace std;

int main(){
    Bptree<int>* bp = new Bptree<int>();
    Bptree<int>* bp2 = new Bptree<int>();
    bp->insert(0,1);
    bp->insert(1,1);
    bp->insert(2,1);
    bp->insert(3,1);
    bp->insert(4,1);
    bp2->insert(90,1);
    bp2->insert(190,1);
    bp2->insert(390,1);
    bp2->insert(290,1);
    bp2->insert(80,1);
    bp2->insert(60,1);
    bp2->insert(160,1);
    bp2->insert(150,1);
    bp2->insert(200,1);
    bp2->insert(920,1);
    bp2->insert(180,1);
    bp2->insert(450,1);
    bp2->insert(420,1);
    bp2->insert(350,1);
    bp2->insert(10,1);
    bp2->insert(20,1);
    bp2->insert(30,1);
    bp2->insert(40,1);
    bp->join(bp2);
    bp->printTree();

    return 0;
}