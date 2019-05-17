#include "Mgr.h"
#include "RandNumGenerator.h"
#include "myUsage.h"
#include <fstream>
#include <limits>
#include <cmath>
#include <cassert>

using namespace FloorPlanning;

void Mgr::Run() {
    MyUsage usg; usg.reset();
    bool legal = false;
    this->FitOutline(usg, legal);
    if (legal) this->OptWireLength(usg);
}

double Mgr::ComputeCurrentCost(bool& legal) {
    int MaxX = 0, MaxY = 0;
    for (int i = 0; i < (int)_block.size(); ++i) {
        if (_block[i].GetLowerRightX() > MaxX) MaxX = _block[i].GetLowerRightX();
        if (_block[i].GetUpperLeftY()  > MaxY) MaxY = _block[i].GetUpperLeftY();
    }
    if (MaxX <= _outline_width && MaxY <= _outline_height) {
        legal = true;
    }
    else {
        legal = false;
    }

    double Area = MaxX * MaxY;
    double AS_penalty;
    if (_outline_width > _outline_height) {
        AS_penalty = std::abs((double)_outline_width / (double)_outline_height - (double)MaxX / (double)MaxY);
        AS_penalty = AS_penalty * AS_penalty;
    }
    else {
        AS_penalty = std::abs((double)_outline_height / (double)_outline_width - (double)MaxY / (double)MaxX);
        AS_penalty = AS_penalty * AS_penalty;
    }

    double cost = Area / (_outline_width * _outline_height) * 0.5 + AS_penalty * 2;

    // cout << Area / (_outline_width * _outline_height) * 0.5 << ' ' << AS_penalty * 2 << endl;

    return cost * BOILING_POINT * 10;
}

double Mgr::ComputeStage2Cost(bool& legal) {
    int          Area = this->ComputeCurrentArea(legal);
    double WireLength = this->ComputeCurrentWireLength();

    double cost = WireLength / _original_wire_length;
    // double cost = _alpha * Area / _original_area + (1 - _alpha) * WireLength / _original_wire_length;

    return cost * BOILING_POINT;
}

int Mgr::ComputeFinalArea(bool& legal) {
    std::cout << "[Mgr] Computing the Area of Final Result!" << std::endl;
    int MaxX = 0, MaxY = 0;
    for (int i = 0; i < (int)_final_result.size(); ++i) {
        if (_final_result[i].GetLowerRightX() > MaxX) MaxX = _final_result[i].GetLowerRightX();
        if (_final_result[i].GetUpperLeftY()  > MaxY) MaxY = _final_result[i].GetUpperLeftY();
    }
    if (MaxX <= _outline_width && MaxY <= _outline_height) legal = true;
    else {
        legal = false;
    }
    _chip_width = MaxX;
    _chip_height = MaxY;
    return MaxX * MaxY;
}

int Mgr::ComputeCurrentArea(bool& legal) {
    int MaxX = 0, MaxY = 0;
    for (int i = 0; i < (int)_block.size(); ++i) {
        if (_block[i].GetLowerRightX() > MaxX) MaxX = _block[i].GetLowerRightX();
        if (_block[i].GetUpperLeftY()  > MaxY) MaxY = _block[i].GetUpperLeftY();
    }
    if (MaxX <= _outline_width && MaxY <= _outline_height) {
        legal = true;
    }
    else {
        legal = false;
    }

    double Area = MaxX * MaxY;
    return Area;
}

double Mgr::ComputeFinalWireLength() {
    // update _block_index_name_map
    for (int i = 0; i < (int)_final_result.size(); ++i) {
        _block_index_name_map[_final_result[i].GetName()] = i;
    }

    double W = 0;
    for (int i = 0; i < (int)_net.size(); ++i) {
        std::vector<std::pair<double, double> > points;
        for (int j = 0; j < _net[i].GetBlockNum(); ++j) {
            const std::string& name = _net[i].GetBlockName(j);
            int id = _block_index_name_map[name];
            double x = _final_result[id].CenterX();
            double y = _final_result[id].CenterY();
            points.push_back(pair<double, double>(x, y));
        }
        for (int j = 0; j < (int)_net[i].GetTerminalNum(); ++j) {
            const std::string& name = _net[i].GetTerminalName(j);
            int id = _terminal_index_name_map[name];
            double x = _terminal[id].CenterX();
            double y = _terminal[id].CenterY();
            points.push_back(pair<double, double>(x, y));
        }
        W += this->ComputeHPWL(points);
    }
    return W;
}

double Mgr::ComputeCurrentWireLength() {
    // update _block_index_name_map
    for (int i = 0; i < (int)_block.size(); ++i) {
        _block_index_name_map[_block[i].GetName()] = i;
    }

    double W = 0;
    for (int i = 0; i < (int)_net.size(); ++i) {
        std::vector<std::pair<double, double> > points;
        for (int j = 0; j < _net[i].GetBlockNum(); ++j) {
            const std::string& name = _net[i].GetBlockName(j);
            int id = _block_index_name_map[name];
            double x = _block[id].CenterX();
            double y = _block[id].CenterY();
            points.push_back(pair<double, double>(x, y));
        }
        for (int j = 0; j < (int)_net[i].GetTerminalNum(); ++j) {
            const std::string& name = _net[i].GetTerminalName(j);
            int id = _terminal_index_name_map[name];
            double x = _terminal[id].CenterX();
            double y = _terminal[id].CenterY();
            points.push_back(pair<double, double>(x, y));
        }
        W += this->ComputeHPWL(points);
    }
    return W;
}

double Mgr::ComputeHPWL(const std::vector<std::pair<double, double> >& points) {
    double MaxX = 0, MaxY = 0, MinX = std::numeric_limits<double>::max(), MinY = std::numeric_limits<double>::max();
    for (auto it = points.begin(); it != points.end(); ++it) {
        if ((*it).first > MaxX) MaxX = (*it).first;
        if ((*it).first < MinX) MinX = (*it).first;
        if ((*it).second > MaxY) MaxY = (*it).second;
        if ((*it).second < MinY) MinY = (*it).second;
    }
    return MaxX - MinX + MaxY - MinY;
}

void Mgr::ShowIntermidateResult() const {
    cout << endl << "********Intermidate Result********" << endl;
    for (auto it = _block.begin(); it != _block.end(); ++it) {
        (*it).ShowResult();
    }
}

void Mgr::ShowFinalResult() const {
    cout << endl << "********Final Result********" << endl;
    for (auto it = _final_result.begin(); it != _final_result.end(); ++it) {
        (*it).ShowResult();
    }
}

void Mgr::dump() {
    bool dummy;
    std::ofstream file(_output_filename);
    double Area = this->ComputeFinalArea(dummy);
    double WireLength = this->ComputeFinalWireLength();
    file << std::fixed;
    file << _alpha * Area + (1 - _alpha) * WireLength << std::endl;
    file << WireLength << std::endl;
    file << Area << endl;
    file << _chip_width << ' ' << _chip_height << endl;
    file << _runtime << endl;
    for (int i = 0; i < (int)_final_result.size(); ++i) {
        file << _final_result[i].GetName() << ' ';
        file << _final_result[i].GetLowerLeftX() << ' ' << _final_result[i].GetLowerLeftY() << ' ' << _final_result[i].GetLowerRightX() << ' ' << _final_result[i].GetUpperLeftY() << endl;
    }
}
