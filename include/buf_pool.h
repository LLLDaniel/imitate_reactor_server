#pragma once
#include <mutex>
#include <unordered_map>
#include "io_buf.h"
#include "macro_utils.h"

// memory size enum
enum MEM_CAP {
        m1k = 1024,
        m4k = 4096,
        m16k = 16384,
        m64k = 65536,
        m256k = 262144,
        m1M = 1048576,
        m4M = 4194304,
        m8M = 8388608,
};

// max size 4G, unit(k)
#define MEM_LIMIT 1024 * 1024 * 4

class BufPool {
        SINGLETON(BufPool)
    public:
        IoBuf* AllocBuf();
        IoBuf* AllocBuf(const int size);
        void revert(IoBuf* buffer);

        void MakeBufList(const MEM_CAP size, const int count);

    private:
        std::unordered_map<int, IoBuf*> buf_pool_;
        uint64_t total_mem_;
        std::mutex mtx_;
};
