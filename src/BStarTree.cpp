#include "BStarTree.h"
#include "RandNumGenerator.h"
#include <queue>
#include <iomanip>
#include <cassert>
#include <algorithm>

extern INT::RandNumGenerator RandIntGen;

using namespace FloorPlanning;

void BStarTree::init(std::vector<Block>& block) {
    _nodes.clear();
    _neighbor.clear();
    _nodes.resize(block.size());
    _neighbor.resize(block.size());
    for (int i = 0; i < (int)block.size(); ++i) {
        _nodes[i] = block[i];
        if (i == 0) {
            _nodes[0].SetParent(-1);
            _nodes_root_id = 0;
        }
        else {
            if (i % 2) {
                _nodes[i-1].SetLeft(i);
                _nodes[i].SetParent(i-1);
            }
            else {
                _nodes[i-1].SetRight(i);
                _nodes[i].SetParent(i-1);
            }
        }
    }
    std::cout << "[B*-Tree] Initialization completed." << std::endl;

    // this->pack();
    // this->print();
    // this->printResult();
    // cout << endl;

    // for (int i = 0; i < 100; ++i) {
        // cout << "original" << endl;
        // this->print();
        // this->perturbation();
        // this->packNeighbor();
        // this->printNeighbor();
        // this->printNeighborResult();
    // }

    // exit(0);
}

void BStarTree::update() {
    _nodes.swap(_neighbor);
}

void BStarTree::packNeighbor() {
    // std::cout << "[B*-Tree] Start packing _neighbor ..." << std::endl;

    HorizontalContour contour;

    // BFS
    std::queue<Block*> frontier;
    frontier.push(&_neighbor[_neighbor_root_id]);
    while (!frontier.empty()) {
        Block* curBlock = frontier.front();
        frontier.pop();
        if (curBlock->GetParent() == -1) { // root
            curBlock->SetResultX(0);
            curBlock->SetResultY(0);
            contour[curBlock->GetLowerRightX()] = 0;
            contour[curBlock->GetLowerLeftX()]  = curBlock->GetUpperLeftY();
        }
        else {
            const Block& parentBlock = _neighbor[curBlock->GetParent()];
            if (parentBlock.GetLeft() == curBlock->GetID()) { // left child
                curBlock->SetResultX(parentBlock.GetLowerRightX());
            }
            else { // right child
                curBlock->SetResultX(parentBlock.GetLowerLeftX());
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
        if (curBlock->GetLeft() != -1) frontier.push(&_neighbor[curBlock->GetLeft()]);
        if (curBlock->GetRight() != -1) frontier.push(&_neighbor[curBlock->GetRight()]);
    }
    // std::cout << "[B*-Tree] Packing completed." << std::endl;
}

void BStarTree::pack() {
    // std::cout << "[B*-Tree] Start packing _nodes ..." << std::endl;

    HorizontalContour contour;

    // BFS
    std::queue<Block*> frontier;
    frontier.push(&_nodes[_nodes_root_id]);
    while (!frontier.empty()) {
        Block* curBlock = frontier.front();
        frontier.pop();
        if (curBlock->GetParent() == -1) { // root
            curBlock->SetResultX(0);
            curBlock->SetResultY(0);
            contour[curBlock->GetLowerRightX()] = 0;
            contour[curBlock->GetLowerLeftX()]  = curBlock->GetUpperLeftY();
        }
        else {
            const Block& parentBlock = _nodes[curBlock->GetParent()];
            if (parentBlock.GetLeft() == curBlock->GetID()) { // left child
                curBlock->SetResultX(parentBlock.GetLowerRightX());
            }
            else { // right child
                curBlock->SetResultX(parentBlock.GetLowerLeftX());
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
        if (curBlock->GetLeft() != -1) frontier.push(&_nodes[curBlock->GetLeft()]);
        if (curBlock->GetRight() != -1) frontier.push(&_nodes[curBlock->GetRight()]);
    }
    // std::cout << "[B*-Tree] Packing completed." << std::endl;
}

void BStarTree::perturbation() {
    // create a new tree and preform perturbation on it
    // prevent memory leak
    if (_neighbor.size() != _nodes.size()) _neighbor.resize(_nodes.size());
    for (int i = 0; i < (int)_nodes.size(); ++i) {
        _neighbor[i] = _nodes[i];
    }
    _neighbor_root_id = _nodes_root_id;

    int i = RandIntGen.Generate(1, 1000000);
    // cout << "[B*-Tree] Perturbation op" << i << endl;
    if (i < 1000000/3) {
        this->op1();
        _op1_count += 1;
    }
    else if (i < 1000000/3*2) {
        this->op2();
        _op2_count += 1;
    }
    else {
        this->op3();
        _op3_count += 1;
    }
}

void BStarTree::op1() { // rotate a macro
    // pick a node randomly and rotate
    int id = RandIntGen.Generate(0, _neighbor.size()-1);
    _neighbor[id].Rotate();

    // cout << "rotating " << _neighbor[id].GetName() << endl;
}

void BStarTree::op2() {
    // delete and insert
    // only delete nodes without child
    // only positions pointed by NULL are legal positions to insert
    // prevent duplicated and illegal IDs
    std::vector<int> candidate; candidate.resize(_neighbor.size());
    for (int i = 0; i < (int)candidate.size(); ++i) {
        candidate[i] = i;
    }
    std::shuffle(candidate.begin(), candidate.end(), std::default_random_engine(RandIntGen.Generate()));
    int deleteID = candidate.back();
    while (_neighbor[deleteID].GetLeft() != -1 || _neighbor[deleteID].GetRight() != -1) {
        candidate.pop_back();
        deleteID = candidate.back();
    }

    candidate.resize(_neighbor.size());
    for (int i = 0; i < (int)candidate.size(); ++i) {
        candidate[i] = i;
    }
    std::shuffle(candidate.begin(), candidate.end(), std::default_random_engine(RandIntGen.Generate()));
    int insertID = candidate.back();
    while (insertID == deleteID || (_neighbor[insertID].GetLeft() != -1 && _neighbor[insertID].GetRight() != -1)) {
        candidate.pop_back();
        insertID = candidate.back();
    }

    this->delete_insert(deleteID, insertID);
}

void BStarTree::op3() {
    // swap two nodes
    // prevent duplicated IDs
    int id1 = RandIntGen.Generate(0, _neighbor.size()-1);
    int id2 = id1;
    while (id1 == id2) id2 = RandIntGen.Generate(0, _neighbor.size()-1);
    // cout << "swapping " << _neighbor[id1].GetName() << ' ' << _neighbor[id2].GetName() << endl;

    std::string name1   = _neighbor[id1].GetName();
    int         width1  = _neighbor[id1].GetWidth();
    int         height1 = _neighbor[id1].GetHeight();

    std::string name2   = _neighbor[id2].GetName();
    int         width2  = _neighbor[id2].GetWidth();
    int         height2 = _neighbor[id2].GetHeight();

    _neighbor[id1].SetName(name2);
    _neighbor[id1].SetWidth(width2);
    _neighbor[id1].SetHeight(height2);

    _neighbor[id2].SetName(name1);
    _neighbor[id2].SetWidth(width1);
    _neighbor[id2].SetHeight(height1);
}

void BStarTree::delete_insert(int deleteID, int insertID) {
    Block& deleteBlock = _neighbor[deleteID];
    Block& parentBlock = _neighbor[deleteBlock.GetParent()];
    if (parentBlock.GetLeft() == deleteID) {
        parentBlock.SetLeft(-1);
    }
    else if (parentBlock.GetRight() == deleteID) {
        parentBlock.SetRight(-1);
    }
    else assert(0);

    Block& insertBlock = _neighbor[insertID];
    if (insertBlock.GetLeft() != -1) {
        assert(insertBlock.GetRight() == -1);
        insertBlock.SetRight(deleteID);
        deleteBlock.SetParent(insertID);
        // cout << "inserting " << deleteBlock.GetName() << " to the right child of " << insertBlock.GetName() << endl;
    }
    else if (insertBlock.GetRight() != -1) {
        assert(insertBlock.GetLeft() == -1);
        insertBlock.SetLeft(deleteID);
        deleteBlock.SetParent(insertID);
        // cout << "inserting " << deleteBlock.GetName() << " to the left child of " << insertBlock.GetName() << endl;
    }
    else {
        int dice = rand() % 2;
        if (dice) {
            insertBlock.SetLeft(deleteID);
            deleteBlock.SetParent(insertID);
            // cout << "inserting " << deleteBlock.GetName() << " to the left child of " << insertBlock.GetName() << endl;
        }
        else {
            insertBlock.SetRight(deleteID);
            deleteBlock.SetParent(insertID);
            // cout << "inserting " << deleteBlock.GetName() << " to the right child of " << insertBlock.GetName() << endl;
        }
    }
}

void BStarTree::print() {
    this->debug("", _nodes_root_id);
}

void BStarTree::printNeighbor() {
    this->debugNeighbor("", _neighbor_root_id);
}

void BStarTree::debug(std::string t, int id) {
    if (id == -1) {
        cout << t << "NULL" << endl;
        return;
    }
    cout << t << _nodes[id].GetName() << endl;
    debug(t+"  ", _nodes[id].GetLeft());
    debug(t+"  ", _nodes[id].GetRight());
}

void BStarTree::debugNeighbor(std::string t, int id) {
    if (id == -1) {
        cout << t << "NULL" << endl;
        return;
    }
    cout << t << _neighbor[id].GetName() << endl;
    debugNeighbor(t+"  ", _neighbor[id].GetLeft());
    debugNeighbor(t+"  ", _neighbor[id].GetRight());
}

void BStarTree::printContour(const HorizontalContour& c) {
    cout << "---Current Contour---" << endl;
    for (auto it = c.begin(); it != c.end(); ++it) {
        cout << "x -> " << std::left << std::setw(5) << (*it).first << " y -> " << (*it).second << endl;
    }
    cout << endl;
}

void BStarTree::printResult() {
    for (int i = 0; i < (int)_nodes.size(); ++i) {
        _nodes[i].ShowResult();
    }
}

void BStarTree::printNeighborResult() {
    for (int i = 0; i < (int)_neighbor.size(); ++i) {
        _neighbor[i].ShowResult();
    }
}

void BStarTree::getblocks(std::vector<Block>& b) {
    std::vector<Block> vec;
    for (int i = 0; i < (int)_nodes.size(); ++i) {
        vec.push_back(_nodes[i]);
    }
    b.swap(vec);
}

void BStarTree::getneighborblocks(std::vector<Block>& b) {
    std::vector<Block> vec;
    for (int i = 0; i < (int)_neighbor.size(); ++i) {
        vec.push_back(_neighbor[i]);
    }
    b.swap(vec);
}

void BStarTree::report() {
    cout << "op1: " << _op1_count << endl;
    cout << "op2: " << _op2_count << endl;
    cout << "op3: " << _op3_count << endl;
    this->print();
}
