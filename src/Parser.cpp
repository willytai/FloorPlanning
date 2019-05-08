#include "Mgr.h"
#include "myUsage.h"
#include <fstream>
#include <iostream>

using std::cerr;
using std::cout;
using std::endl;
using namespace FloorPlanning;

void Mgr::Summary() {
    cout << "[Parser]" << endl;
    cout << "\t * Block File -> " << _input_block << endl;
    cout << "\t * Net File   -> " << _input_net << endl;
    cout << "\t > Outline: " << _outline_width << "(Width) " << _outline_height << "(Height)" << endl;
    cout << "\t > Number of Blocks: " << _num_blocks << endl;
    cout << "\t > Number of Terminlas: " << _num_terminals << endl;
    cout << "\t > Number of Nets: " << _num_nets << endl;
}

void Mgr::Parse() {
    MyUsage usg; usg.reset();
    std::fstream BlockFile, NetFile;
    BlockFile.open(_input_block);
    if (!BlockFile.is_open()) {
        cerr << "Cannot open block file: " << _input_block << endl;
        exit(-1);
    }
    NetFile.open(_input_net);
    if (!NetFile.is_open()) {
        cerr << "Cannot open net file: " << _input_net << endl;
        exit(-1);
    }

    // parse block file
    std::string buffer;
    ParserState state;
    while (getline(BlockFile, buffer)) {
        if (buffer.length() < 3) continue;
        std::istringstream iss(buffer);
        std::string token;
        iss >> token;
        state = this->determin_parser_state(token);
        this->parse(state, token, iss);
    }
    while (getline(NetFile, buffer)) {
        if (buffer.length() < 1) continue;
        std::istringstream iss(buffer);
        std::string token;
        iss >> token;
        state = this->determin_parser_state(token);
        this->parse(state, token, iss);
    }
    this->Summary();
    cout << "\t > CPU time: " << usg.GetTimeUsage() << 's' << endl;
}

void Mgr::parse(const ParserState& state, std::string token, std::istringstream& iss) {
    switch (state) {
        case ParseOutline:
            {
                iss >> token;
                _outline_width = std::stoi(token);
                iss >> token;
                _outline_height = std::stoi(token);
                break;
            }

        case ParseNumBlocks:
            {
                iss >> token;
                _num_blocks = std::stoi(token);
                break;
            }

        case ParseNumTerminals:
            {
                iss >> token;
                _num_terminals = std::stoi(token);
                break;
            }

        case ParseBlock:
            {
                Block newBlock;
                newBlock.SetName(token);
                iss >> token;
                newBlock.SetWidth(std::stoi(token));
                iss >> token;
                newBlock.SetHeight(std::stoi(token));
                _block.push_back(newBlock);
                break;
            }

        case ParseTerminal:
            {
                Terminal newTerminal;
                newTerminal.SetName(token);
                iss >> token; iss >> token;
                newTerminal.SetX(std::stoi(token));
                iss >> token;
                newTerminal.SetY(std::stoi(token));
                _terminal.push_back(newTerminal);
                break;
            }

        case ParseNumNet:
            {
                iss >> token;
                _num_nets = std::stoi(token);
                break;
            }

        case CreateNet:
            {
                Net newNet;
                _net.push_back(newNet);
                break;
            }

        case AddLatestNetCompnent:
            {
                _net.back().AddTerminalByName(token);
            }
    }
}

ParserState Mgr::determin_parser_state(const std::string& pattern) const {
    if (pattern == "Outline:")                   return ParseOutline;
    if (pattern == "NumBlocks:")                 return ParseNumBlocks;
    if (pattern == "NumTerminals:")              return ParseNumTerminals;
    if ((int)_block.size() != _num_blocks)       return ParseBlock;
    if ((int)_terminal.size() != _num_terminals) return ParseTerminal;
    if (pattern == "NumNets:")                   return ParseNumNet;
    if (pattern == "NetDegree:")                 return CreateNet;
    return AddLatestNetCompnent;
}
