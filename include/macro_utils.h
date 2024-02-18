#pragma once

#define SINGLETON(x)                          \
    public:                                   \
        static x* GetInstance() {             \
                static x* instance = new x(); \
                return instance;              \
        }                                     \
                                              \
    private:                                  \
        x();                                  \
        ~x();                                 \
        x(const x& other) = delete;
