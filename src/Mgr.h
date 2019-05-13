#ifndef __MGR_H__
#define __MGR_H__

#include "Block.h"
#include "Net.h"
#include "Terminal.h"
#include "BStarTree.h"
#include <sstream>

namespace FloorPlanning
{

enum ParserState
{
    ParseOutline,
    ParseNumBlocks,
    ParseNumTerminals,
    ParseBlock,
    ParseTerminal,
    ParseNumNet,
    CreateNet,
    AddLatestNetCompnent
};

class Mgr
{
public:
    Mgr(double a, char* block, char* net, char* out) { _alpha = a; _input_block = block; _input_net = net; _output_filename = out; }
    ~Mgr() {}

    // Top API
    void Parse();
    void Summary();
    void Run();

    void ShowResult() const;

private:

    // command line parameters
    double _alpha;
    char*  _input_block;
    char*  _input_net;
    char*  _output_filename;


    // layout information
    int    _outline_width;
    int    _outline_height;
    int    _num_blocks;
    int    _num_terminals;
    int    _num_nets;
    
    std::vector<Block>    _block;
    std::vector<Terminal> _terminal;
    std::vector<Net>      _net;

    // current best solution
    BStarTree _bestTree;

    void        parse                 (const ParserState&, std::string token, std::istringstream&);
    ParserState determin_parser_state (const std::string&) const;
};

}

#endif /* __MGR_H__ */
