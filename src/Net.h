#ifndef __NET_H__
#define __NET_H__

#include "Terminal.h"
#include "Block.h"
#include <vector>
#include <string>
#include <iostream>

using std::cout;
using std::endl;

namespace FloorPlanning
{

class Net
{
public:
    Net() {}
    ~Net() {}

    // access funtions
    int GetDegree() const { return (int)_terminal.size() + (int)_block.size(); }
    std::string& GetBlockName(const int& i)    { return _block[i]; }
    std::string& GetTerminalName(const int& i) { return _terminal[i]; }
    int GetBlockNum() const    { return (int)_block.size(); }
    int GetTerminalNum() const { return (int)_terminal.size(); }

    // setting funtions
    void AddTerminal(std::string t) { _terminal.push_back(t); }
    void AddBlock(std::string b)    { _block.push_back(b); }

    // other
    void Summary() const {
        cout << "NetDegree: " << this->GetDegree() << endl;
        for (auto it = _block.begin(); it != _block.end(); ++it)
            cout << (*it) << endl;
        for (auto it = _terminal.begin(); it != _terminal.end(); ++it)
            cout << (*it) << endl;
    }

private:
    std::vector<std::string>  _block;
    std::vector<std::string>  _terminal;
};

}

#endif /* __NET_H__ */
