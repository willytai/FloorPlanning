#ifndef __BLOCK_H__
#define __BLOCK_H__

#include <string>
#include <iomanip>
#include <iostream>

using std::cout;
using std::endl;

namespace FloorPlanning
{

class Block
{
public:
    Block() { init(); }
    Block(std::string s, int w, int h) { _name = s; _width = w; _height = h; }
    ~Block() {}

    void init() {
        _width = _height = 0;
        _name = "";
    }

    // access functions
    std::string GetName()   const { return _name; }
    int         GetWidth()  const { return _width; }
    int         GetHeight() const { return _height; }

    // setting functions
    void SetName   (const std::string s) { _name = s; }
    void SetWidth  (const int w)         { _width = w; }
    void SetHeight (const int h)         { _height = h; }

    // other
    void Summary() const {
        cout << "Block name: " << std::left << std::setw(10) << _name;
        cout << ' ' << "Block width: " << std::left << std::setw(4) << _width;
        cout << ' ' << "Block height: " << std::left << std::setw(4) << _height << endl;
    }

private:
    std::string _name;
    int         _width;
    int         _height;
};

}

#endif /* __BLOCK_H__ */
