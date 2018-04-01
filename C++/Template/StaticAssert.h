//**************************************************************
//         static_assert implementation
//**************************************************************

#include <type_traits>

// forward declare template class taking a bool.
template<bool>
struct STATIC_ASSERT;

// complete specialization for bool is true.
// omit definition of the false specialization or general implementation.
// so if try to use it with a bool that is false, it cannot figure out what
// class to instantiate thus trigger a compile-time error.
template<>
struct STATIC_ASSERT<true>{};

void TestStaticAssert()
{
    static_assert(std::is_integral<int>::value, "given type is not a integral type.");
    static_assert(std::is_integral<bool>::value, "given type is not a integral type.");
    //static_assert(std::is_integral<float>::value, "given type is not a integral type.");

    STATIC_ASSERT<true>();
    //STATIC_ASSERT<false>();//compile error

    STATIC_ASSERT<std::is_integral<int>::value>();
    //STATIC_ASSERT<std::is_integral<float>::value>();//compile error
}

