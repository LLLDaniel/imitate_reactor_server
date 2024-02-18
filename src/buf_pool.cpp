#include "buf_pool.h"
#include <iostream>
#include <thread>

BufPool::BufPool() {
        total_mem_ = 0;
        MakeBufList(m1k, 10000);
        MakeBufList(m4k, 5000);
        MakeBufList(m16k, 1000);
        MakeBufList(m64k, 500);
        MakeBufList(m256k, 200);
        MakeBufList(m1M, 100);
        MakeBufList(m4M, 50);
        MakeBufList(m8M, 50);
}

void BufPool::MakeBufList(const MEM_CAP size, const int count) {
        buf_pool_[size] = new IoBuf(size);
        if (buf_pool_[size] == nullptr) {
                std::cerr << "new io buffer error. buffer size:" << size
                          << std::endl;
                exit(-1);
        }
        IoBuf* prev = buf_pool_[size];
        for (int i = 0; i < count; ++i) {
                prev->next_ = new IoBuf(size);
                if (prev->next_ == nullptr) {
                        std::cerr << "new io buffer error. buffer size:" << size
                                  << std::endl;
                        exit(-1);
                }
                prev = prev->next_;
        }
        total_mem_ += size / 1024 * count;
}
IoBuf* BufPool::AllocBuf() { return AllocBuf(m1k); }
IoBuf* BufPool::AllocBuf(const int size) {
        int index;
        if (size <= m1k) {
                index = m1k;
        } else if (size <= m4k) {
                index = m4k;
        } else if (size <= m16k) {
                index = m16k;
        } else if (size <= m64k) {
                index = m64k;
        } else if (size <= m256k) {
                index = m256k;
        } else if (size <= m1M) {
                index = m1M;
        } else if (size <= m4M) {
                index = m4M;
        } else if (size <= m8M) {
                index = m8M;
        } else {
                std::cerr << "size is too large." << size << std::endl;
                return nullptr;
        }
        std::unique_lock<std::mutex>(mtx_);
        if (buf_pool_[index] == nullptr) {
                if (total_mem_ + index / 1024 >= MEM_LIMIT) {
                        std::cerr << "memory already large than limit."
                                  << std::endl;
                        return nullptr;
                }
                buf_pool_[index] = new IoBuf(index);
                if (buf_pool_[index] == nullptr) {
                        std::cerr << "new io buffer error. buffer size:" << size
                                  << std::endl;
                        exit(-1);
                }
                total_mem_ += index / 1024;
                return buf_pool_[index];
        }
        IoBuf* temp = buf_pool_[index];
        buf_pool_[index] = temp->next_;
        temp->next_ = nullptr;
        return temp;
}
void BufPool::revert(IoBuf* buffer) {
        if (buf_pool_.find(buffer->capacity_) == buf_pool_.end()) {
                std::cerr << "revert buffer capacity error:" << cap
                          << std::endl;
                return;
        }
        std::unique_lock<std::mutex>(mtx_);
        buffer->head_ = 0;
        buffer->length_ = 0;
        buffer->next_ = buf_pool_[cap];
        buf_pool_[cap] = buffer;
}
