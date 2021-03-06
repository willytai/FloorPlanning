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
    Block(std::string s, int w, int h) { _name = s; _width = w; _height = h; _LowerLeftX = _LowerLeftY = _parent = _left = _right = _id = -1; }
    ~Block() {}

    void init() {
        _width = _height = 0;
        _LowerLeftX = _LowerLeftY = _parent = _left = _right = _id = -1;
        _name = "";
    }

    // access functions
    std::string GetName        () const { return _name; }
    int         GetWidth       () const { return _width; }
    int         GetHeight      () const { return _height; }
    int         GetLowerLeftX  () const { return _LowerLeftX; }
    int         GetLowerLeftY  () const { return _LowerLeftY; }
    int         GetLowerRightX () const { return _LowerLeftX + _width; }
    int         GetUpperLeftY  () const { return _LowerLeftY + _height; }
    double      CenterX        () const { return _LowerLeftX + _width / 2.0; }
    double      CenterY        () const { return _LowerLeftY + _height / 2.0; }
    int         GetParent      () const { return _parent; }
    int         GetLeft        () const { return _left; }
    int         GetRight       () const { return _right; }
    int         GetID          () const { return _id; }

    // setting functions
    void SetResultX (int x)               { _LowerLeftX = x; }
    void SetResultY (int y)               { _LowerLeftY = y; }
    void SetName    (const std::string s) { _name = s; }
    void SetWidth   (const int w)         { _width = w; }
    void SetHeight  (const int h)         { _height = h; }
    void Rotate     ()                    { std::swap(_width, _height); }
    void SetParent  (int i)               { _parent = i; }
    void SetLeft    (int i)               { _left = i; }
    void SetRight   (int i)               { _right = i; }
    void SetID      (int i)               { _id = i; }

    // other
    void Summary() const {
        cout << "Block name: " << std::left << std::setw(10) << _name;
        cout << ' ' << "Block width: " << std::left << std::setw(4) << _width;
        cout << ' ' << "Block height: " << std::left << std::setw(4) << _height << endl;
    }
    void ShowResult() const {
        cout << "Block name: " << std::left << std::setw(10) << _name;
        cout << " LowerLeftX: " << std::left << std::setw(5) << _LowerLeftX;
        cout << " LowerLeftY: " << std::left << std::setw(5) << _LowerLeftY << endl;
    }
    bool Legal() const { return (_width != 0 && _height != 0); }

private:
    std::string _name;
    int         _width;
    int         _height;

    // result
    int _LowerLeftX;
    int _LowerLeftY;

    // for BStarTree
    int _parent;
    int _left;
    int _right;
    int _id;
};

}

#endif /* __BLOCK_H__ */
