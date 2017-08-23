/**
 * Provides a framework of parsing and serialization of intermediate DOM representations.
 * @author Chen Weiguang
 * @version 0.1.0
 */

#pragma once

#include "val.h"
#include "traits.h"

#include "from_str.h"

#include "rustfp/option.h"
#include "rustfp/result.h"
#include "rustfp/unit.h"

#ifndef FMT_HEADER_ONLY
#define FMT_HEADER_ONLY
#endif
#include "fmt/format.h"

#include <cstdint>
#include <functional>
#include <limits>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

namespace serz {
    // declaration section

    namespace details {
        template <class Ser>
        class parse_nvp_action {
        public:
            parse_nvp_action(Ser &ser, const std::string &name);

            auto operator()(::rustfp::Result<const dom_obj &, std::string> &&obj_res) ->
                ::rustfp::Result<const dom_obj &, std::string>;

        private:
            std::reference_wrapper<Ser> ser;
            std::string name;
        };

#ifndef SERZ_DISALLOW_MISSING_ARRAY_OBJECT

        template <class Ser>
        class parse_nvp_action<std::vector<Ser>> {
        public:
            parse_nvp_action(std::vector<Ser> &ser, const std::string &name);

            auto operator()(::rustfp::Result<const dom_obj &, std::string> &&obj_res) ->
                ::rustfp::Result<const dom_obj &, std::string>;

        private:
            std::reference_wrapper<std::vector<Ser>> ser;
            std::string name;
        };

        template <class Ser>
        class parse_nvp_action<std::unordered_map<std::string, Ser>> {
        public:
            parse_nvp_action(std::unordered_map<std::string, Ser> &ser, const std::string &name);

            auto operator()(::rustfp::Result<const dom_obj &, std::string> &&obj_res) ->
                ::rustfp::Result<const dom_obj &, std::string>;

        private:
            std::reference_wrapper<std::unordered_map<std::string, Ser>> ser;
            std::string name;
        };

#endif

        template <class Ser>
        class parse_nvp_action<::rustfp::Option<Ser>> {
        public:
            parse_nvp_action(::rustfp::Option<Ser> &ser, const std::string &name);

            auto operator()(::rustfp::Result<const dom_obj &, std::string> &&obj_res) ->
                ::rustfp::Result<const dom_obj &, std::string>;

        private:
            std::reference_wrapper<::rustfp::Option<Ser>> ser;
            std::string name;
        };

        template <class Ser>
        class serialize_nvp_action {
        public:
            serialize_nvp_action(
                const Ser &ser,
                const std::string &name,
                const bool is_attr = false);

            auto operator()(dom_obj &obj) -> dom_obj &;

        private:
            std::reference_wrapper<const Ser> ser;
            std::string name;
            bool is_attr;
        };
    
        template <class Ser>
        class serialize_nvp_action<::rustfp::Option<Ser>> {
        public:
            serialize_nvp_action(
                const ::rustfp::Option<Ser> &ser,
                const std::string &name,
                const bool is_attr = false);

            auto operator()(dom_obj &obj) -> dom_obj &;

        private:
            std::reference_wrapper<const ::rustfp::Option<Ser>> ser;
            std::string name;
            bool is_attr;
        };

        template <class Ser>
        class done_obj_action {
        public:
            done_obj_action(Ser &ser);

            auto operator()(::rustfp::Result<const dom_obj &, std::string> &&obj_res) ->
                ::rustfp::Result<Ser &, std::string>;

        private:
            std::reference_wrapper<Ser> ser;
        };

        template <class Num, class DomType>
        auto is_valid_conversion(const DomType inner_val) -> bool;

        template <>
        auto is_valid_conversion<uint64_t, dom_int>(const dom_int inner_val) -> bool;

        template <class DomType, class Num>
        auto parse_value_number_impl(Num &ser, const dom_val &val) ->
            ::rustfp::Result<Num &, std::string>;

        template <class Int>
        auto parse_value_int_impl(Int &ser, const dom_val &val) ->
            ::rustfp::Result<Int &, std::string>;

        template <class Flt>
        auto parse_value_flt_impl(Flt &ser, const dom_val &val) ->
            ::rustfp::Result<Flt &, std::string>;

        template <class Ser, bool = std::is_enum<Ser>::value>
        struct parse_value_enum_impl;

        template <class Ser>
        struct parse_value_enum_impl<Ser, false> {
            static auto exec(Ser &ser, const dom_val &val) ->
                ::rustfp::Result<Ser &, std::string>;
        };

        template <class Enum>
        struct parse_value_enum_impl<Enum, true> {
            static auto exec(Enum &ser, const dom_val &val) ->
                ::rustfp::Result<Enum &, std::string>;
        };

        template <class Ser, bool = std::is_enum<Ser>::value>
        struct serialize_value_enum_impl;

        template <class Ser>
        struct serialize_value_enum_impl<Ser, false> {
            static auto exec(const Ser &ser, dom_val &val) -> dom_val &;
        };

        template <class Enum>
        struct serialize_value_enum_impl<Enum, true> {
            static auto exec(const Enum &ser, dom_val &val) -> dom_val &;
        };
    }

    /**
     * For printing the typename in error messages. Generic case.
     */
    template <class Ser>
    struct parse_type_name { static auto get() -> std::string; };

    /**
     * For printing the typename in error messages.
     * Specialized for rustfp::unit_t.
     */
    template <>
    struct parse_type_name<::rustfp::unit_t> { static auto get() -> std::string; };

    /**
     * For printing the typename in error messages.
     * Specialized for bool.
     */
    template <>
    struct parse_type_name<bool> { static auto get() -> std::string; };

    /**
     * For printing the typename in error messages.
     * Specialized for int8_t.
     */
    template <>
    struct parse_type_name<int8_t> { static auto get() -> std::string; };

    /**
     * For printing the typename in error messages.
     * Specialized for int16_t.
     */
    template <>
    struct parse_type_name<int16_t> { static auto get() -> std::string; };

    /**
     * For printing the typename in error messages.
     * Specialized for int32_t.
     */
    template <>
    struct parse_type_name<int32_t> { static auto get() -> std::string; };

    /**
     * For printing the typename in error messages.
     * Specialized for int64_t.
     */
    template <>
    struct parse_type_name<int64_t> { static auto get() -> std::string; };

    /**
     * For printing the typename in error messages.
     * Specialized for uint8_t.
     */
    template <>
    struct parse_type_name<uint8_t> { static auto get() -> std::string; };

    /**
     * For printing the typename in error messages.
     * Specialized for uint16_t.
     */
    template <>
    struct parse_type_name<uint16_t> { static auto get() -> std::string; };

    /**
     * For printing the typename in error messages.
     * Specialized for uint32_t.
     */
    template <>
    struct parse_type_name<uint32_t> { static auto get() -> std::string; };

    /**
     * For printing the typename in error messages.
     * Specialized for uint64_t.
     */
    template <>
    struct parse_type_name<uint64_t> { static auto get() -> std::string; };

    /**
     * For printing the typename in error messages.
     * Specialized for float.
     */
    template <>
    struct parse_type_name<float> { static auto get() -> std::string; };

    /**
     * For printing the typename in error messages.
     * Specialized for double.
     */
    template <>
    struct parse_type_name<double> { static auto get() -> std::string; };

    /**
     * For printing the typename in error messages.
     * Specialized for string.
     */
    template <>
    struct parse_type_name<std::string> { static auto get() -> std::string; };

    /**
     * For printing the typename in error messages.
     * Specialized for vector.
     */
    template <class Ser>
    struct parse_type_name<std::vector<Ser>> { static auto get() -> std::string; };

    /**
     * For printing the typename in error messages.
     * Specialized for unordered_map.
     */
    template <class Ser>
    struct parse_type_name<std::unordered_map<std::string, Ser>> {
        static auto get() -> std::string;
    };
    
    /**
     * Infix convenience to link up multiple parse_nvp actions.
     */
    template <class Ser>
    auto operator&(
        ::rustfp::Result<const dom_obj &, std::string> &&obj_res,
        details::parse_nvp_action<Ser> &&action) ->
        ::rustfp::Result<const dom_obj &, std::string>;

    /**
     * Infix convenience to link up the last parse_nvp to done_obj action.
     */
    template <class Ser>
    auto operator&(
        ::rustfp::Result<const dom_obj &, std::string> &&obj_res,
        details::done_obj_action<Ser> &&action) ->
        ::rustfp::Result<Ser &, std::string>;

    /**
     * Infix convenience to link up multiple serialize_nvp actions.
     */
    template <class Ser>
    auto operator&(dom_obj &obj, details::serialize_nvp_action<Ser> &&action) ->
        dom_obj &;

    /**
     * Provides starting convenience to monadically get dom_obj out of dom_val,
     * allowing the result to chain with parse_nvp and end with done_obj.
     */
    auto as_obj(const dom_val &val) -> ::rustfp::Result<const dom_obj &, std::string>;

    /**
     * Provides ending convenience to end the parsing chain of parse_nvp
     * and returns the result in the correct form.
     */
    template <class Ser>
    auto done_obj(Ser &ser) -> details::done_obj_action<Ser>;

    /**
     * Provides starting convenience to create dom_obj out of the given dom_val
     * for serialization chaining purposes.
     */
    auto create_obj(dom_val &val) -> dom_obj &;

    /**
     * Creates an action to DOM serialization for parsing DOM value
     * in DOM object.
     */
    template <class Ser>
    auto parse_nvp(Ser &ser, const std::string &name) ->
        details::parse_nvp_action<Ser>;

    /**
     * Provides the base case of implementation of parsing
     * of non-parsable values, allowing the compiler to emit an
     * error message. This function cannot be used.
     */
    template <class Ser>
    auto parse_value(Ser &ser, const dom_val &val) ->
        ::rustfp::Result<Ser &, std::string>;

    /**
     * Provides parsing implementation for bool.
     */
    auto parse_value(::rustfp::unit_t &ser, const dom_val &val) ->
        ::rustfp::Result<::rustfp::unit_t &, std::string>;

    /**
     * Provides parsing implementation for bool.
     */
    auto parse_value(bool &ser, const dom_val &val) ->
        ::rustfp::Result<bool &, std::string>;

    /**
     * Provides parsing implementation for int8_t.
     */
    auto parse_value(int8_t &ser, const dom_val &val) ->
        ::rustfp::Result<int8_t &, std::string>;

    /**
     * Provides parsing implementation for int16_t.
     */
    auto parse_value(int16_t &ser, const dom_val &val) ->
        ::rustfp::Result<int16_t &, std::string>;

    /**
     * Provides parsing implementation for int32_t.
     */
    auto parse_value(int32_t &ser, const dom_val &val) ->
        ::rustfp::Result<int32_t &, std::string>;

    /**
     * Provides parsing implementation for int64_t.
     */
    auto parse_value(int64_t &ser, const dom_val &val) ->
        ::rustfp::Result<int64_t &, std::string>;

    /**
     * Provides parsing implementation for uint8_t.
     */
    auto parse_value(uint8_t &ser, const dom_val &val) ->
        ::rustfp::Result<uint8_t &, std::string>;

    /**
     * Provides parsing implementation for uint16_t.
     */
    auto parse_value(uint16_t &ser, const dom_val &val) ->
        ::rustfp::Result<uint16_t &, std::string>;

    /**
     * Provides parsing implementation for uint32_t.
     */
    auto parse_value(uint32_t &ser, const dom_val &val) ->
        ::rustfp::Result<uint32_t &, std::string>;

    /**
     * Provides parsing implementation for uint64_t.
     */
    auto parse_value(uint64_t &ser, const dom_val &val) ->
        ::rustfp::Result<uint64_t &, std::string>;

    /**
     * Provides parsing implementation for float.
     */
    auto parse_value(float &ser, const dom_val &val) ->
        ::rustfp::Result<float &, std::string>;

    /**
     * Provides parsing implementation for double.
     */
    auto parse_value(double &ser, const dom_val &val) ->
        ::rustfp::Result<double &, std::string>;

    /**
     * Provides parsing implementation for std::string.
     */
    auto parse_value(std::string &ser, const dom_val &val) ->
        ::rustfp::Result<std::string &, std::string>;

    /**
     * Provides parsing implementation for dom_val;
     */
    auto parse_value(dom_val &ser, const dom_val &val) ->
        ::rustfp::Result<dom_val &, std::string>;

    /**
     * Provides parsing implementation for std::vector<Ser>,
     * where Ser must itself be parsable.
     */
    template <class Ser>
    auto parse_value(std::vector<Ser> &sers, const dom_val &val) ->
        ::rustfp::Result<std::vector<Ser> &, std::string>;

    /**
     * Provides parsing implementation for
     * std::unordered_map<std::string, Ser>, where Ser must itself be parsable.
     */
    template <class Ser>
    auto parse_value(std::unordered_map<std::string, Ser> &sers, const dom_val &val) ->
        ::rustfp::Result<std::unordered_map<std::string, Ser> &, std::string>;

    /**
     * Provides parsing implementation for
     * ::rustfp::Option<Ser>, where Ser must itself be parsable.
     */
    template <class Ser>
    auto parse_value(::rustfp::Option<Ser> &ser, const dom_val &val) ->
        ::rustfp::Result<::rustfp::Option<Ser> &, std::string>;

    /**
     * Creates an action to DOM serialization for serializing
     * DOM value in DOM object. Refrain from use for std::vector<Ser>
     * because this would cause the child node for XML serialization
     * to have blank node names. Otherwise it is okay for use for other
     * format types like JSON. Allows marking the DOM value as an XML
     * attribute also.
     */
    template <class Ser>
    auto serialize_nvp(
        const Ser &ser,
        const std::string &name,
        const bool is_attr = false) ->
        details::serialize_nvp_action<Ser>;

    /**
     * Provides the base case of implementation of serializing
     * of non-serializable values, allowing the compiler to emit an
     * error message. This function cannot be used.
     */
    template <class Ser>
    auto serialize_value(const Ser &ser, dom_val &val) -> dom_val &;

    /**
     * Provides serializing implementation for rustfp::unit_t.
     */
    auto serialize_value(const ::rustfp::unit_t &ser, dom_val &val) -> dom_val &;
    
    /**
     * Provides serializing implementation for bool.
     */
    auto serialize_value(const bool &ser, dom_val &val) -> dom_val &;
    
    /**
     * Provides serializing implementation for int8_t.
     */
    auto serialize_value(const int8_t &ser, dom_val &val) -> dom_val &;
    
    /**
     * Provides serializing implementation for int16_t.
     */
    auto serialize_value(const int16_t &ser, dom_val &val) -> dom_val &;
    
    /**
     * Provides serializing implementation for int32_t.
     */
    auto serialize_value(const int32_t &ser, dom_val &val) -> dom_val &;
    
    /**
     * Provides serializing implementation for int64_t.
     */
    auto serialize_value(const int64_t &ser, dom_val &val) -> dom_val &;
    
    /**
     * Provides serializing implementation for uint8_t.
     */
    auto serialize_value(const uint8_t &ser, dom_val &val) -> dom_val &;
    
    /**
     * Provides serializing implementation for uint16_t.
     */
    auto serialize_value(const uint16_t &ser, dom_val &val) -> dom_val &;
    
    /**
     * Provides serializing implementation for uint32_t.
     */
    auto serialize_value(const uint32_t &ser, dom_val &val) -> dom_val &;
    
    /**
     * Provides serializing implementation for uint64_t.
     */
    auto serialize_value(const uint64_t &ser, dom_val &val) -> dom_val &;
    
    /**
     * Provides serializing implementation for float.
     */
    auto serialize_value(const float &ser, dom_val &val) -> dom_val &;
    
    /**
     * Provides serializing implementation for double.
     */
    auto serialize_value(const double &ser, dom_val &val) -> dom_val &;
    
    /**
     * Provides serializing implementation for std::string.
     */
    auto serialize_value(const std::string &ser, dom_val &val) -> dom_val &;

    /**
     * Provides serializing implementation for dom_val.
     */
    auto serialize_value(const dom_val &ser, dom_val &val) -> dom_val &;

    /**
     * Provides serializing implementation for std::vector<Ser>,
     * where Ser must itself be serializable.
     */
    template <class Ser>
    auto serialize_value(const std::vector<Ser> &sers, dom_val &val) -> dom_val &;

    /**
     * Provides serializing implementation for
     * std::unordered_map<std::string, Ser>, where Ser must itself be serializable.
     */
    template <class Ser>
    auto serialize_value(
        const std::unordered_map<std::string, Ser> &sers, dom_val &val) -> dom_val &;

    /**
     * Provides serializing implementation for ::rustfp::Option<Ser>,
     * where Ser must itself be serializable.
     */
    template <class Ser>
    auto serialize_value(const ::rustfp::Option<Ser> &ser, dom_val &val) -> dom_val &;

    // implementation section

    namespace details {
        template <class Ser>
        parse_nvp_action<Ser>::parse_nvp_action(Ser &ser, const std::string &name) :
            ser(ser),
            name(name) {

        }

        template <class Ser>
        auto parse_nvp_action<Ser>::operator()(
            ::rustfp::Result<const dom_obj &, std::string> &&obj_res) ->
            ::rustfp::Result<const dom_obj &, std::string> {

            return std::move(obj_res).and_then([this](const dom_obj &obj) {
                const auto it = obj.find(name);

                return (it != obj.cend())
                    ? parse_value(ser.get(), it->second)
                        .map([&obj](Ser &) { return std::cref(obj); })

                    : ::rustfp::Err(
                        fmt::format("Unable to find key with name '{}' "
                            "while performing parse_nvp", name));
            });
        }

#ifndef SERZ_DISALLOW_MISSING_ARRAY_OBJECT

        template <class Ser>
        parse_nvp_action<std::vector<Ser>>::parse_nvp_action(
            std::vector<Ser> &ser,
            const std::string &name) :

            ser(ser),
            name(name) {

        }

        template <class Ser>
        auto parse_nvp_action<std::vector<Ser>>::operator()(
            ::rustfp::Result<const dom_obj &, std::string> &&obj_res) ->
            ::rustfp::Result<const dom_obj &, std::string> {

            return std::move(obj_res).and_then([this](const dom_obj &obj) {
                // alter the behaviour here to not necessary to find the name
                const auto it = obj.find(name);

                return (it != obj.cend())
                    ? parse_value(ser.get(), it->second)
                        .map([&obj](std::vector<Ser> &) { return std::ref(obj); })

                    : [this, &obj] {
                        ser.get().clear();
                        return ::rustfp::Ok(std::cref(obj));
                    }();
            });
        }

        template <class Ser>
        parse_nvp_action<std::unordered_map<std::string, Ser>>::parse_nvp_action(
            std::unordered_map<std::string, Ser> &ser,
            const std::string &name) :

            ser(ser),
            name(name) {

        }

        template <class Ser>
        auto parse_nvp_action<std::unordered_map<std::string, Ser>>::operator()(
            ::rustfp::Result<const dom_obj &, std::string> &&obj_res) ->
            ::rustfp::Result<const dom_obj &, std::string> {

            return std::move(obj_res).and_then([this](const dom_obj &obj) {
                // alter the behaviour here to not necessary to find the name
                const auto it = obj.find(name);

                return (it != obj.cend())
                    ? parse_value(ser.get(), it->second)
                        .map([&obj](std::unordered_map<std::string, Ser> &) { return std::cref(obj); })

                    : [this, &obj] {
                        ser.get().clear();
                        return ::rustfp::Ok(std::cref(obj));
                    }();
            });
        }

#endif

        template <class Ser>
        parse_nvp_action<::rustfp::Option<Ser>>::parse_nvp_action(
            ::rustfp::Option<Ser> &ser,
            const std::string &name) :

            ser(ser),
            name(name) {

        }

        template <class Ser>
        auto parse_nvp_action<::rustfp::Option<Ser>>::operator()(
            ::rustfp::Result<const dom_obj &, std::string> &&obj_res) ->            
            ::rustfp::Result<const dom_obj &, std::string> {

            return std::move(obj_res).and_then([this](const dom_obj &obj) {
                // alter the behaviour here to not necessary to find the name
                const auto it = obj.find(name);

                return (it != obj.cend())
                    ? parse_value(ser.get(), it->second)
                        .map([&obj](::rustfp::Option<Ser> &) { return std::ref(obj); })

                    : [this, &obj] {
                        ser.get() = ::rustfp::None;
                        return ::rustfp::Ok(std::ref(obj));
                    }();
            });
        }

        template <class Ser>
        serialize_nvp_action<Ser>::serialize_nvp_action(
            const Ser &ser,
            const std::string &name,
            const bool is_attr) :

            ser(ser),
            name(name),
            is_attr(is_attr) {

        }

        template <class Ser>
        auto serialize_nvp_action<Ser>::operator()(dom_obj &obj) -> dom_obj & {
            // object will always have repetition in the names
            dom_val child_val(name);
            child_val.set_attribute(is_attr);

            serialize_value(ser.get(), child_val);
            obj.emplace(name, std::move(child_val));

            return obj;
        }

        template <class Ser>
        serialize_nvp_action<::rustfp::Option<Ser>>::serialize_nvp_action(
            const ::rustfp::Option<Ser> &ser,
            const std::string &name,
            const bool is_attr) :

            ser(ser),
            name(name),
            is_attr(is_attr) {
            
        }

        template <class Ser>
        auto serialize_nvp_action<::rustfp::Option<Ser>>::operator()(dom_obj &obj) -> dom_obj & {
            // if optional value has value, then use the original action
            // to perform the serialization, otherwise do nothing
            return ser.get().is_some()
                ? serialize_nvp_action<Ser>(ser.get().get_unchecked(), name, is_attr)(obj)
                : obj;
        }

        template <class Ser>
        done_obj_action<Ser>::done_obj_action(Ser &ser) :
            ser(ser) {

        }

        template <class Ser>
        auto done_obj_action<Ser>::operator()(
            ::rustfp::Result<const dom_obj &, std::string> &&obj_res) ->
            ::rustfp::Result<Ser &, std::string> {

            return std::move(obj_res).map([this](const dom_obj &) { return std::ref(ser.get()); });
        }

        template <class Num, class DomType>
        auto is_valid_conversion(const DomType inner_val) -> bool {
            // allow safe implicit conversion for comparison
            // between sign and unsigned types since dom_int is
            // the maximum bytes of signed integral type
            return
                inner_val >= std::numeric_limits<Num>::lowest() &&
                inner_val <= std::numeric_limits<Num>::max();
        }

        template <>
        inline auto is_valid_conversion<uint64_t, dom_int>(const dom_int inner_val) -> bool {
            // special case of comparing between int64_t and uint64_t
            // simply check if source value is greater or equal to 0
            // and if so, it must be within the bounds of uint64_t
            return inner_val >= 0;
        }

        template <class DomType, class Num>
        auto parse_value_number_impl(Num &ser, const dom_val &val) ->
            ::rustfp::Result<Num &, std::string> {

            return val.get<DomType>()
                // try the direct integer/float type first
                .and_then([&ser](const DomType &inner_val) {
                    return is_valid_conversion<Num>(inner_val)
                        ? [&ser, inner_val] {
                            // within valid bounds
                            ser = static_cast<Num>(inner_val);
                            return ::rustfp::Some(std::ref(ser));
                        }()

                        : ::rustfp::None;
                })
                // if fail, then try if the string can be coerced into integer/float
                .or_else([&ser, &val] {
                    return val.get<dom_str>()
                        .and_then([&ser](const dom_str &inner_val) -> ::rustfp::Option<Num &> {
                            return ::from_str::parse<Num>(inner_val)
                                .map([&ser](const Num parsed_val) {
                                    ser = parsed_val;
                                    return std::ref(ser);
                                });
                        });
                })
                .ok_or_else([] {
                    return fmt::format("Unable to parse into value of type '{}'",
                        parse_type_name<Num>::get());
                });
        }

        template <class Int>
        auto parse_value_int_impl(Int &ser, const dom_val &val) ->
            ::rustfp::Result<Int &, std::string> {

            return parse_value_number_impl<dom_int>(ser, val);
        }

        template <class Flt>
        auto parse_value_flt_impl(Flt &ser, const dom_val &val) ->
            ::rustfp::Result<Flt &, std::string> {

            return parse_value_number_impl<dom_flt>(ser, val);
        }

        template <class Ser>
        auto parse_value_enum_impl<Ser, false>::exec(Ser &, const dom_val &) ->
            ::rustfp::Result<Ser &, std::string> {

            static_assert(sizeof(Ser) < 0,
                "parse_value must be defined for every custom type");

            return ::rustfp::Err(std::string());
        }

        template <class Enum>
        auto parse_value_enum_impl<Enum, true>::exec(Enum &ser, const dom_val &val) ->
            ::rustfp::Result<Enum &, std::string> {

            return val.get<dom_int>()
                .map([&ser](const dom_int itg) {
                    ser = static_cast<Enum>(itg);
                    return std::ref(ser);
                }) 
                .ok_or_else([] {
                    return std::string(
                        "Unable to get dom_int from dom_val to parse enum");
                });
        }

        template <class Ser>
        auto serialize_value_enum_impl<Ser, false>::exec(const Ser &, dom_val &val) ->
            dom_val & {

            static_assert(sizeof(Ser) < 0,
                "serialize_value must be defined for every custom type");

            return val;
        }

        template <class Enum>
        auto serialize_value_enum_impl<Enum, true>::exec(const Enum &ser, dom_val &val) ->
            dom_val & {

            val = static_cast<dom_int>(ser);
            return val;
        }
    }
    
    template <class Ser>
    auto parse_type_name<Ser>::get() -> std::string { return "unknown"; }

    inline auto parse_type_name<::rustfp::unit_t>::get() -> std::string { return "unit_t"; }

    inline auto parse_type_name<bool>::get() -> std::string { return "bool"; }

    inline auto parse_type_name<int8_t>::get() -> std::string { return "i8"; }

    inline auto parse_type_name<int16_t>::get() -> std::string { return "i16"; }

    inline auto parse_type_name<int32_t>::get() -> std::string { return "i32"; }

    inline auto parse_type_name<int64_t>::get() -> std::string { return "i64"; }

    inline auto parse_type_name<uint8_t>::get() -> std::string { return "u8"; }

    inline auto parse_type_name<uint16_t>::get() -> std::string { return "u16"; }

    inline auto parse_type_name<uint32_t>::get() -> std::string { return "u32"; }

    inline auto parse_type_name<uint64_t>::get() -> std::string { return "u64"; }

    inline auto parse_type_name<float>::get() -> std::string { return "f32"; }

    inline auto parse_type_name<double>::get() -> std::string { return "f64"; }

    inline auto parse_type_name<std::string>::get() -> std::string { return "string"; }

    template <class Ser>
    auto parse_type_name<std::vector<Ser>>::get() -> std::string {
        return fmt::format("vector<{}>", parse_type_name<Ser>::get());
    }

    template <class Ser>
    auto parse_type_name<std::unordered_map<std::string, Ser>>::get() -> std::string {
        return fmt::format("unordered_map<string, {}>", parse_type_name<Ser>::get());
    }

    template <class Ser>
    auto operator&(
        ::rustfp::Result<const dom_obj &, std::string> &&obj_res,
        details::parse_nvp_action<Ser> &&action) ->
        ::rustfp::Result<const dom_obj &, std::string> {

        return action(std::move(obj_res));
    }

    template <class Ser>
    auto operator&(
        ::rustfp::Result<const dom_obj &, std::string> &&obj_res,
        details::done_obj_action<Ser> &&action) -> ::rustfp::Result<Ser &, std::string> {

        return action(std::move(obj_res));
    }

    template <class Ser>
    auto operator&(dom_obj &obj, details::serialize_nvp_action<Ser> &&action) ->
        dom_obj & {

        return action(obj);
    }

    inline auto as_obj(const dom_val &val) -> ::rustfp::Result<const dom_obj &, std::string> {
        return val.get<dom_obj>()
            .ok_or_else([] {
                return std::string("Unable to interpret DOM value as DOM object");
            });
    }

    template <class Ser>
    auto done_obj(Ser &ser) -> details::done_obj_action<Ser> {
        return details::done_obj_action<Ser>(ser);
    }

    inline auto create_obj(dom_val &val) -> dom_obj & {
        if (!val.is<dom_obj>()) {
            val = dom_obj();
        }

        return val.get_unchecked<dom_obj>();
    }

    template <class Ser>
    auto parse_nvp(Ser &ser, const std::string &name) -> details::parse_nvp_action<Ser> {
        return details::parse_nvp_action<Ser>(ser, name);
    }

    template <class Ser>
    auto parse_value(Ser &ser, const dom_val &val) -> ::rustfp::Result<Ser &, std::string> {
        return details::parse_value_enum_impl<Ser>::exec(ser, val);
    }

    inline auto parse_value(::rustfp::unit_t &ser, const dom_val &) -> ::rustfp::Result<::rustfp::unit_t &, std::string> {
        return ::rustfp::Ok(std::ref(ser));
    }

    inline auto parse_value(bool &ser, const dom_val &val) -> ::rustfp::Result<bool &, std::string> {
        return val.get<dom_bln>()
            .map([&ser](const dom_bln bln) {
                ser = bln;
                return std::ref(ser);
            })
            .ok_or_else([] {
                return std::string("Unable to get boolean or string from given "
                    "DOM value for parsing of bool");
            })

            .or_else([&ser, &val](const std::string &err_msg) {
                return val.get<dom_str>()
                    .ok_or_else([&err_msg] { return err_msg; })
                    .and_then([&ser](const dom_str &str) -> ::rustfp::Result<bool &, std::string> {
                        if (str == "true") {
                            ser = true;
                            return ::rustfp::Ok(std::ref(ser));
                        } else if (str == "false") {
                            ser = false;
                            return ::rustfp::Ok(std::ref(ser));
                        }

                        return ::rustfp::Err(
                            fmt::format("Unable to convert '{}' to bool"));
                    });
            });
    }

    inline auto parse_value(int8_t &ser, const dom_val &val) ->
        ::rustfp::Result<int8_t &, std::string> {

        return details::parse_value_int_impl(ser, val);
    }

    inline auto parse_value(int16_t &ser, const dom_val &val) ->
        ::rustfp::Result<int16_t &, std::string> {

        return details::parse_value_int_impl(ser, val);
    }

    inline auto parse_value(int32_t &ser, const dom_val &val) ->
        ::rustfp::Result<int32_t &, std::string> {

        return details::parse_value_int_impl(ser, val);
    }

    inline auto parse_value(int64_t &ser, const dom_val &val) ->
        ::rustfp::Result<int64_t &, std::string> {

        return details::parse_value_int_impl(ser, val);
    }

    inline auto parse_value(uint8_t &ser, const dom_val &val) ->
        ::rustfp::Result<uint8_t &, std::string> {

        return details::parse_value_int_impl(ser, val);
    }

    inline auto parse_value(uint16_t &ser, const dom_val &val) ->
        ::rustfp::Result<uint16_t &, std::string> {

        return details::parse_value_int_impl(ser, val);
    }

    inline auto parse_value(uint32_t &ser, const dom_val &val) ->
        ::rustfp::Result<uint32_t &, std::string> {

        return details::parse_value_int_impl(ser, val);
    }

    inline auto parse_value(uint64_t &ser, const dom_val &val) ->
        ::rustfp::Result<uint64_t &, std::string> {

        return details::parse_value_int_impl(ser, val);
    }

    inline auto parse_value(float &ser, const dom_val &val) ->
        ::rustfp::Result<float &, std::string> {

        return details::parse_value_flt_impl(ser, val);
    }

    inline auto parse_value(double &ser, const dom_val &val) ->
        ::rustfp::Result<double &, std::string> {

        return details::parse_value_flt_impl(ser, val);
    }

    inline auto parse_value(std::string &ser, const dom_val &val) ->
        ::rustfp::Result<std::string &, std::string> {

        return val.get<dom_str>()
            // attach the error message first
            .ok_or_else([] {
                return std::string("Unable to interpret the DOM value as string");
            })

            // if it is dom_str, just simply assign the value over
            .map([&ser](const dom_str &str) {
                ser = str;
                return std::ref(ser);
            })

            // otherwise try DomNullStringObject / dom_null variant,
            // accepting it as an empty string
            .or_else([&ser, &val](const std::string &err_msg) {
                return val.is<dom_null_str_obj>() || val.is<dom_null>()
                    ? [&ser]() -> ::rustfp::Result<std::string &, std::string> {
                        ser = "";
                        return ::rustfp::Ok(std::ref(ser));
                    }()

                    : ::rustfp::Err(err_msg);
            });
    }

    inline auto parse_value(dom_val &ser, const dom_val &val) ->
        ::rustfp::Result<dom_val &, std::string> {

        ser = val;
        return ::rustfp::Ok(std::ref(ser));
    }

    template <class Ser>
    auto parse_value(std::vector<Ser> &sers, const dom_val &val) ->
        ::rustfp::Result<std::vector<Ser> &, std::string> {

        return val.get<dom_arr>()
            // attach the error message first
            .ok_or_else([] {
                return std::string("Unable to interpret the DOM value as array");
            })

            // try to process the value as dom_arr
            .and_then([&sers](const dom_arr &arr) {
                auto res = ::rustfp::Result<::rustfp::unit_t, std::string>(::rustfp::Ok(::rustfp::Unit));

                for (const auto &arr_val : arr) {
                    res = std::move(res).and_then([&arr_val, &sers](auto) {
                        Ser ser;

                        return parse_value(ser, arr_val)
                            .map([&ser, &sers](const Ser &) {
                                sers.push_back(std::move(ser));
                                return ::rustfp::Unit;
                            });
                    });
                }

                return std::move(res).map([&sers](auto) { return std::ref(sers); });
            })

            // otherwise try dom_null, accepting it as an empty vector
            .or_else([&sers, &val](const std::string &err_msg) {
                return val.is<dom_null>()
                    ? [&sers]() -> ::rustfp::Result<std::vector<Ser> &, std::string> {
                        sers.clear();
                        return ::rustfp::Ok(std::ref(sers));
                    }()

                    : ::rustfp::Err(err_msg);
            })
            
            // otherwise simply accept as a single value vector
            .or_else([&sers, &val](const std::string &) {
                sers.clear();
                Ser ser;

                return parse_value(ser, val)
                    .map([&ser, &sers](const Ser &) {
                        sers.push_back(std::move(ser));
                        return std::ref(sers);
                    });
            });
    }

    template <class Ser>
    auto parse_value(std::unordered_map<std::string, Ser> &sers, const dom_val &val) ->
        ::rustfp::Result<std::unordered_map<std::string, Ser> &, std::string> {

        return val.get<dom_obj>()
            // attach the error message first
            .ok_or_else([] {
                return std::string("Unable to interpret the DOM value as object");
            })

            // try to process the vlaue as dom_obj
            .and_then([&sers](const dom_obj &obj) {
                auto res = ::rustfp::Result<::rustfp::unit_t, std::string>(::rustfp::Ok(::rustfp::Unit));

                for (const auto &obj_val : obj) {
                    res = std::move(res).and_then([&obj_val, &sers](auto) {
                        Ser ser;

                        return parse_value(ser, obj_val.second).map([&obj_val, &sers](Ser &ser) {
                            sers.emplace(obj_val.first, std::move(ser));
                            return ::rustfp::Unit;
                        });
                    });
                }

                return std::move(res).map([&sers](auto) { return std::ref(sers); });
            })

            // otherwise try DomNullStringObject / dom_null,
            // accepting it as an empty unordered_map
            .or_else([&sers, &val](const std::string &err_msg) {
                return val.is<dom_null_str_obj>() || val.is<dom_null>()
                    ? [&sers]() -> ::rustfp::Result<std::unordered_map<std::string, Ser> &, std::string> {
                        sers.clear();
                        return ::rustfp::Ok(std::ref(sers));
                    }()

                    : ::rustfp::Err(err_msg);
            });
    }

    template <class Ser>
    auto parse_value(::rustfp::Option<Ser> &ser, const dom_val &val) ->
        ::rustfp::Result<::rustfp::Option<Ser> &, std::string> {

        // since parse_nvp_action would have already found the dom_val
        // Option value here will definitely have a valid value
        // unless of parsing error of Ser

        Ser ser_inner;

        return parse_value(ser_inner, val).map([&ser, &ser_inner](const Ser &) {
            ser = ::rustfp::Some(std::move(ser_inner));
            return std::ref(ser);
        });
    }

    template <class Ser>
    auto serialize_nvp(const Ser &ser, const std::string &name, const bool is_attr) ->
        details::serialize_nvp_action<Ser> {

        return details::serialize_nvp_action<Ser>(ser, name, is_attr);
    }

    template <class Ser>
    auto serialize_value(const Ser &ser, dom_val &val) -> dom_val & {
        return details::serialize_value_enum_impl<Ser>::exec(ser, val);
    }

    inline auto serialize_value(const ::rustfp::unit_t &, dom_val &val) -> dom_val & {
        val = dom_null();
        return val;
    }

    inline auto serialize_value(const bool &ser, dom_val &val) -> dom_val & {
        val = static_cast<dom_bln>(ser);
        return val;
    }

    inline auto serialize_value(const int8_t &ser, dom_val &val) -> dom_val & {
        val = static_cast<dom_int>(ser);
        return val;
    }

    inline auto serialize_value(const int16_t &ser, dom_val &val) -> dom_val & {
        val = static_cast<dom_int>(ser);
        return val;
    }

    inline auto serialize_value(const int32_t &ser, dom_val &val) -> dom_val & {
        val = static_cast<dom_int>(ser);
        return val;
    }

    inline auto serialize_value(const int64_t &ser, dom_val &val) -> dom_val & {
        val = static_cast<dom_int>(ser);
        return val;
    }

    inline auto serialize_value(const uint8_t &ser, dom_val &val) -> dom_val & {
        val = static_cast<dom_int>(ser);
        return val;
    }

    inline auto serialize_value(const uint16_t &ser, dom_val &val) -> dom_val & {
        val = static_cast<dom_int>(ser);
        return val;
    }

    inline auto serialize_value(const uint32_t &ser, dom_val &val) -> dom_val & {
        val = static_cast<dom_int>(ser);
        return val;
    }

    inline auto serialize_value(const uint64_t &ser, dom_val &val) -> dom_val & {
        val = static_cast<dom_int>(ser);
        return val;
    }

    inline auto serialize_value(const float &ser, dom_val &val) -> dom_val & {
        val = static_cast<dom_flt>(ser);
        return val;
    }

    inline auto serialize_value(const double &ser, dom_val &val) -> dom_val & {
        val = static_cast<dom_flt>(ser);
        return val;
    }

    inline auto serialize_value(const std::string &ser, dom_val &val) -> dom_val & {
        val = ser;
        return val;
    }

    inline auto serialize_value(const dom_val &ser, dom_val &val) -> dom_val & {
        val = ser;
        return val;
    }

    template <class Ser>
    auto serialize_value(const std::vector<Ser> &sers, dom_val &val) -> dom_val & {
        dom_arr arr;

        for (const auto &ser : sers) {
            dom_val child_val;
            arr.push_back(serialize_value(ser, child_val));
        }

        val = arr;
        return val;
    }

    template <class Ser>
    auto serialize_value(
        const std::unordered_map<std::string, Ser> &sers, dom_val &val) -> dom_val & {

        auto &obj = create_obj(val);

        for (const auto &nameSrz : sers) {
            dom_val child_val(nameSrz.first);
            serialize_value(nameSrz.second, child_val);
            obj.emplace(nameSrz.first, std::move(child_val)); 
        }

        return val;
    }

    template <class Ser>
    auto serialize_value(const ::rustfp::Option<Ser> &ser, dom_val &val) -> dom_val & {
        ser.match(
            [&val](const Ser &srzVal) { val = srzVal; },
            [&val] { val = dom_null(); });

        return val;
    }
}
