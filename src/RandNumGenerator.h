#ifndef __RANDNUMGENERATOR_H__
#define __RANDNUMGENERATOR_H__

#include <cstdlib>
#include <ctime>

#define SEED 5

namespace INT
{

class RandNumGenerator
{
public:
    RandNumGenerator() { init(); }
    ~RandNumGenerator() {}

    void init() {
        srand(SEED);
        // srand(time(NULL));
    }

    int Generate(int low, int high) {
        return rand() % (high - low + 1) + low;
    }

    int Generate() { return rand(); }
};

}

namespace FLOAT
{

class RandNumGenerator
{
public:
    RandNumGenerator() { init(); }
    ~RandNumGenerator() {}

    void init() {
        srand(SEED);
        // srand(time(NULL));
    }

    double Generate(double low, double high) {
        return (double)rand() * (high - low) / (RAND_MAX + 1.0) + low;
    }
};

}


#endif /* __RANDNUMGENERATOR_H__ */
