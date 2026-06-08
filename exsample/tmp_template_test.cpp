template<template<class...> class T>
struct A {};

template<template<template<class...> class> class T>
struct B {};

template<template<template<template<class...> class> class> class T>
struct C {};

template<class T>
struct X;

template<template<class...> class T>
struct X<T> { static constexpr int value = 1; };

template<template<template<class...> class> class T>
struct X<T> { static constexpr int value = 2; };

template<template<template<template<class...> class> class> class T>
struct X<T> { static constexpr int value = 3; };

int main()
{
    static_assert(X<A>::value == 1);
    static_assert(X<B>::value == 2);
    static_assert(X<C>::value == 3);
}
