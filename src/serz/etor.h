/**
 * Provides conversion of exception handling into monadic return value.
 * @author Chen Weiguang
 * @version 0.1.0
 */

#pragma once

#include "traits.h"

#include "rustfp/result.h"

#include <exception>
#include <string>
#include <type_traits>
#include <utility>

namespace serz {
    // declaration section

    /**
     * Converts base exception into string.
     * Meant for overloading to allow derived exception classes
     * to customize their conversion to string.
     */
    auto exception_to_str(const std::exception &e) -> std::string;

    /**
     * Wrapping structure to facilitate run and mix exception
     * handling. Recursively wraps try-catch block until the base etor case.
     */
    template <class Exception = std::exception, class... Exceptions>
    struct etor
    {
        /**
         * Executes the given closure with try-catch wrapping.
         * May copy or move the result into Ok, and any caught exceptions
         * will be shifted into Err.
         */
        template <class Fn>
        static auto run(Fn &&fn) ->
            ::rustfp::Result<special_decay_t<std::result_of_t<Fn()>>, std::string>;

        /**
         * Same as run, except that it expects the closure 
         * to return Result<T, string> to allow the user to mix any intended error
         * value together with the exception error string.
         */
        template <class ResFn>
        static auto mix(ResFn &&res_fn) -> std::result_of_t<ResFn()>;

        /**
         * Same as previous overload, except that it performs
         * a map of string into some other value of possibly different type,
         * before mixing with the intended error value.
         */
        template <class ResFn, class MapFn>
        static auto mix(ResFn &&res_fn, const MapFn &map_fn) -> std::result_of_t<ResFn()>;
    };

    /**
     * Wrapping structure to facilitate run and mix exception
     * handling. This is the base etor case, which creates the inner most try-catch block. 
     */
    template <class Exception>
    struct etor<Exception> {
        /**
         * Executes the given closure with try-catch wrapping.
         * May copy or move the result into Ok, and any caught exceptions
         * will be shifted into Err.
         */
        template <class Fn>
        static auto run(Fn &&fn) ->
            ::rustfp::Result<special_decay_t<std::result_of_t<Fn()>>, std::string>;

        /**
         * Same as run, except that it expects the closure 
         * to return Result<T, string> to allow the user to mix any intended error
         * value together with the exception error string.
         */ 
        template <class ResFn>
        static auto mix(ResFn &&res_fn) -> std::result_of_t<ResFn()>;

        /**
         * Same as previous overload, except that it performs
         * a map of string into some other value of possibly different type,
         * before mixing with the intended error value.
         */
        template <class ResFn, class MapFn>
        static auto mix(ResFn &&res_fn, const MapFn &map_fn) -> std::result_of_t<ResFn()>;
    };

    // implementation section

    inline auto exception_to_str(const std::exception &e) -> std::string {
        return e.what();
    }

    template <class Exception, class... Exceptions>
    template <class Fn>
    auto etor<Exception, Exceptions...>::run(Fn &&fn) ->
        ::rustfp::Result<special_decay_t<std::result_of_t<Fn()>>, std::string> {

        // Fp/Result.h
        using ::rustfp::Err;
        using ::rustfp::Ok;

        // string
        using std::string;

        // type_traits
        using std::result_of;

        // utility
        using std::forward;

        try {
            return etor<Exceptions...>::run(forward<Fn>(fn));
        } catch (const Exception &e) {
            return Err(exception_to_str(e));
        }
    }

    template <class Exception, class... Exceptions>
    template <class ResFn>
    auto etor<Exception, Exceptions...>::mix(ResFn &&res_fn) -> std::result_of_t<ResFn()> {

        // Fp/Result.h
        using ::rustfp::Err;
        using ::rustfp::Ok;

        // type_traits
        using std::result_of;

        // utility
        using std::forward;

        try {
            return etor<Exceptions...>::mix(forward<ResFn>(res_fn));
        } catch (const Exception &e) {
            return Err(exception_to_str(e));
        }
    }

    template <class Exception, class... Exceptions>
    template <class ResFn, class MapFn>
    auto etor<Exception, Exceptions...>::mix(ResFn &&res_fn, const MapFn &map_fn) -> std::result_of_t<ResFn()> {
        // Fp/Result.h
        using ::rustfp::Err;
        using ::rustfp::Ok;

        // type_traits
        using std::result_of;

        // utility
        using std::forward;

        try {
            return etor<Exceptions...>::mix(forward<ResFn>(res_fn), map_fn);
        } catch (const Exception &e) {
            return Err(map_fn(exception_to_str(e)));
        }
    }

    template <class Exception>
    template <class Fn>
    auto etor<Exception>::run(Fn &&fn) ->
        ::rustfp::Result<special_decay_t<std::result_of_t<Fn()>>, std::string>
    {
        // Fp/Result.h
        using ::rustfp::Err;
        using ::rustfp::Ok;

        // string
        using std::string;

        // type_traits
        using std::result_of;

        // utility
        using std::forward;

        try {
            return Ok(UnitResFwd(forward<Fn>(fn)));
        } catch (const Exception &e) {
            return Err(exception_to_str(e));
        }
    }

    template <class Exception>
    template <class ResFn>
    auto etor<Exception>::mix(ResFn &&res_fn) -> std::result_of_t<ResFn()>
    {
        // Fp/Result.h
        using ::rustfp::Err;

        // type_traits
        using std::result_of;

        // utility
        using std::forward;

        try {
            return res_fn();
        } catch (const Exception &e) {
            return Err(exception_to_str(e));
        }
    }

    template <class Exception>
    template <class ResFn, class MapFn>
    auto etor<Exception>::mix(ResFn &&res_fn, const MapFn &map_fn) -> std::result_of_t<ResFn()>
    {
        // Fp/Result.h
        using ::rustfp::Err;

        // type_traits
        using std::result_of;

        // utility
        using std::forward;

        try {
            return res_fn();
        } catch (const Exception &e) {
            return Err(map_fn(exception_to_str(e)));
        }
    }
}
