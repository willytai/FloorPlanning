#include "myUsage.h"
#include "Mgr.h"
#include "RandNumGenerator.h"

INT::RandNumGenerator   RandIntGen;
FLOAT::RandNumGenerator RandFloatGen;

int main(int argc, char *argv[])
{
    if (argc != 5) {
        std::cout << "usage: ./fp <alpha> <input.block name> <input.net name> <output file name>" << std::endl;
        exit(-1);
    }
    double alpha      = std::stof(argv[1]);
    char* input_block = argv[2];
    char* input_net   = argv[3];
    char* out         = argv[4];
    MyUsage usg;
    FloorPlanning::Mgr mgr(alpha, input_block, input_net, out);
    usg.reset(); // tic

    mgr.Parse();
    mgr.Run();
    // mgr.ShowResult();

    usg.report(1, 1); // toc
    return 0;
}
