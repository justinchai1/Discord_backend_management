/**
 * Project 2 - Binary Trees
 * UserTree.h
 * Implementation for the UTree class.
 */

#include "utree.h"

/**
 * Destructor, deletes all dynamic memory.
 */
UTree::~UTree() {
clear();
}

/**
 * Sources a .csv file to populate Account objects and insert them into the UTree.
 * @param infile path to .csv file containing database of accounts
 * @param append true to append to an existing tree structure or false to clear before importing
 */ 
void UTree::loadData(string infile, bool append) {
    std::ifstream instream(infile);
    string line;
    char delim = ',';
    const int numFields = 5;
    string fields[numFields];

    /* Check to make sure the file was opened */
    if(!instream.is_open()) {
        std::cerr << __FUNCTION__ << ": File " << infile << " could not be opened or located" << endl;
        exit(-1);
    }

    /* Should we append or clear? */
    if(!append) this->clear();

    /* Read in the data from the .csv file and insert into the UTree */
    while(std::getline(instream, line)) {
        std::stringstream buffer(line);

        /* Quick check to make sure each line is formatted correctly */
        int delimCount = 0;
        for(unsigned int c = 0; c < buffer.str().length(); c++) if(buffer.str()[c] == delim) delimCount++;
        if(delimCount != numFields - 1) {
            throw std::invalid_argument("Malformed input file detected - ensure each line contains 5 fields deliminated by a ','");
        }

        /* Populate the account attributes - 
         * Each line always has 5 sections of data */
        for(int i = 0; i < numFields; i++) {
            std::getline(buffer, line, delim);
            fields[i] = line;
        }
        Account newAcct = Account(fields[0], std::stoi(fields[1]), std::stoi(fields[2]), fields[3], fields[4]);
        this->insert(newAcct);
    }
}

/**
 * Dynamically allocates a new UNode in the tree and passes insertion into DTree. 
 * Should also update heights and detect imbalances in the traversal path after
 * an insertion.
 * @param newAcct Account object to be inserted into the corresponding DTree
 * @return true if the account was inserted, false otherwise
 */
UNode* UTree::insertNode(UNode* node, string username){
     if(node==nullptr){
        return nullptr;// ifthe tree is empty

    }
    else if(node->getUsername()==username){
        return node;// if they find out the disc are both the same
    }
   
    if(node->_left!=nullptr && username<node->getUsername()  ){
        return insertNode(node->_left, username);
    }
    else if(node->_right!=nullptr && username>node->getUsername()  ){
        return insertNode(node->_right, username);
    }
    else{
        return node;
    }
  
}
void UTree::update(UNode* node){
     if(node==nullptr){
        return;// ifthe tree is empty
        
    }
 
update(node->_left);
updateHeight(node); 
update(node->_right);
updateHeight(node);
if(checkImbalance(node)== true){
           //cout << "ifrner" << endl;  
            rebalance(node);
         } 
}
bool UTree::insert(Account newAcct) {
      if(_root==nullptr){
        _root = new UNode();
        _root->getDTree()->insert(newAcct);
        //calll
        
        
        return true;
    }
    UNode* bode = retrieve(newAcct.getUsername());
    bool insertStat =false;
if( bode== nullptr){
UNode* prev = insertNode(_root, newAcct.getUsername());
if(prev->getUsername() == newAcct.getUsername()){
        return false;
    }
   
    else{
        if(newAcct.getUsername()> prev->getUsername()){
            prev->_right = new UNode();
            insertStat = prev->_right->getDTree()->insert(newAcct);
        }
        else 
        {
            prev->_left = new UNode();
            insertStat = prev->_left->getDTree()->insert(newAcct);
            
        }
}
}
else{
 insertStat = bode->getDTree()->insert(newAcct);
//update the height and chekc for imbalance

}
update(_root);
return insertStat;
}

/**
 * Removes a user with a matching username and discriminator.
 * @param username username to match
 * @param disc discriminator to match
 * @param removed DNode object to hold removed account
 * @return true if an account was removed, false otherwise
 */
void UTree::removeUNode(UNode* node){
UNode* x = nullptr;
UNode* trav = node->_left;
if(node->_right==nullptr && node->_left==nullptr ){// if the node has no child
    node->getDTree()->clear();
    delete node;
}
else if(node->_right!=nullptr && node->_left==nullptr){// if the node has only a right child
node = node->_right;
node->_right->getDTree()->clear();
delete node->_right;
}
else if(node->_left!=nullptr && node->_left->_height>1){// if theres a whole ass subtree in the left child
    while(trav->_right!=nullptr){
        trav = trav->_right;
    }
    x=trav;
    x->_right= node->_right;
    x->_left = node->_left;
    node = x; 
    if(trav->_left!=nullptr){//replace x with its left child and deletes the left child
        x= trav->_left;
        trav->_left->getDTree()->clear();
        delete trav->_left;
    }
    else{ // deletes x when there is ano left chidl
        x->getDTree()->clear();
        delete x;
    }
}
update(_root);
}
bool UTree::removeUser(string username, int disc, DNode*& removed) {
UNode* user = retrieve(username);
if(user ==nullptr){
    return false;
}
else{
    user->getDTree()->remove(disc,removed);
    if(user->getDTree()->getNumUsers()==0){
        removeUNode( user);
    }
    return true;
}
}

/**
 * Retrieves a set of users within a UNode.
 * @param username username to match
 * @return UNode with a matching username, nullptr otherwise
 */
UNode* UTree::retrieve(string username) {
return retrieveUserName(_root,username);
}

/**
 * Retrieves the specified Account within a DNode.
 * @param username username to match
 * @param disc discriminator to match
 * @return DNode with a matching username and discriminator, nullptr otherwise
 */
DNode* UTree::retrieveUser(string username, int disc) {
return retrieve(username)->getDTree()->retrieve(disc);
}

/**
 * Returns the number of users with a specific username.
 * @param username username to match
 * @return number of users with the specified username
 */
UNode* UTree::retrieveUserName(UNode* node,string username) {
    if(node==nullptr){
return nullptr;
}
if(node->getUsername()==username){
    return node;
}
if(username>node->getUsername()){
    retrieveUserName(node->_right, username);
}
else if(username<node->getUsername()){
    retrieveUserName(node->_left, username);
}
return nullptr;
}
int UTree::numUsers(string username) {
UNode* user = retrieveUserName(_root,username);
return user->getDTree()->getNumUsers();
}

/**
 * Helper for the destructor to clear dynamic memory.
 */
void UTree::clearHelper(UNode* node){
     if (node == nullptr) return;

    clearHelper(node->_left);
    clearHelper(node->_right);
   
    node->getDTree()->clear();
    delete node;
}
void UTree::clear() {
    clearHelper(_root);
_root = nullptr;
}

/**
 * Prints all accounts' details within every DTree.
 */
void UTree::accountHelper(UNode* node)const{
if(node != nullptr){
    accountHelper(node->_left);
    node->getDTree()->printAccounts();
    accountHelper(node->_right);
}
}
void UTree::printUsers() const {
accountHelper(_root);
}

/**
 * Dumps the UTree in the '()' notation.
 */
void UTree::dump(UNode* node) const {
    if(node == nullptr) return;
    cout << "(";
    dump(node->_left);
    cout << node->getUsername() << ":" << node->getHeight() << ":" << node->getDTree()->getNumUsers();
    dump(node->_right);
    cout << ")";
}

/**
 * Updates the height of the specified node.
 * @param node UNode object in which the height will be updated
 */
void UTree::updateHeight(UNode* node) {
    if(node->_right== nullptr and node->_left ==nullptr){// if it has no children
        node->_height=1;
    }
    else if( node->_right!= nullptr and node->_left ==nullptr){//if it only has a right chidl
        node->_height = 1+ node->_right->_height;
    }
    else if( node->_right== nullptr and node->_left !=nullptr){// if it only has a left child
        node->_height = 1+ node->_left->_height;
    }
    else{// if it has both chidlren then it checks which one has the greater height
        if(node->_left->_height > node->_right->_height){
    node->_height = 1+ node->_left->_height;
}
else{
    node->_height = 1+ node->_right->_height;
}
    }

}

/**
 * Checks for an imbalance, defined by AVL rules, at the specified node.
 * @param node UNode object to inspect for an imbalance
 * @return (can change) returns true if an imbalance occured, false otherwise
 */
int UTree::checkImbalance(UNode* node) {
if(node->_left->_height - node->_right->_height >1 || node->_right->_height - node->_left->_height >1){
return true;
}
else {
return false;
}
}

//----------------
/**
 * Begins and manages the rebalance procedure for an AVL tree (pass by reference).
 * @param node UNode object where an imbalance occurred
 */
UNode* UTree::rightRot(UNode*& node)
{  
    UNode* u = node->_right;
    node->_right = u->_right;
    u->_right =node;
    updateHeight(node);
    updateHeight(u);
    return node;
}
UNode* UTree::leftRot(UNode*& node)
{
    UNode* u = node->_right;
    node->_right = u->_left;
    u->_left = node;
    updateHeight(node);
    updateHeight(u);
    return node;
}
UNode* UTree::doubleLeftRot(UNode*& node){
node->_right = rightRot(node->_right);
return leftRot(node);
}
UNode* UTree::doubleRightRot(UNode*& node){
    node->_left = leftRot(node->_left);
    return rightRot(node);
}
int UTree::balanceFactor(UNode*& node){
    int left;
    int right;
    if (node->_left == nullptr) { 
        left = 0;
        right = node->_right->_height;
    }
    else if (node->_right == nullptr) { 
        right = 0;
        left = node->_left->_height;
    }
    else{
        left = node->_left->_height;
    right = node->_right->_height;
    }
    
    return left - right;
}
void UTree::rebalance(UNode*& node) {
    int bf= balanceFactor(node);

if(bf>0 and node->_left->_left!=nullptr){// left side is heavy and the left left node is there
rightRot(node);
}
else if(bf>0 and node->_left->_left==nullptr){// if the left left node is not there
  doubleRightRot(node);  
}
else if(bf<0 and node->_right->_right!=nullptr){// right side is heavy and right right node is there
leftRot(node);
}
else if(bf<0 and node->_right->_right==nullptr){// if the right right node is not there
doubleLeftRot(node);
}
// -- OR --
}

/**
 * Begins and manages the rebalance procedure for an AVL tree (returns a pointer).
 * @param node UNode object where an imbalance occurred
 * @return UNode object replacing the unbalanced node's position in the tree
 */
//UNode* UTree::rebalance(UNode* node) {

//}
//----------------