#include "stdafx.h"
#include "Reflection.h"

struct TypeDescriptor_Int : TypeDescriptor
{
    TypeDescriptor_Int() : TypeDescriptor{"int", sizeof(int)}
    {
    }
    virtual void dump(const void* obj, int /* unused */) const override
    {
        std::cout << "int{" << *(const int*) obj << "}";
    }
};

template <>
TypeDescriptor* getPrimitiveDescriptor<int>()
{
    static TypeDescriptor_Int typeDesc;
    return &typeDesc;
}

struct TypeDescriptor_StdString : TypeDescriptor
{
    TypeDescriptor_StdString() : TypeDescriptor{"std::string", sizeof(std::string)}
    {
    }
    virtual void dump(const void* obj, int /* unused */) const override
    {
        std::cout << "std::string{\"" << *(const std::string*) obj << "\"}";
    }
};

template <>
TypeDescriptor* getPrimitiveDescriptor<std::string>()
{
    static TypeDescriptor_StdString typeDesc;
    return &typeDesc;
}