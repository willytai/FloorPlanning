#ifndef __BSTARTREE_H__
#define __BSTARTREE_H__

#include "Block.h"
#include <vector>
#include <map>

namespace FloorPlanning
{

class BStarTree
{
    typedef std::map<int, int> HorizontalContour; // (x, y)
public:
    BStarTree() { _op1_count = _op2_count = _op3_count = 0; }
    ~BStarTree() {}

    void init(std::vector<Block>&);
    void pack();
    void packNeighbor();
    void perturbation();
    void getblocks(std::vector<Block>&);
    void getneighborblocks(std::vector<Block>&);
    void update();
    void report();

    size_t size() const { return _nodes.size(); }

    void op1();
    void op2();
    void op3();

    void print();
    void printNeighbor();
    void printResult();
    void printNeighborResult();

private:
    std::vector<Block>  _nodes;
    int                 _nodes_root_id;
    std::vector<Block>  _neighbor;
    int                 _neighbor_root_id;

    void delete_insert(int, int);
    void printContour(const HorizontalContour&);
    void debug(std::string, int);
    void debugNeighbor(std::string, int);

    // debug
    int _op1_count;
    int _op2_count;
    int _op3_count;
};

}

#endif /* __BSTARTREE_H__ */
