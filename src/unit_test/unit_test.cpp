#include "serz/serz.h"

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "rustfp/result.h"

#include <iostream>
#include <string>
#include <utility>

// serz
using serz::parse_from_json_content_and_ret;

// rustfp
using rustfp::Err;
using rustfp::Ok;
using rustfp::Result;

// std
using std::cerr;
using std::move;
using std::string;

// setup

struct X {
    int x;
    double y;
    string z;
    bool a;
};

namespace serz {
    auto parse_value(X &ser, const dom_val &val) -> Result<X &, string> {
        return as_obj(val) &
            parse_nvp(ser.x, "x") &
            parse_nvp(ser.y, "y") &
            parse_nvp(ser.z, "z") &
            parse_nvp(ser.a, "a") &
            done_obj(ser);
    }
}

// test cases

TEST_CASE("Parse X", "[parse_X]") {
    static constexpr auto CONTENT = "{"
        "\"x\": 777,"
        "\"y\": 0.5,"
        "\"z\": \"Hello World\","
        "\"a\": false"
        "}";

    auto parse_res = parse_from_json_content_and_ret<X>(CONTENT);

    parse_res.match_err([](const auto &err_msg) {
        cerr << err_msg << '\n';
    });

    REQUIRE(parse_res.is_ok());

    const auto x = move(parse_res).unwrap_unchecked();
    REQUIRE(777 == x.x);
    REQUIRE(0.5 == x.y);
    REQUIRE("Hello World" == x.z);
    REQUIRE(!x.a);
}