#pragma once
#include "MetaNN/facilities/null_param.h"
#include "MetaNN/facilities/traits.h"

namespace MetaNN
{

namespace NSMultiTypeDict
{

template<size_t N, template<typename...> class TCont, typename... T>
struct Create_
{
    using type = typename Create_<N-1, TCont, NullParameter, T...>::type;
};

template<size_t N, template<typename...> class TCont, typename... T>
struct Create_<0, Tcont, T...>
{
    using type = TCont<T...>;
};

template<typename TFindTag, size_t N, typename TCurTag, typename...TTags>
struct Tag2ID_
{
    constexpr static size_t value = Tag2ID_<TFindTag, N+1, TTags...>::value;
};

template<typename TFindTag, size_t N, typename TCurTag, typename...TTags>
struct Tag2ID_<TFindTag, N, TFindTag, TTags...>
{
    constexpr static size_t value = N;
};

template<typename TFindTag, typename... TTags>
constexpr size_t Tag2ID = Tag2ID_<TFindTag, 0, TTags...>::value;

template<typename TVal, size_t N, size_t M, typename TProcessedTypes, typename... TRemainTypes>
struct NewTupleType_;

template<typename TVal, size_t N, size_t M, template<typename...> class TCont,
        typename... TModifiedTypes, typename TCurType, typename... TRemainTypes>
struct NewTupleType_<TVal, N, M, TCont<TModifiedTypes...>,
                    TCurType, TRemainTypes...>
{
    using type = typename NewTupleType_<TVal, N, M+1, TCont<TModifiedTypes..., TCurType>, TRemainTypes...>
};

template<typename TVal, size_t N, template<typename...> class TCont,
        typename... TModifiedTypes, typename TCurType, typename... TRemainTypes>
struct NewTupleType_<TVal, N, N, TCont<TModifiedTypes...>,
                    TCurType, TRemainTypes...>
{
    using type = TCont<TModifiedTypes..., TVal, TRemainTypes...>;
};

template<typaname TVal, size_t TagPos, typename TCont, typename... TRemainTypes>
using NewTupleType = typename NewTupleType_<TVal, TagPos, 0, TCont, TRemainTypes...>::type;

template<size_t Pos, typename... TTags>
struct Pos2Type_
{
    static_assert((Pos != 0), "Invalid position.");
};

template<size_t Pos, typename TCur, typename...TTags>
struct Pos2Type_<Pos, TCur, TTags...>
{
    using type = typename std::condition_t<(Pos == 0),
                                            Identity_<TCur>,
                                            Pos2Type_<Pos-1, TTags...>>::type;
};

template<size_t Pos, typename... TTags>
using Pos2Type = typename Pos2Type_<Pos, TTags...>::type;

}



template <typename... TParameters>
struct VarTypeDict
{
public:
    template<typename... TTypes>
    struct Values {
        Values() = default;
        Values(std::shared_ptr<void>(&&input)[sizeof...(TTypes)]) {
            for (size_t i = 0; i < sizeof...(TTypes); ++i) {
                m_tuple[i] = std::move(input[i]);
            }
        }

    public:
        template<typename TTag, typename TVal>
        auto Set(TVal&& val) && 
        {
            using namespace NSMultiTypeDict;
            constexpr static size_t TagPos = Tag2ID<TTag, TParameters...>;
            using rawVal = std::decay_t<TVal>;
            ralVal* tmp = new rawVal(std::forward<TVal>(val));
            m_tuple[TagPos] = std::shared_ptr<void>(tmp,
                                [](void* ptr){
                                    rawVal* nptr = static_cast<rawVal*>(ptr);
                                    delete nptr;
                                });
            using new_type = NewTupleType<rawVal, TagPos, Values<>, TTypes...>;
            return new_type(std::move(m_tuple));
        }

        template<typename TTag>
        auto& Get() const
        {
            using namespace NSMultiTypeDict;
            constexpr static size_t TagPos = Tag2ID<TTag, TParameters...>;
            using AimType = Pos2Type<TagPos, TTypes...>;
            void* tmp = m_tuple[TagPos].get();
            AimType* res = static_cast<AimType*>(tmp);
            return *res;
        }

    private:
        std::shared_ptr<void> m_tuple[sizeof...(TTypes)];
    };
private:
    static auto Create() {
        using type = typename NSMultiTypeDict::Create_<sizeof...(TParameters), Values>::type;
        return type{};
    }
};

}