#include "simulator.h"
namespace Simulator {
Simulator::Simulator() {
    memset(imemory, 0, sizeof(imemory));
    memset(dmemory, 0, sizeof(dmemory));
    memset(reg, 0, sizeof(reg));
}

Simulator::~Simulator() {
    // Do nothing
}

void Simulator::run() {
    // TODO
}
} // namespace Simulator
