#include "Mgr.h"

using namespace FloorPlanning;

void Mgr::Run() {
    _bestTree.init(_block);
    _bestTree.pack();
}

void Mgr::ShowResult() const {
    cout << endl << "********Result********" << endl;
    for (auto it = _block.begin(); it != _block.end(); ++it) {
        (*it).ShowResult();
    }
}
