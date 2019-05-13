#include "BStarTree.h"
#include "RandNumGenerator.h"
#include <queue>
#include <iomanip>
#include <cassert>

extern INT::RandNumGenerator RandIntGen;

using namespace FloorPlanning;

void BStarTree::init(std::vector<Block>& block) {
    _nodes.clear();
    _nodes.resize(block.size());
    for (int i = 0; i < (int)block.size(); ++i) {
        TreeNode* n = new TreeNode(&block[i], NULL, NULL, NULL);
        if (i == 0) {
            _root = n;
        }
        else {
            int parent_id = (i-1) >> 1;
            n->SetParent(_nodes[parent_id]);
            if (i%2 == 0) { // right child
                _nodes[parent_id]->SetRightChild(n);
            }
            else { // left child
                _nodes[parent_id]->SetLeftChild(n);
            }
        }
        _nodes[i] = n;
    }
    std::cout << "[B*-Tree] Initialization completed." << std::endl;
}

void BStarTree::pack() {
    std::cout << "[B*-Tree] Start packing..." << std::endl;

    HorizontalContour contour;

    // BFS
    std::queue<TreeNode*> frontier;
    frontier.push(_root);
    while (!frontier.empty()) {
        TreeNode* curTreeNode = frontier.front();
        frontier.pop();
        if (curTreeNode == _root) {
            Block* curBlock = curTreeNode->_block;
            curBlock->SetResultX(0);
            curBlock->SetResultY(0);
            contour[curBlock->GetLowerRightX()] = 0;
            contour[curBlock->GetLowerLeftX()]  = curBlock->GetUpperLeftY();
        }
        else {
            const Block* parentBlock = curTreeNode->_parent->_block;
            Block* curBlock          = curTreeNode->_block;
            if (curTreeNode->_parent->_left == curTreeNode) { // left child
                curBlock->SetResultX(parentBlock->GetLowerRightX());
            }
            else { // right child
                curBlock->SetResultX(parentBlock->GetLowerLeftX());
            }
            // the update procedure are the same
            // compute max Y
            auto LowerBound = contour.lower_bound(curBlock->GetLowerLeftX());
            if ((*LowerBound).first != curBlock->GetLowerLeftX()) --LowerBound;
            auto UpperBound = contour.upper_bound(curBlock->GetLowerRightX());
            int maxY = 0;
            for (auto bound = LowerBound; bound != UpperBound; ++bound) {
                if ((*bound).second > maxY) maxY = (*bound).second;
            }
            curBlock->SetResultY(maxY);

            // remove illegal bouds
            if ((*LowerBound).first != curBlock->GetLowerLeftX()) ++LowerBound;
            auto updateBound = contour.lower_bound(curBlock->GetLowerRightX());
            if ((*updateBound).first != curBlock->GetLowerRightX()) --updateBound;
            int preHeight = (*updateBound).second;
            std::vector<HorizontalContour::iterator> bounds_to_be_erased;
            for (auto bound = LowerBound; bound != UpperBound; ++bound) {
                if ((*bound).first  < curBlock->GetLowerRightX()) {
                    bounds_to_be_erased.push_back(bound);
                }
            }
            for (int i = 0; i < (int)bounds_to_be_erased.size(); ++i) {
                contour.erase(bounds_to_be_erased[i]);
            }

            // update the bounds
            contour[curBlock->GetLowerRightX()] = preHeight;
            contour[curBlock->GetLowerLeftX()]  = curBlock->GetUpperLeftY();
        }
        if (curTreeNode->_left) frontier.push(curTreeNode->_left);
        if (curTreeNode->_right) frontier.push(curTreeNode->_right);
    }
    std::cout << "[B*-Tree] Packing completed." << std::endl;
}

void BStarTree::perturbation() {
    int i = RandIntGen.Generate(1, 3);
    if (i == 1) return this->op1();
    if (i == 2) return this->op2();
    if (i == 3) return this->op3();
}

void BStarTree::op1() { // rotate a macro
    // pick a node randomly and rotate
    int id = RandIntGen.Generate(0, _nodes.size()-1);
    _nodes[id]->_block->Rotate();
}

void BStarTree::op2() {
    // delete and insert
    // can delete any node but only positions pointed by NULL are legal positions
    // prevent duplicated and illegal IDs
    int deleteID = RandIntGen.Generate(0, _nodes.size()-1);
    int insertID = deleteID;
    while (insertID == deleteID || (_nodes[insertID]->_left && _nodes[insertID]->_right)) insertID = RandIntGen.Generate(0, _nodes.size()-1);
    this->delete_by_index(deleteID);
    this->insert_by_index(deleteID, insertID);

    if (!deleteID) { // update _root
        for (auto it = _nodes.begin(); it != _nodes.end(); ++it) {
            if ((*it)->_parent == NULL) {
                _root = *it;
                break;
            }
        }
    }
}

void BStarTree::op3() {
    // swap two nodes
    // prevent duplicated IDs
    int id1 = RandIntGen.Generate(0, _nodes.size()-1);
    int id2 = id1;
    while (id1 == id2) id2 = RandIntGen.Generate(0, _nodes.size()-1);
    std::swap(_nodes[id1]->_block, _nodes[id2]->_block);
}

void BStarTree::delete_by_index(const int& i) {
    this->delete_by_node(_nodes[i]);
}

void BStarTree::delete_by_node(TreeNode* target) {
    if (target->_left == NULL && target->_right == NULL) {
        if (target->_parent->_right == target) {
            target->_parent->_right = NULL;
        }
        else {
            target->_parent->_left = NULL;
        }
    }
    else if (target->_left == NULL) {
        target->_right->_parent = target->_parent;
        if (target->_parent->_right == target) {
            target->_parent->_right = target->_right;
        }
        else {
            target->_parent->_left = target->_right;
        }
    }
    else if (target->_right == NULL) {
        target->_left->_parent = target->_parent;
        if (target->_parent->_left == target) {
            target->_parent->_left = target->_left;
        }
        else {
            target->_parent->_right = target->_left;
        }
    }
    else {
        // pick one child to replace itself
        // move the other child to the original position of this child
        // delete this other child
        int id = RandIntGen.Generate(0, 1);
        TreeNode *t1, *t2, *t3, *t4, *t5;
        t1 = target->_parent;
        t2 = target->_left->_left;
        t3 = target->_left->_right;
        t4 = target->_right->_left;
        t5 = target->_right->_right;
        if (id == 0) {
            if (target->_parent) {
                if (target->_parent->_right == target) {
                    target->_parent->_right = target->_left;
                }
                else {
                    target->_parent->_left = target->_left;
                }
            }

            target->_left->_parent = t1;
            target->_left->_left   = target->_right;
            target->_left->_right  = target;

            target->_right->_left  = t2;
            target->_right->_right = t3;

            target->_parent = target->_left;
            target->_left   = t4;
            target->_right  = t5;
        }
        else {
            if (target->_parent) {
                if (target->_parent->_right == target) {
                    target->_parent->_right = target->_right;
                }
                else {
                    target->_parent->_left = target->_right;
                }
            }

            target->_right->_parent = t1;
            target->_right->_right  = target->_left;
            target->_right->_left   = target;

            target->_left->_left  = t4;
            target->_left->_right = t5;

            target->_parent = target->_right;
            target->_left   = t2;
            target->_right  = t3;
        }
        this->delete_by_node(target);
    }
}

void BStarTree::insert_by_index(const int& targetID, const int& parentID) {
    // choose a child randomly
    int id = RandIntGen.Generate(0, 1);
    _nodes[targetID]->SetParent(_nodes[parentID]);
    _nodes[targetID]->_left = NULL;
    _nodes[targetID]->_right = NULL;
    if (id == 0) { // set left child if left child is NULL
        if (_nodes[parentID]->_left) {
            assert(_nodes[parentID]->_right == NULL);
            _nodes[parentID]->SetRightChild(_nodes[targetID]);
        }
        else {
            assert(_nodes[parentID]->_left == NULL);
            _nodes[parentID]->SetLeftChild(_nodes[targetID]);
        }
    }
    else { // set right child if right child is NULL
        if (_nodes[parentID]->_right) {
            assert(_nodes[parentID]->_left == NULL);
            _nodes[parentID]->SetLeftChild(_nodes[targetID]);
        }
        else {
            assert(_nodes[parentID]->_right == NULL);
            _nodes[parentID]->SetRightChild(_nodes[targetID]);
        }
    }
}

void BStarTree::print() {
    this->debug("", _root);
}

void BStarTree::debug(std::string t, TreeNode* n) {
    cout << t << n->_block->GetName() << endl;
    if (!n->_left && !n->_right) return;
    if (n->_left) debug(t+"  ", n->_left);
    else cout << endl;
    if (n->_right) debug(t+"  ", n->_right);
    else cout << endl;
}

void BStarTree::printContour(const HorizontalContour& c) {
    cout << "---Current Contour---" << endl;
    for (auto it = c.begin(); it != c.end(); ++it) {
        cout << "x -> " << std::left << std::setw(5) << (*it).first << " y -> " << (*it).second << endl;
    }
    cout << endl;
}
