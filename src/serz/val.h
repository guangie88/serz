/**
 * Contains all DOM value types.
 * @author Chen Weiguang
 * @version 0.1.0
 */

#pragma once

#include "insert_map.h"
#include "traits.h"

#include "rustfp/option.h"

#include "mapbox/variant.hpp"

#ifndef FMT_HEADER_ONLY
#define FMT_HEADER_ONLY
#endif
#include "fmt/format.h"

#include <cstdint>
#include <functional>
#include <string>
#include <utility>
#include <vector>

namespace serz {
    // declaration section

    /**
     * Describes the various DOM value type.
     */
    enum class dom_val_type {
        null_type,
        obj_type,
        arr_type,
        bool_type,
        int_type,
        flt_type,
        str_type,
        null_string_obj_type,
    };

    // forward declaration
    class dom_val;

    /**
     * Alias to DOM object type contained in a dom_val.
     */
    using dom_obj = insert_map<std::string, dom_val>;

    /**
     * Alias to DOM array type contained in a dom_val.
     */
    using dom_arr = std::vector<dom_val>;

    /**
     * Alias to DOM boolean type contained in a dom_val.
     */
    using dom_bln = bool;

    /**
     * Alias to DOM integer type contained in a dom_val.
     */
    using dom_int = int64_t;

    /**
     * Alias to DOM floating-point type contained in a dom_val.
     */
    using dom_flt = double;

    /**
     * Alias to DOM string type contained in a dom_val.
     */
    using dom_str = std::string;

    /**
     * Representation of null value in the DOM value.
     */
    struct dom_null {};

    /**
     * Alias to DOM type that possibly represents null,
     * empty string or empty object.
     * This is only useful for XML serialization,
     * where a node with no children node is used.
     */
    struct dom_null_str_obj {};

    /**
     * Intermediate representation of DOM value for possibly
     * multiple implementations such as XML and JSON.
     */
    class dom_val
    {
    public:
        /**
         * Defaulted default constructor.
         */
        dom_val();

        /**
         * Defaulted move constructs into this instance.
         */
        dom_val(dom_val &&rhs) = default;

        /**
         * Copy constructs into this instance.
         */
        dom_val(const dom_val &rhs);

        /**
         * Initializes this instance with null value.
         */
        dom_val(const dom_null nll);

        /**
         * Initializes this instance with a given object value.
         */
        dom_val(const dom_obj &obj);

        /**
         * Initializes this instance with a given array value.
         */
        dom_val(const dom_arr &arr);

        /**
         * Initializes this instance with a given variant of null, string 
         */
        dom_val(const dom_null_str_obj nso);

        /**
         * Initializes this instance with a given boolean value.
         */
        dom_val(const dom_bln &bln, const bool is_attr = false);

        /**
         * Initializes this instance with a given integer value.
         */
        dom_val(const dom_int &itg, const bool is_attr = false);

        /**
         * Initializes this instance with a given floating point value.
         */
        dom_val(const dom_flt &flt, const bool is_attr = false);

        /**
         * Initializes this instance with a given string value.
         */
        dom_val(const dom_str &str, const bool is_attr = false);

        /**
         * Defaulted move assignment.
         */
        auto operator=(dom_val &&rhs) -> dom_val & = default;

        /**
         * Custom copy constructor which performs deep copying
         * of all the held values in this instance.
         */
        auto operator=(const dom_val &rhs) -> dom_val &;

        /**
         * Assigns new object value into this instance.
         */
        auto operator=(const dom_obj &obj) -> dom_val &;

        /**
         * Assigns new array value into this instance.
         */
        auto operator=(const dom_arr &arr) -> dom_val &;

        /**
         * Assigns new boolean value into this instance.
         */
        auto operator=(const dom_bln &bln) -> dom_val &;

        /**
         * Assigns new integer value into this instance.
         */
        auto operator=(const dom_int &itg) -> dom_val &;

        /**
         * Assigns new floating point value into this instance.
         */
        auto operator=(const dom_flt &flt) -> dom_val &;

        /**
         * Assigns new string value into this instance.
         */
        auto operator=(const dom_str &str) -> dom_val &;

        /**
         * Sets this instance to null.
         */
        auto operator=(const dom_null nll) -> dom_val &;

        /**
         * Sets this instance to variant of possibly null,
         * empty string or empty object.
         */
        auto operator=(const dom_null_str_obj nso) -> dom_val &;

        /**
         * Checks if this value is an attribute.
         */
        auto is_attribute() const -> bool;

        /**
         * Sets the is-attribute property to the given value.
         */
        auto set_attribute(const bool is_attr) -> dom_val &;

        /**
         * Gets the current DOM type that the value is holding.
         */
        auto get_type() const -> dom_val_type;

        /**
         * Checks if the value is holding is currently holding
         * the specified DOM type.
         */
        template <class DomType>
        auto is() const -> bool;

        /**
         * Gets the underlying type value with checking.
         */
        template <class DomType>
        auto get() -> ::rustfp::Option<DomType &>;

        /**
         * Gets the underlying type value with no checking.
         * Undefined behaviour if the given type does not match actual type.
         */
        template <class DomType>
        auto get_unchecked() -> DomType &;

        /**
         * Gets the underlying type value with checking. Readonly.
         */
        template <class DomType>
        auto get() const -> ::rustfp::Option<const DomType &>;

        /**
         * Gets the underlying type value with no checking. Readonly.
         * Undefined behaviour if the given type does not match actual type.
         */
        template <class DomType>
        auto get_unchecked() const -> const DomType &;

    private:
        /**
         * Holds the value from any of the possible the DOM value type.
         */
        mapbox::util::variant<
            mapbox::util::recursive_wrapper<dom_obj>,
            mapbox::util::recursive_wrapper<dom_arr>,
            dom_bln, dom_int, dom_flt, dom_str, dom_null, dom_null_str_obj> vts;

        /**
         * Meant only for XML serialization purposes.
         * For serializing the node as an attribute.
         */
        bool is_attr = false;
    };

    // implementation section

    inline dom_val::dom_val() : dom_val(dom_null()) {

    }

    inline dom_val::dom_val(const dom_val &rhs) :
        vts(rhs.vts),
        is_attr(rhs.is_attr) {

    }

    inline dom_val::dom_val(const dom_null nll) :
        vts(nll) {

    }

    inline dom_val::dom_val(const dom_obj &obj) :
        vts(obj) {

    }

    inline dom_val::dom_val(const dom_arr &arr) :
        vts(arr) {

    }

    inline dom_val::dom_val(const dom_null_str_obj nso) :
        vts(nso) {

    }

    inline dom_val::dom_val(const dom_bln &bln, const bool is_attr) :
        vts(bln),
        is_attr(is_attr) {

    }

    inline dom_val::dom_val(const dom_int &itg, const bool is_attr) :
        vts(itg),
        is_attr(is_attr) {

    }

    inline dom_val::dom_val(const dom_flt &flt, const bool is_attr) :
        vts(flt),
        is_attr(is_attr) {

    }

    inline dom_val::dom_val(const dom_str &str, const bool is_attr) :
        vts(str),
        is_attr(is_attr) {

    }

    inline auto dom_val::operator=(const dom_val &rhs) -> dom_val & {
        vts = rhs.vts;
        is_attr = rhs.is_attr;
        return *this;
    }

    inline auto dom_val::operator=(const dom_null nll) -> dom_val & {
        vts = nll;
        return *this;
    }

    inline auto dom_val::operator=(const dom_obj &obj) -> dom_val & {
        vts = obj;
        return *this;
    }

    inline auto dom_val::operator=(const dom_arr &arr) -> dom_val & {
        vts = arr;
        return *this;
    }

    inline auto dom_val::operator=(const dom_bln &bln) -> dom_val & {
        vts = bln;
        return *this;
    }

    inline auto dom_val::operator=(const dom_int &itg) -> dom_val & {
        vts = itg;
        return *this;
    }

    inline auto dom_val::operator=(const dom_flt &flt) -> dom_val & {
        vts = flt;
        return *this;
    }

    inline auto dom_val::operator=(const dom_str &str) -> dom_val & {
        vts = str;
        return *this;
    }

    inline auto dom_val::is_attribute() const -> bool {
        return is_attr;
    }

    inline auto dom_val::set_attribute(const bool is_attr) -> dom_val & {
        this->is_attr = is_attr;
        return *this;
    }

    inline auto dom_val::get_type() const -> dom_val_type {
        return
            vts.is<dom_obj>() ? dom_val_type::obj_type :
            vts.is<dom_arr>() ? dom_val_type::arr_type :
            vts.is<dom_bln>() ? dom_val_type::bool_type :
            vts.is<dom_int>() ? dom_val_type::int_type :
            vts.is<dom_flt>() ? dom_val_type::flt_type :
            vts.is<dom_str>() ? dom_val_type::str_type :
            vts.is<dom_null>() ? dom_val_type::null_type :
            dom_val_type::null_string_obj_type;
    }

    template <>
    inline auto dom_val::is<dom_obj>() const -> bool {
        return get_type() == dom_val_type::obj_type;
    }

    template <>
    inline auto dom_val::is<dom_arr>() const -> bool {
        return get_type() == dom_val_type::arr_type;
    }

    template <>
    inline auto dom_val::is<dom_bln>() const -> bool {
        return get_type() == dom_val_type::bool_type;
    }

    template <>
    inline auto dom_val::is<dom_int>() const -> bool {
        return get_type() == dom_val_type::int_type;
    }

    template <>
    inline auto dom_val::is<dom_flt>() const -> bool {
        return get_type() == dom_val_type::flt_type;
    }

    template <>
    inline auto dom_val::is<dom_str>() const -> bool {
        return get_type() == dom_val_type::str_type;
    }

    template <>
    inline auto dom_val::is<dom_null>() const -> bool {
        return get_type() == dom_val_type::null_type;
    }

    template <>
    inline auto dom_val::is<dom_null_str_obj>() const -> bool {
        return get_type() == dom_val_type::null_string_obj_type;
    }

    template <class DomType>
    auto dom_val::get() -> ::rustfp::Option<DomType &> {
        return is<DomType>()
            ? ::rustfp::Some(std::ref(vts.get_unchecked<DomType>()))
            : ::rustfp::None;
    }

    template <class DomType>
    auto dom_val::get_unchecked() -> DomType & {
        return vts.get_unchecked<DomType>();
    }

    template <class DomType>
    auto dom_val::get() const -> ::rustfp::Option<const DomType &> {
        return is<DomType>()
            ? ::rustfp::Some(std::ref(vts.get_unchecked<DomType>()))
            : ::rustfp::None;
    }

    template <class DomType>
    auto dom_val::get_unchecked() const -> const DomType & {
        return vts.get_unchecked<DomType>();
    }
}
