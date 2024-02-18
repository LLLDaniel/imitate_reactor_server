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

        IoBuf* next_;

        int capacity_;
        // the avaibled memory length
        int length_;
        // the unresolved data header index
        int head_;

    private:
        // the buff header pointer
        char* data_;
};
