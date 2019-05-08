#ifndef __NET_H__
#define __NET_H__

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
    int GetDegree() const { return (int)_terminals.size(); }

    // setting funtions
    void AddTerminalByName(std::string t) { _terminals.push_back(t); }

    // other
    void Summary() const {
        cout << "NetDegree: " << _terminals.size() << endl;
        for (auto it = _terminals.begin(); it != _terminals.end(); ++it)
            cout << *it << endl;
    }

private:
    std::vector<std::string> _terminals;
};

}

#endif /* __NET_H__ */
