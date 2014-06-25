#include "simulator.h"
namespace Simulator {
    uint_32t_word Simulator::getVPN(uint_32t_word VA, int mode) {
        return (VA >> getOffsetBits(mode));
    }
    // ==========================
    // mode:
    //      0 => I
    //      1 => D
    void Simulator::read(uint_32t_word VA, int mode) {
        if (mode == 0) {
            // read for I
            uint_32t_word VPN = getVPN(VA, mode);
            int PPN = findITLBPPN(VPN);
            if (PPN != -1) {
                ITLB.hits++;
            } else {
                ITLB.misses++;
                PPN = findIPTEPPN(VPN);
                if (PPN != -1) {
                    IPTE.hits++;
                    updateITLBPPN(VPN, PPN);
                } else {
                    IPTE.misses++;
                    PPN = insertIMEM(VA);
                    updateIPTEPPN(VPN, PPN);
                    updateITLBPPN(VPN, PPN);
                }
            }
            queryICACHE(VA, PPN);
        } else {
            // read for D
            uint_32t_word VPN = getVPN(VA, mode);
            int PPN = findDTLBPPN(VPN);
            if (PPN != -1) {
                DTLB.hits++;
            } else {
                DTLB.misses++;
                PPN = findDPTEPPN(VPN);
                if (PPN != -1) {
                    DPTE.hits++;
                    updateDTLBPPN(VPN, PPN);
                } else {
                    DPTE.misses++;
                    PPN = insertDMEM(VA);
                    updateDPTEPPN(VPN, PPN);
                    updateDTLBPPN(VPN, PPN);
                }
            }
            queryDCACHE(VA, PPN);
        }
    }
    int Simulator::findIPTEPPN(uint_32t_word VPN) {
        int PPN = -1;
        for (int i = 0; i < IPTE.entry; i++) {
            if (IPTE.ppn[i].valid == true && IPTE.ppn[i].vpn == VPN) {
                PPN = IPTE.ppn[i].ppn;
                IPTE.ppn[i].LRU = LRUct++;
                break;
            }
        }
        return PPN;
    }
    int Simulator::findDPTEPPN(uint_32t_word VPN) {
        int PPN = -1;
        for (int i = 0; i < DPTE.entry; i++) {
            if (DPTE.ppn[i].valid == true && DPTE.ppn[i].vpn == VPN) {
                PPN = DPTE.ppn[i].ppn;
                DPTE.ppn[i].LRU = LRUct++;
                break;
            }
        }
        return PPN;
    }
    int Simulator::findITLBPPN(uint_32t_word VPN) {
        int PPN = -1;
        for (int i = 0; i < ITLB.entry; i++) {
            if (ITLB.ppn[i].valid == true && ITLB.ppn[i].vpn == VPN) {
                PPN = ITLB.ppn[i].ppn;
                ITLB.ppn[i].LRU = LRUct++;
                break;
            }
        }
        return PPN;
    }
    int Simulator::findDTLBPPN(uint_32t_word VPN) {
        int PPN = -1;
        for (int i = 0; i < DTLB.entry; i++) {
            if (DTLB.ppn[i].valid == true && DTLB.ppn[i].vpn == VPN) {
                PPN = DTLB.ppn[i].ppn;
                DTLB.ppn[i].LRU = LRUct++;
                break;
            }
        }
        return PPN;
    }
    void Simulator::updateITLBPPN(uint_32t_word VPN, int PPN) {
        int ind = -1;
        for (int i = 0; i < ITLB.entry; i++) {
            if (ITLB.ppn[i].valid == false) {
                ind = i;
            }
        }
        if (ind != -1) {
            ITLB.ppn[ind].valid = true;
            ITLB.ppn[ind].vpn = VPN;
            ITLB.ppn[ind].ppn = PPN;
        } else {
            int LLRU = 2147483647;
            for (int i = 0; i < ITLB.entry; i++) {
                if (ITLB.ppn[i].LRU < LLRU) {
                    LLRU = ITLB.ppn[i].LRU;
                    ind = i;
                }
            }
            ITLB.ppn[ind].valid = true;
            ITLB.ppn[ind].vpn = VPN;
            ITLB.ppn[ind].ppn = PPN;
        }
    }
    void Simulator::updateDTLBPPN(uint_32t_word VPN, int PPN) {
        int ind = -1;
        for (int i = 0; i < DTLB.entry; i++) {
            if (DTLB.ppn[i].valid == false) {
                ind = i;
            }
        }
        if (ind != -1) {
            DTLB.ppn[ind].valid = true;
            DTLB.ppn[ind].vpn = VPN;
            DTLB.ppn[ind].ppn = PPN;
        } else {
            int LLRU = 2147483647;
            for (int i = 0; i < DTLB.entry; i++) {
                if (DTLB.ppn[i].LRU < LLRU) {
                    LLRU = DTLB.ppn[i].LRU;
                    ind = i;
                }
            }
            DTLB.ppn[ind].valid = true;
            DTLB.ppn[ind].vpn = VPN;
            DTLB.ppn[ind].ppn = PPN;
        }
    }
    void Simulator::updateIPTEPPN(uint_32t_word VPN, int PPN) {
        int ind = -1;
        for (int i = 0; i < IPTE.entry; i++) {
            if (IPTE.ppn[i].valid == false) {
                ind = i;
            }
        }
        if (ind != -1) {
            IPTE.ppn[ind].valid = true;
            IPTE.ppn[ind].vpn = VPN;
            IPTE.ppn[ind].ppn = PPN;
        } else {
            int LLRU = 2147483647;
            for (int i = 0; i < IPTE.entry; i++) {
                if (IPTE.ppn[i].LRU < LLRU) {
                    LLRU = IPTE.ppn[i].LRU;
                    ind = i;
                }
            }
            IPTE.ppn[ind].valid = true;
            IPTE.ppn[ind].vpn = VPN;
            IPTE.ppn[ind].ppn = PPN;
        }
    }
    void Simulator::updateDPTEPPN(uint_32t_word VPN, int PPN) {
        int ind = -1;
        for (int i = 0; i < DPTE.entry; i++) {
            if (DPTE.ppn[i].valid == false) {
                ind = i;
            }
        }
        if (ind != -1) {
            DPTE.ppn[ind].valid = true;
            DPTE.ppn[ind].vpn = VPN;
            DPTE.ppn[ind].ppn = PPN;
        } else {
            int LLRU = 2147483647;
            for (int i = 0; i < DPTE.entry; i++) {
                if (DPTE.ppn[i].LRU < LLRU) {
                    LLRU = DPTE.ppn[i].LRU;
                    ind = i;
                }
            }
            DPTE.ppn[ind].valid = true;
            DPTE.ppn[ind].vpn = VPN;
            DPTE.ppn[ind].ppn = PPN;
        }
    }
    int Simulator::insertIMEM(uint_32t_word VA) {
        int PPN = -1;
        for (int i = 0; i < IMEM.size/IMEM.pageSize; i++) {
            if (IMEM.page[i].valid == false) {
                PPN = i;
            }
        }
        if (PPN != -1) {
            IMEM.page[PPN].valid = true;
            for (int i = 0; i < IMEM.pageSize/4; i++) {
                IMEM.page[PPN].space[i] = VA+i*4;
            }
            IMEM.page[PPN].LRU = LRUct++;
        } else {
            int LLRU = 2147483647;
            for (int i = 0; i < IMEM.size/IMEM.pageSize; i++) {
                if (IMEM.page[i].LRU < LLRU) {
                    LLRU = IMEM.page[i].LRU;
                    PPN = i;
                }
            }
            IMEM.page[PPN].valid = true;
            for (int i = 0; i < IMEM.pageSize/4; i++) {
                IMEM.page[PPN].space[i] = VA+i*4;
            }
            IMEM.page[PPN].LRU = LRUct++;
        }
        return PPN;
    }
    int Simulator::insertDMEM(uint_32t_word VA) {
        int PPN = -1;
        for (int i = 0; i < DMEM.size/DMEM.pageSize; i++) {
            if (DMEM.page[i].valid == false) {
                PPN = i;
            }
        }
        if (PPN != -1) {
            DMEM.page[PPN].valid = true;
            for (int i = 0; i < DMEM.pageSize/4; i++) {
                DMEM.page[PPN].space[i] = VA+i*4;
            }
            DMEM.page[PPN].LRU = LRUct++;
        } else {
            int LLRU = 2147483647;
            for (int i = 0; i < DMEM.size/DMEM.pageSize; i++) {
                if (DMEM.page[i].LRU < LLRU) {
                    LLRU = DMEM.page[i].LRU;
                    PPN = i;
                }
            }
            DMEM.page[PPN].valid = true;
            for (int i = 0; i < DMEM.pageSize/4; i++) {
                DMEM.page[PPN].space[i] = VA+i*4;
            }
            DMEM.page[PPN].LRU = LRUct++;
        }
        return PPN;
    }
    int Simulator::getOffsetBits(int mode) {
        int pageOffsetBits = 0, tmp;
        if (mode == 0) {
            tmp = IMEM.pageSize-1;
        } else {
            tmp = DMEM.pageSize-1;
        }
        while (tmp) {
            pageOffsetBits++;
            tmp /= 2;
        }
        return pageOffsetBits;
    }
    int Simulator::getBlockOffsetBits(int mode) {
        int blockOffsetBits = 0, tmp;
        if (mode == 0) {
            tmp = ICACHE.blockSize-1;
        } else {
            tmp = DCACHE.blockSize-1;
        }
        while (tmp) {
            blockOffsetBits++;
            tmp /= 2;
        }
        return blockOffsetBits;
    }
    int Simulator::getIndexOffsetBits(int mode) {
        int indexOffsetBits = 0, tmp;
        if (mode == 0) {
            tmp = ICACHE.totalSize/ICACHE.blockSize/ICACHE.setAssociativity-1;
        } else {
            tmp = DCACHE.totalSize/DCACHE.blockSize/DCACHE.setAssociativity-1;
        }
        while (tmp) {
            indexOffsetBits++;
            tmp /= 2;
        }
        return indexOffsetBits;
    }
    uint_32t_word Simulator::getPA(uint_32t_word VA, int PPN, int mode) {
        uint_32t_word PA = (uint_32t_word)PPN;
        uint_32t_word PageOffset = VA % (1 << getOffsetBits(mode));
        PA = (PA << getOffsetBits(mode))+PageOffset;
        return PA;
    }
    uint_32t_word Simulator::getTag(uint_32t_word PA, int mode) {
        uint_32t_word tmp = PA;
        tmp = tmp >> getBlockOffsetBits(mode);
        tmp = tmp >> getIndexOffsetBits(mode);
        return tmp;
    }
    uint_32t_word Simulator::getInd(uint_32t_word PA, int mode) {
        uint_32t_word tmp = PA;
        tmp = tmp >> getBlockOffsetBits(mode);
        return (tmp % (1 << getIndexOffsetBits(mode)));
    }
    void Simulator::queryICACHE(uint_32t_word VA, int PPN) {
        uint_32t_word PA = getPA(VA, PPN, 0);
        uint_32t_word tag = getTag(PA, 0);
        uint_32t_word ind = getInd(PA, 0);
        int hit = -1;
        for (int i = 0; i < ICACHE.setAssociativity; i++) {
            if (ICACHE.block[ind][i].valid == true && ICACHE.block[ind][i].tag == tag) {
                hit = i;
            }
        }
        if (hit != -1) {
            ICACHE.hits++;
            ICACHE.block[ind][hit].LRU = LRUct++;
        } else {
            ICACHE.misses++;
            for (int i = 0; i < ICACHE.setAssociativity; i++) {
                if (ICACHE.block[ind][i].valid == false) {
                    hit = i;
                }
            }
            if (hit != -1) {
                ICACHE.block[ind][hit].valid = true;
                ICACHE.block[ind][hit].tag = tag;
                ICACHE.block[ind][hit].LRU = LRUct++;
            } else {
                int LLRU = 2147483647;
                for (int i = 0; i < ICACHE.setAssociativity; i++) {
                    if (ICACHE.block[ind][i].LRU < LLRU) {
                        LLRU = ICACHE.block[ind][i].LRU;
                        hit = i;
                    }
                }
                ICACHE.block[ind][hit].valid = true;
                ICACHE.block[ind][hit].tag = tag;
                ICACHE.block[ind][hit].LRU = LRUct++;
            }
        }
    }
    void Simulator::queryDCACHE(uint_32t_word VA, int PPN) {
        uint_32t_word PA = getPA(VA, PPN, 1);
        uint_32t_word tag = getTag(PA, 1);
        uint_32t_word ind = getInd(PA, 1);
        int hit = -1;
        for (int i = 0; i < DCACHE.setAssociativity; i++) {
            if (DCACHE.block[ind][i].valid == true && DCACHE.block[ind][i].tag == tag) {
                hit = i;
            }
        }
        if (hit != -1) {
            DCACHE.hits++;
            DCACHE.block[ind][hit].LRU = LRUct++;
        } else {
            DCACHE.misses++;
            for (int i = 0; i < DCACHE.setAssociativity; i++) {
                if (DCACHE.block[ind][i].valid == false) {
                    hit = i;
                }
            }
            if (hit != -1) {
                DCACHE.block[ind][hit].valid = true;
                DCACHE.block[ind][hit].tag = tag;
                DCACHE.block[ind][hit].LRU = LRUct++;
            } else {
                int LLRU = 2147483647;
                for (int i = 0; i < DCACHE.setAssociativity; i++) {
                    if (DCACHE.block[ind][i].LRU < LLRU) {
                        LLRU = DCACHE.block[ind][i].LRU;
                        hit = i;
                    }
                }
                DCACHE.block[ind][hit].valid = true;
                DCACHE.block[ind][hit].tag = tag;
                DCACHE.block[ind][hit].LRU = LRUct++;
            }
        }
    }
    void Simulator::write(uint_32t_word VA, int mode) {
        if (mode == 0) {
            // Do nothing
            // Nobody can write I part
            read(VA, mode);
        } else {
            read(VA, mode);
        }
    }
} // namespace Simulator
