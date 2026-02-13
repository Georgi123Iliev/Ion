
#pragma once

#include <iostream>
#include<tuple>
#include<type_traits>

//NTTP callables only - so no stateful lambdas

template<typename Sig>
struct signature;

template<typename Ret, typename... Args>
struct signature<Ret(*)(Args...)>
{
    using ArgTypes = std::tuple<Args...>;
};

template<typename Ret, typename Class, typename... Args>
struct signature<Ret(Class::*)(Args...) const>
{
    using ArgTypes = std::tuple<Args...>;
};

template<typename T>
struct signature : public signature<decltype(&T::operator())>
{
};



template<auto Callable, size_t Index>
struct inspector
{
    using IthType = std::tuple_element_t<Index, typename signature<decltype(Callable)>::ArgTypes>;
};

template<auto Callable, size_t Index>
using inspector_t = typename inspector<Callable, Index>::IthType;

