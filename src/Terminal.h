#ifndef __TERMINAL_H__
#define __TERMINAL_H__

#include <string>
#include <iostream>
#include <iomanip>

using std::cout;
using std::endl;

namespace FloorPlanning
{

class Terminal
{
public:
    Terminal() { init(); }
    Terminal(std::string s, int x, int y) { _name = s; _x = x; _y = y; }
    ~Terminal() {}
    
    void init() {
        _name = "";
        _x = _y = -1;
    }

    // access functions
    std::string GetName() const { return _name; }
    int         GetX()    const { return _x; }
    int         GetY()    const { return _y; }

    // setting functions
    void SetName (std::string s) { _name = s; }
    void SetX    (int x)         { _x = x; }
    void SetY    (int y)         { _y = y; }

    // other
    void Summary() const {
        cout << "Terminal name: " << std::left << std::setw(10) << _name;
        cout << ' ' << "X: " << std::left << std::setw(4) << _x;
        cout << ' ' << "Y: " << std::left << std::setw(4) << _y << endl;
    }

private:
    std::string _name;
    int         _x;
    int         _y;
};

}

#endif /* __TERMINAL_H__ */
