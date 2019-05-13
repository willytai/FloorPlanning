#ifndef __BSTARTREE_H__
#define __BSTARTREE_H__

#include "Block.h"
#include <vector>
#include <map>

namespace FloorPlanning
{

class TreeNode
{
    friend class BStarTree;
public:
    TreeNode() :  _left(NULL), _right(NULL), _parent(NULL) {}
    TreeNode(Block* b, TreeNode* l, TreeNode* r, TreeNode* p) { _block = b; _left = l; _right = r; _parent = p; }
    ~TreeNode() {}

    void SetBlock      (Block* b)     { _block = b; }
    void SetLeftChild  (TreeNode*& t) { _left = t; }
    void SetRightChild (TreeNode*& t) { _right = t; }
    void SetParent     (TreeNode*& t) { _parent = t; }

private:
    Block*    _block; // the corresponding block
    TreeNode* _left;  // lowest adjacent block on the right
    TreeNode* _right; // first block above with the same x-coordinate
    TreeNode* _parent;
};

class BStarTree
{
    typedef std::map<int, int> HorizontalContour; // (x, y)
public:
    BStarTree() {}
    ~BStarTree() {}

    void init(std::vector<Block>&);
    void pack();
    void perturbation();

    void op1();
    void op2();
    void op3();
    void print();

private:
    TreeNode*               _root;
    std::vector<TreeNode*>  _nodes; // this will never be changed after initialization

    void delete_by_index(const int&);
    void delete_by_node(TreeNode*);
    void insert_by_index(const int&, const int&);
    void printContour(const HorizontalContour&);
    void debug(std::string, TreeNode*);
};

}

#endif /* __BSTARTREE_H__ */
