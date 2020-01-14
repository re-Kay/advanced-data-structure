#ifndef BPTREE_H
#define BPTREE_H

#include<vector>
#include<iostream>

// now assume integer key, user defined type value
// assume order = 3
template<typename V>
class Bptree{
	public:
		const int order;
	private:
		class InternalNode; //pre-declare for Node
		class Node{
			public:
				int order;
				InternalNode* parent;
				bool isLeaf;
				std::vector<int> keys;
				int size;
				Node(int ord){ 
					this->order = ord; 
					size = 0; 
					parent=NULL; 
					keys.reserve(order + 1);
				};
		};

		class InternalNode : public Node{
			public:
				std::vector<Node*> children;
				InternalNode(int ord) : Node(ord){ 
					this->isLeaf = false; 
					children.reserve(this->order + 1);
				};
				InternalNode* split(){
					InternalNode* temp = new InternalNode(this->order);
						int half = (this->order+1)/2;
						for (int i=half; i<this->size; i++){
							temp->keys.push_back(this->keys[i]);
							temp->children.push_back(this->children[i]);
							this->children[i]->parent = temp;
						}
						temp->size = this->size - half;
						this->size = half;
						return temp;
				};
				void insertIntoNode(int key, Node* ch){
					for (int i=this->size-1; i>=0; i--){
						if (this->keys[i] > key){
							this->keys[i+1] = this->keys[i];
							this->children[i+1] = this->children[i];
						}else{
							this->keys[i+1] = key;
							this->children[i+1] = ch;
							this->size++;
							return;
						}
					}
					this->keys[0] = key;
					this->children[0] = ch;
					this->size++;
				}
				void eraseFromNode(int key){
					int i;
					for(i=0; i<this->size && this->keys[i] != key; i++);
					if (i < this->size){ //found
						for (int j=i; j<this->size; j++){
							this->keys[j] = this->keys[j+1];
							this->children[j] = this->children[j+1];
						}
						this->size --;
					}
				}
				// only the left node call this method
				// call only one of the nodes underflow, and have >= (1+order/2) *2 keys
				void redistributeWithNode(InternalNode* right){
					int numToMove = (this->size + right->size)/2 - this->size;
					if (numToMove > 0){
						for(int i=0; i<numToMove; i++){ // move from right node to left node
							this->keys[this->size + i] = right->keys[i];
							this->children[this->size + i]=right->children[i];
							right->children[i]->parent = this;
						}
						for (int i=numToMove; i < right->size; i++){
							right->keys[i-numToMove] = right->keys[i];
							right->children[i-numToMove] = right->children[i];
						}
						right->size -= numToMove;
						this->size += numToMove;
					} else {
						numToMove *= -1;
						for(int i=right->size - 1; i>=0; i--){ // move from left node to right node
							right->keys[i+numToMove] = right->keys[i];
							right->children[i+numToMove] = right->children[i];
						}
						for(int i=1; i<=numToMove; i++){
							right->keys[numToMove - i] = this->keys[this->size - i];
							right->children[numToMove - i] = this->children[this->size - i];
							this->children[this->size - i]->parent = right;
						}
						this->size -= numToMove;
						right->size += numToMove;
					}					
				}

				// only the left node call this method
				// call only one of the nodes underflow, and have < (1+order/2) *2 keys
				void mergeWithNode(InternalNode* right){
					int i;
					for(i=0; i<right->size; i++){ // move everything to left node
						this->keys[this->size + i] = right->keys[i];
						this->children[this->size + i]=right->children[i];
						right->children[i]->parent = this;
					}
					this->size += right->size;
					delete right;
				}
		};

		class LeafNode : public Node{
			public:
				std::vector<V> values;
				LeafNode *next, *prev;
				LeafNode(int ord) : Node(ord){ 
					this->isLeaf = true; 
					values.reserve(this->order + 1); 
				};
				LeafNode* split(){
					LeafNode* temp = new LeafNode(this->order);
						int half = (this->order+1)/2;
						for (int i=half; i<this->size; i++){
							temp->keys.push_back(this->keys[i]);
							temp->values.push_back(this->values[i]);
						}
						temp->next = this->next;
						if (temp->next) temp->next->prev = temp;
						temp->prev = this;
						this->next = temp;
						temp->size = this->size - half;
						this->size = half;
						return temp;
				};
				void insertIntoNode(int key, V value){
					int i;
					for (i=this->size-1; i>=0 && this->keys[i] >= key; i--){
						if (this->keys[i] == key){
							this->values[i] = value;
							return;
						}
					}
					for(int j=this->size-1; j>i; j--){
						this->keys[j+1] = this->keys[j];
						this->values[j+1] = this->values[j];
					}
					this->keys[i+1] = key;
					this->values[i+1] = value;
					this->size++;
				}
				void eraseFromNode(int key){
					int i;
					for(i=0; i<this->size && this->keys[i] != key; i++);
					if (i < this->size){ //found
						for (int j=i; j < this->size-1; j++){
							this->keys[j] = this->keys[j+1];
							this->values[j] = this->values[j+1];
						}
						this->size --;
					}
				}
				void redistributeWithNode(LeafNode* right){
					int numToMove = (this->size + right->size)/2 - this->size;
					if (numToMove > 0){
						for(int i=0; i<numToMove; i++){ // move from right node to left node
							this->keys[this->size + i] = right->keys[i];
							this->values[this->size + i]=right->values[i];
						}
						for (int i=numToMove; i < right->size; i++){
							right->keys[i-numToMove] = right->keys[i];
							right->values[i-numToMove] = right->values[i];
						}
						right->size -= numToMove;
						this->size += numToMove;
					} else {
						numToMove *= -1;
						for(int i=right->size - 1; i>=0; i--){ // move from left node to right node
							right->keys[i+numToMove] = right->keys[i];
							right->values[i+numToMove] = right->values[i];
						}
						for(int i=1; i<=numToMove; i++){
							right->keys[numToMove - i] = this->keys[this->size - i];
							right->values[numToMove - i] = this->values[this->size - i];
						}
						this->size -= numToMove;
						right->size += numToMove;
					}	
				}
				void mergeWithNode(LeafNode* right){
					int i;
					for(i=0; i<right->size; i++){ // move everything to left node
						this->keys[this->size + i] = right->keys[i];
						this->values[this->size + i]=right->values[i];
					}
					this->size += right->size;
					this->next = right->next;
					delete right;
				}
		};
		Node* root;
		int height;
		void handleOverflowAt(Node* r);
		void handleUnderflowAt(Node * r);
	public:
		Bptree();
		Bptree(Node* r, int h);
		~Bptree();
		void insert(int key, V val);
		int getHeight();
		void erase(int key);
		bool contains(int key);
		Bptree<V>* search(int key);
		Bptree<V>* search(int keylb, int keyub);
		Bptree<V>* splitWithKeySmallerThan(int key);
		Bptree<V>* splitWithKeyAtLeast(int key);
		Bptree<V>* findWithMinKey();
		Bptree<V>* findWithMaxKey();
		void join(Bptree<V>* anotherTree);
		void printValues();
		void printTree();
};

template<typename V>
void Bptree<V>::handleOverflowAt(Node* curr){
	while (curr->size > this->order){
		if (curr == root){
			InternalNode* newRoot = new InternalNode(this->order);
			if (curr->isLeaf){ 
				LeafNode* right = ((LeafNode*)curr)->split();
				right->parent = newRoot;
				newRoot->insertIntoNode(right->keys[0], right);
			} else {
				InternalNode* right = ((InternalNode*)curr)->split();
				right->parent = newRoot;
				newRoot->insertIntoNode(right->keys[0], right);
			}
			curr->parent = newRoot;
			newRoot->insertIntoNode(curr->keys[0], curr);
			this->root = newRoot;
			this->height++;
			return;
		}else{
			if (curr->isLeaf){
				LeafNode* right = ((LeafNode*)curr)->split();
				right->parent = curr->parent;
				curr->parent->insertIntoNode(right->keys[0], right);
			} else {
				InternalNode* right = ((InternalNode*)curr)->split();
				right->parent = curr->parent;
				curr->parent->insertIntoNode(right->keys[0], right);
			}
			curr = curr->parent;
		}
	}
}

template<typename V>
void Bptree<V>::handleUnderflowAt(Node* curr){
	while(curr->size < 1+this->order/2){
		if (curr == this->root){ // only when root is leaf or root has two leaves just merged
			if (!this->root->isLeaf && this->root->size == 1){ // root has one leaf only 
				this->root = ((InternalNode*)this->root)->children[0];
				this->height --; 
			}
			return;
		}
		// find sibling (exist since not root)
		Node *left = curr, *right = curr;
		int i;
		for ( i=0; i<curr->parent->size && curr->parent->children[i]!=curr; i++);
		if (i==0) { right = curr->parent->children[1]; }
		else { left = curr->parent->children[i-1]; }

		// merge or redistribute
		int oldRightKey = right->keys[0];
		if (left->size + right->size >= (1+order/2) *2){ // redistribute and finish handling
			if (left->isLeaf)
				((LeafNode*)left)->redistributeWithNode((LeafNode*)right);
			else ((InternalNode*)left)->redistributeWithNode((InternalNode*)right);
			for (i=0; i<left->parent->size && left->parent->keys[i] != oldRightKey; i++);
			left->parent->keys[i] = right->keys[0]; // update parent with new key after redistribution
			return;
		} else {	// merge and handle parent
			if (left->isLeaf)
				((LeafNode*)left)->mergeWithNode((LeafNode*)right);
			else ((InternalNode*)left)->mergeWithNode((InternalNode*)right);
			curr = left->parent;
			((InternalNode*)curr)->eraseFromNode(oldRightKey);
		}
	}
}


template<typename V>
Bptree<V>::Bptree() :
order(3), root(new LeafNode(order)), height(0){
};

template<typename V>
Bptree<V>::Bptree(Node* r, int h) :
order(r->order), root(r), height(h){
}


template<typename V>
Bptree<V>::~Bptree(){
	if (!this->root->isLeaf){
		for (int i=0;i<this->root->size;i++)
			delete ((InternalNode*)this->root)->children[i];
		delete this->root->children;
		delete this->root->keys;
	} else {
		delete ((LeafNode*)this->root)->values;
		delete this->root->keys;
	}
}

template<typename V>
int Bptree<V>::getHeight(){
	return this->height;
}

template<typename V>
void Bptree<V>::insert(int key, V val){
	Node* curr = this->root;
	// find the correct node to insert
	while(!curr->isLeaf){
		int i;
		for (i=curr->size-1; i>0 && curr->keys[i] > key; i--);
		curr = ((InternalNode*)curr)->children[i];
	}
	((LeafNode*)curr)->insertIntoNode(key, val);

	// propagate leftmost key
	if (this->root->keys[0] > key){
		Node* runner = root;
		while (!runner->isLeaf){
			runner->keys[0] = key;
			runner = ((InternalNode*)runner)->children[0];
		}
	}

	// handle overflow
	this->handleOverflowAt(curr);
}

template<typename V>
void Bptree<V>::erase(int key){
	Node* curr = this->root;
	// find the correct node to erase
	while(!curr->isLeaf){
		int i;
		for (i=curr->size-1; i>0 && curr->keys[i] > key; i--);
		curr = ((InternalNode*)curr)->children[i];
	}
	((LeafNode*)curr)->eraseFromNode(key);
	// propagate key if first element is erased
	if (curr->size > 0){
		InternalNode* runner = curr->parent;
		Node* last = curr;
		int i=0;
		while(i==0 && runner){
			for(i=0; i<runner->size && runner->children[i]!=last; i++);
			runner->keys[i] = runner->children[i]->keys[0];
			last = runner;
			runner = runner->parent;
		}
	}
	// handle underflow
	this->handleUnderflowAt(curr);
}

template<typename V>
Bptree<V>* Bptree<V>::search(int key){
	Node* curr = this->root;
	Bptree<V>* temp = new Bptree();
	// find the correct node
	while(!curr->isLeaf){
		int i;
		for (i=curr->size-1; i>0 && curr->keys[i] > key; i--);
		curr = ((InternalNode*)curr)->children[i];
	}
	for (int i=0; i<curr->size;i++) 
		if (curr->keys[i] == key) {
			temp->insert(key, ((LeafNode*)curr)->values[i]);
			break;
		}
	return temp;
}

template<typename V>
Bptree<V>* Bptree<V>::search(int keylb, int keyub){
	Bptree<V>* left = this->splitWithKeySmallerThan(keyub);
	return left->splitWithKeyAtLeast(keylb);
}

template<typename V>
Bptree<V>* Bptree<V>::splitWithKeyAtLeast(int key){
	return new Bptree();
}

template<typename V>
Bptree<V>* Bptree<V>::splitWithKeySmallerThan(int key){
	// maintain list of bptree that smaller than key
	// find the node recursively
		// target = i-th children
		// if left part of the node is large enough
		// split the left part of the node as bptree
		// else put all left children as bptree into list
		// go down to i-th children
	// join all bptrees
	return new Bptree();
}


template<typename V>
Bptree<V>* Bptree<V>::findWithMinKey(){
	Bptree<V>* temp = new Bptree();
	if (this->root->size == 0) return temp;
	Node* runner = root;
	while (!runner->isLeaf) runner = ((InternalNode*)runner)->children[0];
	temp->insert(runner->keys[0], ((LeafNode*)runner)->values[0]);
	return temp;
}

template<typename V>
Bptree<V>* Bptree<V>::findWithMaxKey(){
	Bptree<V>* temp = new Bptree();
	if (this->root->size == 0) return temp;
	Node* runner = root;
	while (!runner->isLeaf) runner = ((InternalNode*)runner)->children[runner->size - 1];
	temp->insert(runner->keys[runner->size - 1], ((LeafNode*)runner)->values[runner->size - 1]);
	return temp;
}

template<typename V>
void Bptree<V>::join(Bptree<V>* that){
	if (that->root->isLeaf && that->root->size == 0) return; // nothing in tree 2
	if (this->root->isLeaf && this->root->size == 0){ // nothing in tree 1
		this->root = that->root;
		this->height = that->height;
		return;
	}
	if (this->height == that->height){
		if (this->root->isLeaf){ // both leaf and either one has 1 child only
			if (this->root->size <= 1 || that->root->size <=1) {
				for(int i=0; i<that->root->size; i++){
					this->root->keys[this->root->size+i] = that->root->keys[i];
					((LeafNode*)this->root)->values[this->root->size+i] = ((LeafNode*)that->root)->values[i];
				}
				this->root->size += that->root->size;
				this->handleOverflowAt(this->root);
				return;
			}
		}
		InternalNode* temp = new InternalNode(this->order);
		temp->keys[0] = this->root->keys[0];
		temp->children[0] = this->root;
		temp->keys[1] = that->root->keys[0];
		temp->children[1] = that->root;
		temp->size = 2;
		this->root->parent = temp;
		that->root->parent = temp;
		this->root = temp;
		this->height++;
	} else if (this->height < that->height){
		// go to left subtree of tree 2
		InternalNode* curr = (InternalNode*)that->root;
		for (int i=0; i<that->height-this->height -1; i++)
			curr = (InternalNode*)curr->children[0];
		
		// if this tree has only leaf of size 1
		if (this->root->isLeaf && this->root->size==1){
			LeafNode* right = (LeafNode*)curr->children[0];
			for (int i=right->size - 1; i>=0; i--){
				right->keys[i+1] = right->keys[i];
				right->values[i+1] = right->values[i];
			}
			right->keys[0] = this->root->keys[0];
			right->values[0] = ((LeafNode*)this->root)->values[0];
			right->size++;
			// propagate leftmost key
			InternalNode* runner = right->parent;
			while (runner){
				runner->keys[0] = this->root->keys[0];
				runner = runner->parent;
			}
			that->handleOverflowAt(right);
			// transfer info
			this->root = that->root;
			this->height = that->height;
			return;
		}
		// insert
		for (int i=curr->size-1; i>=0; i--){
			curr->keys[i+1] = curr->keys[i];
			curr->children[i+1] = curr->children[i];
		}
		curr->keys[0] = this->root->keys[0];
		curr->children[0] = this->root;
		curr->size++;
		// propagate leftmost key
		InternalNode* runner = curr->parent;
		while (runner){
			runner->keys[0] = this->root->keys[0];
			runner = runner->parent;
		}
		// handle overflow
		that->handleOverflowAt(curr);

		// transfer info
		this->root = that->root;
		this->height = that->height;
	} else {
		// go to right subtree
		InternalNode* curr = (InternalNode*)this->root;
		for (int i=0; i<this->height-that->height - 1; i++)
			curr = (InternalNode*)curr->children[curr->size - 1];
		
		// if that tree has only leaf of size 1
		if (that->root->isLeaf && that->root->size==1){
			LeafNode* left = (LeafNode*)curr->children[curr->size - 1];
			left->keys[curr->size] = that->root->keys[0];
			left->values[curr->size] = ((LeafNode*)that->root)->values[0];
			left->size++;
			that->handleOverflowAt(left);
			return;
		}
		// insert
		curr->keys[curr->size] = that->root->keys[0];
		curr->children[curr->size] = that->root;
		curr->size++;
		// handle overflow
		this->handleOverflowAt(curr);
	}

}

template<typename V>
bool Bptree<V>::contains(int key){
	Node* curr = this->root;
	// find the correct node
	while(!curr->isLeaf){
		int i;
		for (i=curr->size-1; i>0 && curr->keys[i] >= key; i--);
		curr = ((InternalNode*)curr)->children[i];
	}
	for (int i=0; i<curr->size;i++) if (curr->keys[i] == key) return true;
	return false;
}


template<typename V>
void Bptree<V>::printValues(){
	Node* curr = this->root;
	while(!curr->isLeaf) curr = ((InternalNode*)curr)->children[0];
	while(curr){
		std::cout << "[";
		for (int i=0; i<curr->size; i++) 
			std::cout << "(" << curr->keys[i]<<" ,"<< ((LeafNode*)curr)->values[i]<<")" ;
		std::cout << "]";
		curr = ((LeafNode*)curr)->next;
	}
	std::cout << std::endl;
}

template<typename V>
void Bptree<V>::printTree(){ // inorder
	std::cout << "[";
	for (int i=0; i<this->root->size; i++) 
		std::cout << " " << this->root->keys[i];
	std::cout << "]";
	std::cout << std::endl;
	if (!this->root->isLeaf)
		for (int i=0; i<this->root->size; i++){
			Bptree* st = new Bptree(((InternalNode*)this->root)->children[i], this->height - 1);
			st->printTree();
		}
	
}


#endif