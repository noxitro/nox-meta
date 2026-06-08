#include <type_traits>
template<template<class...> class T> void f0();
template<template<template<class...> class> class T> void f1();
template<template<template<template<class...> class> class> class T> void f2();
template<template<class...> class T> class A {};
template<template<template<class...> class> class T> class B {};
template<template<template<template<class...> class> class> class T> class C {};
void test(){ f0<A>(); f1<B>(); f2<C>(); }