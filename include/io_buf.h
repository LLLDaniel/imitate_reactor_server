#pragma once

/*
 * define a buffer struct of memory
 * */

class IoBuf {
    public:
        IoBuf(int size);
        ~IoBuf();

        void Clear();

        void Pop(const int len);

        void Adjust();

        void Copy(const IoBuf* other);

    private:
        int capacity_;
        // the avaibled memory length
        int length_;
        // the unresolved data header index
        int head_;
        // the buff header pointer
        char* data_;

        IoBuf* next_;
};
