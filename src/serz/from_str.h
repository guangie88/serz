/**
 * Contains all string to primitive types parsing conversion.
 * @author Chen Weiguang
 * @version 0.1.0
 */

#pragma once

#include "rustfp/option.h"

#include <cstdint>
#include <cstring>
#include <sstream>

namespace from_str {
    // declaration section

    auto parse_u8(const char target[]) -> ::rustfp::Option<uint8_t>;

    auto parse_u8(const std::string &target) -> ::rustfp::Option<uint8_t>;

    auto parse_u16(const char target[]) -> ::rustfp::Option<uint16_t>;

    auto parse_u16(const std::string &target) -> ::rustfp::Option<uint16_t>;

    auto parse_u32(const char target[]) -> ::rustfp::Option<uint32_t>;

    auto parse_u32(const std::string &target) -> ::rustfp::Option<uint32_t>;

    auto parse_u64(const char target[]) -> ::rustfp::Option<uint64_t>;

    auto parse_u64(const std::string &target) -> ::rustfp::Option<uint64_t>;

    auto parse_i8(const char target[]) -> ::rustfp::Option<int8_t>;

    auto parse_i8(const std::string &target) -> ::rustfp::Option<int8_t>;

    auto parse_i16(const char target[]) -> ::rustfp::Option<int16_t>;

    auto parse_i16(const std::string &target) -> ::rustfp::Option<int16_t>;

    auto parse_i32(const char target[]) -> ::rustfp::Option<int32_t>;

    auto parse_i32(const std::string &target) -> ::rustfp::Option<int32_t>;

    auto parse_i64(const char target[]) -> ::rustfp::Option<int64_t>;

    auto parse_i64(const std::string &target) -> ::rustfp::Option<int64_t>;

    auto parse_f32(const char target[]) -> ::rustfp::Option<float>;

    auto parse_f32(const std::string &target) -> ::rustfp::Option<float>;

    auto parse_f64(const char target[]) -> ::rustfp::Option<double>;

    auto parse_f64(const std::string &target) -> ::rustfp::Option<double>;

    auto parse_bool(const char target[]) -> ::rustfp::Option<bool>;

    auto parse_bool(const std::string &target) -> ::rustfp::Option<bool>;

    template <class T>
    auto parse(const char target[]) -> ::rustfp::Option<T>;

    template <class T>
    auto parse(const std::string &target) -> ::rustfp::Option<T>;

    // implementation section

    inline auto parse_u8(const char target[]) -> ::rustfp::Option<uint8_t> {
        std::stringstream ss;
        uint8_t val;
        ss << target;
        ss >> val;
        return ::rustfp::Some(val);
    }

    inline auto parse_u8(const std::string &target) -> ::rustfp::Option<uint8_t> {
            return parse_u8(target.c_str());
    }

    inline auto parse_u16(const char target[]) -> ::rustfp::Option<uint16_t> {
        std::stringstream ss;
        uint16_t val;
        ss << target;
        ss >> val;
        return ::rustfp::Some(val);
    }

    inline auto parse_u16(const std::string &target) -> ::rustfp::Option<uint16_t> {
        return parse_u16(target.c_str());
    }

    inline auto parse_u32(const char target[]) -> ::rustfp::Option<uint32_t> {
        std::stringstream ss;
        uint32_t val;
        ss << target;
        ss >> val;
        return ::rustfp::Some(val);
    }

    inline auto parse_u32(const std::string &target) -> ::rustfp::Option<uint32_t> {
        return parse_u32(target.c_str());
    }

    inline auto parse_u64(const char target[]) -> ::rustfp::Option<uint64_t> {
        std::stringstream ss;
        uint64_t val;
        ss << target;
        ss >> val;
        return ::rustfp::Some(val);
    }

    inline auto parse_u64(const std::string &target) -> ::rustfp::Option<uint64_t> {
        return parse_u64(target.c_str());
    }

    inline auto parse_i8(const char target[]) -> ::rustfp::Option<int8_t> {
        std::stringstream ss;
        int8_t val;
        ss << target;
        ss >> val;
        return ::rustfp::Some(val);
    }

    inline auto parse_i8(const std::string &target) -> ::rustfp::Option<int8_t> {
        return parse_i8(target.c_str());
    }

    inline auto parse_i16(const char target[]) -> ::rustfp::Option<int16_t> {
        std::stringstream ss;
        int16_t val;
        ss << target;
        ss >> val;
        return ::rustfp::Some(val);
    }

    inline auto parse_i16(const std::string &target) -> ::rustfp::Option<int16_t> {
        return parse_i16(target.c_str());
    }

    inline auto parse_i32(const char target[]) -> ::rustfp::Option<int32_t> {
        std::stringstream ss;
        int32_t val;
        ss << target;
        ss >> val;
        return ::rustfp::Some(val);
    }

    inline auto parse_i32(const std::string &target) -> ::rustfp::Option<int32_t> {
        return parse_i32(target.c_str());
    }

    inline auto parse_i64(const char target[]) -> ::rustfp::Option<int64_t> {
        std::stringstream ss;
        int64_t val;
        ss << target;
        ss >> val;
        return ::rustfp::Some(val);
    }

    inline auto parse_i64(const std::string &target) -> ::rustfp::Option<int64_t> {
        return parse_i64(target.c_str());
    }

    inline auto parse_f32(const char target[]) -> ::rustfp::Option<float> {
        std::stringstream ss;
        float val;
        ss << target;
        ss >> val;
        return ::rustfp::Some(val);
    }

    inline auto parse_f32(const std::string &target) -> ::rustfp::Option<float> {
        return parse_f32(target.c_str());
    }

    inline auto parse_f64(const char target[]) -> ::rustfp::Option<double> {
        std::stringstream ss;
        double val;
        ss << target;
        ss >> val;
        return ::rustfp::Some(val);
    }

    inline auto parse_f64(const std::string &target) -> ::rustfp::Option<double> {
        return parse_f64(target.c_str());
    }

    inline auto parse_bool(const char target[]) -> ::rustfp::Option<bool> {
        if (target[0] == 't' &&
            target[1] == 'r' &&
            target[2] == 'u' &&
            target[3] == 'e' &&
            std::strlen(target) == 4) {
            
            return ::rustfp::Some(true);
        } else if (target[0] == 'f' &&
            target[1] == 'a' &&
            target[2] == 'l' &&
            target[3] == 's' &&
            target[4] == 'e' &&
            std::strlen(target) == 5) {

            return ::rustfp::Some(false);
        } else {
            return ::rustfp::None;
        }
    }

    inline auto parse_bool(const std::string &target) -> ::rustfp::Option<bool> {
        return parse_bool(target.c_str());
    }

    template <>
    inline auto parse<uint8_t>(const char target[]) -> ::rustfp::Option<uint8_t> {
        return parse_u8(target);
    }

    template <>
    inline auto parse<uint16_t>(const char target[]) -> ::rustfp::Option<uint16_t> {
        return parse_u16(target);
    }

    template <>
    inline auto parse<uint32_t>(const char target[]) -> ::rustfp::Option<uint32_t> {
        return parse_u32(target);
    }

    template <>
    inline auto parse<uint64_t>(const char target[]) -> ::rustfp::Option<uint64_t> {
        return parse_u64(target);
    }

    template <>
    inline auto parse<int8_t>(const char target[]) -> ::rustfp::Option<int8_t> {
        return parse_i8(target);
    }

    template <>
    inline auto parse<int16_t>(const char target[]) -> ::rustfp::Option<int16_t> {
        return parse_i16(target);
    }

    template <>
    inline auto parse<int32_t>(const char target[]) -> ::rustfp::Option<int32_t> {
        return parse_i32(target);
    }

    template <>
    inline auto parse<int64_t>(const char target[]) -> ::rustfp::Option<int64_t> {
        return parse_i64(target);
    }

    template <>
    inline auto parse<float>(const char target[]) -> ::rustfp::Option<float> {
        return parse_f32(target);
    }

    template <>
    inline auto parse<double>(const char target[]) -> ::rustfp::Option<double> {
        return parse_f64(target);
    }

    template <>
    inline auto parse<bool>(const char target[]) -> ::rustfp::Option<bool> {
        return parse_bool(target);
    }

    template <>
    inline auto parse<uint8_t>(const std::string &target) -> ::rustfp::Option<uint8_t> {
        return parse_u8(target);
    }

    template <>
    inline auto parse<uint16_t>(const std::string &target) -> ::rustfp::Option<uint16_t> {
        return parse_u16(target);
    }

    template <>
    inline auto parse<uint32_t>(const std::string &target) -> ::rustfp::Option<uint32_t> {
        return parse_u32(target);
    }

    template <>
    inline auto parse<uint64_t>(const std::string &target) -> ::rustfp::Option<uint64_t> {
        return parse_u64(target);
    }

    template <>
    inline auto parse<int8_t>(const std::string &target) -> ::rustfp::Option<int8_t> {
        return parse_i8(target);
    }

    template <>
    inline auto parse<int16_t>(const std::string &target) -> ::rustfp::Option<int16_t> {
        return parse_i16(target);
    }

    template <>
    inline auto parse<int32_t>(const std::string &target) -> ::rustfp::Option<int32_t> {
        return parse_i32(target);
    }

    template <>
    inline auto parse<int64_t>(const std::string &target) -> ::rustfp::Option<int64_t> {
        return parse_i64(target);
    }

    template <>
    inline auto parse<float>(const std::string &target) -> ::rustfp::Option<float> {
        return parse_f32(target);
    }

    template <>
    inline auto parse<double>(const std::string &target) -> ::rustfp::Option<double> {
        return parse_f64(target);
    }

    template <>
    inline auto parse<bool>(const std::string &target) -> ::rustfp::Option<bool> {
        return parse_bool(target);
    }
}