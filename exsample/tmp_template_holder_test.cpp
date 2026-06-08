template<template<class...> class T>
struct A {};

template<template<template<class...> class> class T>
struct B {};

template<template<template<template<class...> class> class> class T>
struct C {};

template<template<template<template<class...> class> class> class T>
struct Holder { static constexpr int value = 3; };

int main()
{
    static_assert(Holder<C>::value == 3);
}
