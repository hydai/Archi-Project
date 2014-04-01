#ifndef SIMULATOR_H_
#define SIMULATOR_H_

#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace Simulator {
    const int MEMORY_SIZE = 1024;
    const int REG_SIZE = 32;
    typedef uint_32t unsigned int;
    class Simulator {
        public:
            Simulator();
            ~Simulator();
            void run();
        private:
            uint_32t imemory[MEMORY_SIZE/4], dmemory[MEMORY_SIZE/4];
            uint_32t reg[REG_SIZE];
            uint_32t pc;
            FILE *iimage, *dimage, *errordump, *snapshot;
            void init();
            bool loadData();
            void loadIimage();
            void loadDimage();
            void dump();
            

    };
} // namespace Simulator

#endif
