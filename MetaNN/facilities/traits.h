#pragma once
#include <cstddef>

namespace MetaNN
{

template<typename T>
struct Identity_
{
    using type = T;
};

template<bool cur, typename TNext>
constexpr static bool AndValue = false;

template<typename TNext>
constexpr static bool AndValue<true, TNext> = TNext::value;

template<typename T>
using RemConstRef = std::remove_cv_t<std::remove_reference_t<T>>;
}