#include "Mgr.h"
#include "RandNumGenerator.h"
#include <cmath>
#include <cassert>

extern INT::RandNumGenerator RandIntGen;

namespace FloorPlanning
{

void Mgr::FitOutline(MyUsage& usg, bool& legal) {
    _runtime = 0.0;
    _BTree.init(_block);
    _BTree.pack();
    _BTree.getblocks(_block);
    _curBestCost = this->ComputeCurrentCost(legal);
    _globalBestCost = _curBestCost;
    cout << "[OutLine SA] Initial Cost: " << _curBestCost << endl;
    int bound = REPORT_INTERVAL;
    int T     = BOILING_POINT;
    while (T > FREEZING_POINT) {
        for (int i = 0; i < P; ++i) {
            _BTree.perturbation();
            _BTree.packNeighbor();
            _BTree.getneighborblocks(_block);
            double curCost = this->ComputeCurrentCost(legal);
            if (curCost <= _globalBestCost) {
                _final_result   = _block;
                _globalBestCost = curCost;
            }
            if (curCost <= _curBestCost) {
                _curBestCost  = curCost;
                _BTree.update();
                if (legal) {
                    assert(_curBestCost == _globalBestCost);
                    break;
                }
            }
            else {
                int dice = RandIntGen.Generate(0, 1000000);
                double threshold = std::exp(-(curCost - _curBestCost) / T);
                // cout << std::setw(5) << curCost - _curBestCost << ' ' << std::setw(10) << std::exp(-(curCost - _curBestCost) / T) << ' ' << T << endl;
                if (dice < threshold*1000000) {
                    _curBestCost  = curCost;
                    _BTree.update();
                }
            }
            _runtime += usg.GetTimeUsage();
            if (_runtime > bound) {
                cout << "[OutLine SA] curBestCost " << _curBestCost;
                cout << " globalBestCost " << _globalBestCost;
                cout << " Temperature " << std::left << std::setw(4) << T;
                cout << " time " << std::left << std::setw(4) << _runtime << 's';
                cout << endl;
                bound += REPORT_INTERVAL;
            }
            if (_runtime > TIME_LIMIT) break;
        }
        if (legal) break;
        if (_runtime > TIME_LIMIT) break;
        T *= LAMBDA;
    }
    if (legal) cout << "[OutLine SA] Found a Legal Solution!" << endl;
    else cout << "[OutLine SA] Solution Not Found!" << endl;

    _original_area        = this->ComputeFinalArea(legal);
    _original_wire_length = this->ComputeFinalWireLength();
}

void Mgr::OptWireLength(MyUsage& usg) {
    bool legal = false;
    _curBestCost = this->ComputeStage2Cost(legal); assert(legal);
    _globalBestCost = _curBestCost;
    cout << "[WireLength SA] Initial Cost: " << _curBestCost << endl;
    int bound = REPORT_INTERVAL;
    int T     = BOILING_POINT/10;
    while (T > FREEZING_POINT) {
        for (int i = 0; i < P/10; ++i) {
            _BTree.perturbation();
            _BTree.packNeighbor();
            _BTree.getneighborblocks(_block);
            double curCost = this->ComputeStage2Cost(legal);
            if (!legal) continue;
            if (curCost < _globalBestCost) {
                _final_result   = _block;
                _globalBestCost = curCost;
            }
            if (curCost < _curBestCost) {
                _curBestCost  = curCost;
                _BTree.update();
            }
            else {
                int dice = RandIntGen.Generate(0, 1000000);
                double threshold = std::exp(-(curCost - _curBestCost) / T);
                // cout << std::setw(5) << curCost - _curBestCost << ' ' << std::setw(10) << std::exp(-(curCost - _curBestCost) / T) << ' ' << T << endl;
                if (dice < threshold*1000000) {
                    _curBestCost  = curCost;
                    _BTree.update();
                }
            }
            _runtime += usg.GetTimeUsage();
            if (_runtime > bound) {
                cout << "[WireLength SA] curBestCost " << _curBestCost;
                cout << " globalBestCost " << _globalBestCost;
                cout << " Temperature " << std::left << std::setw(4) << T;
                cout << " time " << std::left << std::setw(4) << _runtime << 's';
                cout << endl;
                bound += REPORT_INTERVAL;
            }
            if (_runtime > TIME_LIMIT) break;
        }
        if (_runtime > TIME_LIMIT) break;
        T *= LAMBDA;
    }
}

}
