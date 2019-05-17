#ifndef __MGR_H__
#define __MGR_H__

#include "Block.h"
#include "Net.h"
#include "Terminal.h"
#include "BStarTree.h"
#include "myUsage.h"
#include <sstream>
#include <map>

#define REPORT_INTERVAL 1
#define BOILING_POINT   10000
#define FREEZING_POINT  1
#define LAMBDA          0.9
#define P               50000
#define TIME_LIMIT      3000

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
    void FitOutline(MyUsage&, bool&);
    void OptWireLength(MyUsage&);
    void dump();

    double ComputeCurrentCost(bool&);
    double ComputeStage2Cost(bool&);
    int    ComputeFinalArea(bool&);
    int    ComputeCurrentArea(bool&);
    double ComputeFinalWireLength();
    double ComputeCurrentWireLength();
    double ComputeHPWL(const std::vector<std::pair<double, double> >&);

    void   ShowIntermidateResult() const;
    void   ShowFinalResult() const;

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

    // result
    std::vector<Block>    _final_result;
    double                _globalBestCost;
    int                   _chip_width;
    int                   _chip_height;
    double                _runtime;

    double                _curBestCost;

    // maps
    std::map<std::string, int> _block_index_name_map;
    std::map<std::string, int> _terminal_index_name_map;

    // current solution
    BStarTree _BTree;

    // the original wire length of the tree that fitted in the outline
    double _original_wire_length;
    double _original_area;

    void        parse                 (const ParserState&, std::string token, std::istringstream&);
    ParserState determin_parser_state (const std::string&) const;
};

}

#endif /* __MGR_H__ */
