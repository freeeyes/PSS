#ifndef _PSS_singleton_H_
#define _PSS_singleton_H_
#include <new>
#include <memory>
#include <functional>

// CLASS TEMPLATE PSS_singleton forward decl
template<typename _Ty, bool delayed = false>
class PSS_singleton;

/// CLASS TEMPLATE PSS_singleton
/// the managed PSS_singleton object will be destructed after main function.
template<typename _Ty>
class PSS_singleton<_Ty, false>
{
    typedef PSS_singleton<_Ty, false> _Myt;
    typedef _Ty* pointer;
public:
    template<typename ..._Args>
    static pointer instance(_Args...args)
    {
        if (nullptr == _Myt::__single__.get())
        {
            if (nullptr == _Myt::__single__.get())
            {
                _Myt::__single__.reset(new(std::nothrow) _Ty(args...));
            }
        }
        return _Myt::__single__.get();
    }

    static void destroy(void)
    {
        if (_Myt::__single__.get() != nullptr)
        {
            _Myt::__single__.reset();
        }
    }

private:
    static std::unique_ptr<_Ty> __single__;
private:
    PSS_singleton(void) = delete; // just disable construct, assign operation, copy construct also not allowed.
};

/// CLASS TEMPLATE PSS_singleton, support delay init with variadic args
/// the managed PSS_singleton object will be destructed after main function.
template<typename _Ty>
class PSS_singleton<_Ty, true>
{
    typedef PSS_singleton<_Ty, true> _Myt;
    typedef _Ty* pointer;
public:
    template<typename ..._Args>
    static pointer instance(_Args...args)
    {
        if (nullptr == _Myt::__single__.get())
        {

            if (nullptr == _Myt::__single__.get())
            {
                _Myt::__single__.reset(new(std::nothrow) _Ty());
                if (_Myt::__single__ != nullptr)
                    _Myt::delay_init(args...);
            }
        }
        return _Myt::__single__.get();
    }

    static void destroy(void)
    {
        if (_Myt::__single__.get() != nullptr)
        {
            _Myt::__single__.reset();
        }
    }

private:

    template<typename _Fty, typename..._Args>
    static void delay_init(const _Fty& memf, _Args...args)
    { // init use specific member func with more than 1 args
        std::mem_fn(memf)(_Myt::__single__.get(), args...);
    }

    template<typename _Fty, typename _Arg>
    static void delay_init(const _Fty& memf, const _Arg& arg)
    { // init use specific member func with 1 arg
        std::mem_fun(memf)(_Myt::__single__.get(), arg);
    }

    template<typename _Fty>
    static void delay_init(const _Fty& memf)
    { // init use specific member func without arg
        std::mem_fun(memf)(_Myt::__single__.get());
    }

    static void delay_init(void)
    { // if no member func specificed, use preferred func 'init'
        _Myt::__single__->init();
    }

private:
    static std::unique_ptr<_Ty> __single__;
private:
    PSS_singleton(void) = delete; // just disable construct, assign operation, copy construct also not allowed.
};

template<typename _Ty>
std::unique_ptr<_Ty> PSS_singleton<_Ty, false>::__single__;

template<typename _Ty>
std::unique_ptr<_Ty> PSS_singleton<_Ty, true>::__single__;

// TEMPLATE alias
template<typename _Ty>
using delayed = PSS_singleton < _Ty, true >;

#endif


