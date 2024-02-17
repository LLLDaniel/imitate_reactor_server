#include "io_buf.h"
#include <assert.h>
#include <string.h>
#include <iostream>

IoBuf::IoBuf(int size) {
        capacity_ = size;
        length_ = 0;
        head_ = 0;
        next_ = nullptr;

        data_ = new char[size];
        assert(data_ != nullptr);
}
IoBuf::~IoBuf() {
        if (data_) {
                delete[] data_;
                data_ = nullptr;
        }
}

void IoBuf::Clear() {
        head_ = 0;
        length_ = 0;
}

void IoBuf::Pop(const int len) {
        if (len > capacity_) {
                std::cerr << "io buffer pop length error." << std::endl;
                return;
        }
        length_ -= len;
        head_ += len;
}

void IoBuf::Adjust() {
        if (head_ != 0) {
                if (length_ != 0) {
                        memmove(data_, data_ + head_, length_);
                }
                head_ = 0;
        }
}

void IoBuf::Copy(const IoBuf* other) {
        if (other->capacity_ != capacity_) {
                std::cerr << "Copy error, io buffer capacity is not equal."
                          << std::endl;
                return;
        }
        memcpy(data_, other->data_ + other->head_, other->length_);
        head_ = 0;
        length_ = other->length_;
}
