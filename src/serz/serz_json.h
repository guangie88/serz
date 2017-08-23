/**
 * Provides JSON parsing and serialization into intermediate DOM representation.
 * @author Chen Weiguang
 * @version 0.1.0
 */

#pragma once

#include "etor.h"
#include "serialization.h"

#include "rustfp/result.h"
#include "rustfp/unit.h"

#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"

#ifndef FMT_HEADER_ONLY
#define FMT_HEADER_ONLY
#endif
#include "fmt/format.h"

#include <fstream>
#include <functional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

namespace serz {
    // declaration section

    /** Alias to implementation document type. */
    using json_doc = rapidjson::Document;

    /** Alias to implementation JSON value. */
    using json_val = rapidjson::GenericValue<rapidjson::UTF8<>>;

    /** Alias to implementation JSON object. */
    using json_obj = rapidjson::GenericObject<true, json_val>;

    /** Alias to implementation JSON array. */
    using json_arr = rapidjson::GenericArray<true, json_val>;

    /** Alias to implementation JSON string. */
    using json_str = std::string;

    /**
     * Parses the JSON content into DOM value.
     */
    auto parse_json(const std::string &content) -> ::rustfp::Result<dom_val, std::string>;

    /**
     * Parses the JSON content from the given input file stream.
     */
    auto parse_json_from_stream(std::istream &istr) -> ::rustfp::Result<dom_val, std::string>;

    /**
     * Parses the JSON content in the file path into DOM value.
     */
    auto parse_json_from_file(const std::string &file_path) -> ::rustfp::Result<dom_val, std::string>;

    /**
     * Parses the JSON content into the referenced serializable value.
     */
    template <class Ser>
    auto parse_from_json_content(Ser &ser, const std::string &content) -> ::rustfp::Result<Ser &, std::string>;

    /**
     * Parses the JSON content and returns the serializable value.
     * Serializable value must be default constructible.
     */
    template <class Ser>
    auto parse_from_json_content_and_ret(const std::string &content) -> ::rustfp::Result<Ser, std::string>;

    /**
     * Parses the JSON content into the referenced serializable value
     * from the given input file stream.
     */
    template <class Ser>
    auto parse_from_json_stream(Ser &ser, std::istream &istr) -> ::rustfp::Result<Ser &, std::string>;

    /**
     * Parses the JSON content and returns the serializable value
     * from the given input file stream.
     */
    template <class Ser>
    auto parse_from_json_stream_and_ret(std::istream &istr) -> ::rustfp::Result<Ser, std::string>;

    /**
     * Parses the JSON content in the file path into the referenced serializable value.
     */
    template <class Ser>
    auto parse_from_json_file(Ser &ser, const std::string &file_path) -> ::rustfp::Result<Ser &, std::string>;

    /**
     * Parses the JSON content in the file path and returns the serializable value.
     * Serializable value must be default constructible.
     */
    template <class Ser>
    auto parse_from_json_file_and_ret(const std::string &file_path) -> ::rustfp::Result<Ser, std::string>;

    /**
     * Serializes the DOM value into JSON content.
     */
    auto serialize_json(const dom_val &val) -> std::string;

    /**
     * Serializes the DOM value into JSON content and writes into the output stream.
     */
    auto serialize_json_into_stream(const dom_val &val, std::ostream &ostr) -> ::rustfp::Result<::rustfp::unit_t, std::string>;

    /**
     * Serializes the DOM value into JSON content and writes into the given file path.
     */
    auto serialize_json_into_file(const dom_val &val, const std::string &file_path) -> ::rustfp::Result<::rustfp::unit_t, std::string>;

    /**
     * Serializes the given serializable value into JSON content.
     */
    template <class Ser>
    auto serialize_into_json_content(const Ser &ser) -> std::string;

    /**
     * Serializes the given serializable value into JSON content and writes into the output stream.
     */
    template <class Ser>
    auto serialize_into_json_stream(const Ser &ser, std::ostream &ostr) -> ::rustfp::Result<const Ser &, std::string>;

    /**
     * Serializes the given serializable value into JSON content and writes into the given file path.
     */
    template <class Ser>
    auto serialize_into_json_file(const Ser &ser, const std::string &file_path) -> ::rustfp::Result<const Ser &, std::string>;

    // implementation section

    namespace details {
        inline auto make_json_dom_val() -> dom_val {
            return dom_val();
        }

        template <class DomType>
        auto make_json_dom_val(DomType &&dom_type_v) -> dom_val {
            return dom_val(dom_type_v);
        }

        inline auto parse_json_impl(const json_val &json_val_v) -> ::rustfp::Result<dom_val, std::string> {
            if (json_val_v.IsObject()) {
                const auto json_obj_v = json_val_v.GetObject();

                auto val = make_json_dom_val(dom_obj());
                auto &obj = val.get_unchecked<dom_obj>();

                for (const auto &json_pair : json_obj_v) {
                    const auto childRes = parse_json_impl(json_pair.value);

                    childRes.match_ok([&json_pair, &obj](const dom_val &child_val_v) {
                        obj.emplace(json_pair.name.GetString(), child_val_v);
                    });
                }

                return ::rustfp::Ok(std::move(val));
            } else if (json_val_v.IsArray()) {
                const auto jsonArr = json_val_v.GetArray();

                auto val = make_json_dom_val(dom_arr());
                auto &arr = val.get_unchecked<dom_arr>();

                for (const auto &jsonElem : jsonArr) {
                    const auto childRes = parse_json_impl(jsonElem);

                    childRes.match_ok([&arr](const dom_val &child_val_v) {
                        arr.push_back(child_val_v);
                    });
                }

                return ::rustfp::Ok(std::move(val));
            } else if (json_val_v.IsString()) {
                auto val = make_json_dom_val(dom_str());
                val = std::string(json_val_v.GetString());
                return ::rustfp::Ok(std::move(val));
            } else if (json_val_v.IsBool()) {
                auto val = make_json_dom_val(dom_bln());
                val = json_val_v.GetBool();
                return ::rustfp::Ok(std::move(val));
            } else if (json_val_v.IsInt() || json_val_v.IsInt64() || json_val_v.IsUint() || json_val_v.IsUint64()) {
                auto val = make_json_dom_val(dom_int());

                // only uint64_t will suffer loss in precision
                if (json_val_v.IsInt()) val = static_cast<int64_t>(json_val_v.GetInt());
                else if (json_val_v.IsInt64()) val = json_val_v.GetInt64();
                else if (json_val_v.IsUint()) val = static_cast<int64_t>(json_val_v.GetUint());
                else val = static_cast<int64_t>(json_val_v.GetUint64());

                return ::rustfp::Ok(std::move(val));
            } else if (json_val_v.IsFloat() || json_val_v.IsDouble()) {
                auto val = make_json_dom_val(dom_flt());

                if (json_val_v.IsFloat()) val = static_cast<double>(json_val_v.GetFloat());
                else val = json_val_v.GetDouble();

                return ::rustfp::Ok(std::move(val));
            } else if (json_val_v.IsNull()) {
                return ::rustfp::Ok(make_json_dom_val(dom_null()));
            } else {
                return ::rustfp::Err(std::string("Invalid value type found while parsing JSON values"));
            }
        }

        inline void serialize_json_impl(const dom_val &val, json_val &json_val_v, json_doc &doc) {
            if (val.is<dom_obj>()) {
                const auto &obj = val.get_unchecked<dom_obj>();
                auto json_obj_v = json_val_v.SetObject().GetObject();

                for (const auto &keyValue : obj) {
                    json_val jsonStrVal;
                    jsonStrVal.SetString(keyValue.first.c_str(), doc.GetAllocator());

                    json_val child_val_v;
                    serialize_json_impl(keyValue.second, child_val_v, doc);
                    json_obj_v.AddMember(jsonStrVal, child_val_v, doc.GetAllocator());
                }
            } else if (val.is<dom_arr>()) {
                const auto &arr = val.get_unchecked<dom_arr>();
                auto jsonArr = json_val_v.SetArray().GetArray();

                for (const auto &elem : arr) {
                    json_val child_val_v;
                    serialize_json_impl(elem, child_val_v, doc);
                    jsonArr.PushBack(child_val_v, doc.GetAllocator());
                }
            } else if (val.is<dom_bln>()) {
                json_val_v.SetBool(val.get_unchecked<dom_bln>());
            } else if (val.is<dom_int>()) {
                json_val_v.SetInt64(val.get_unchecked<dom_int>());
            } else if (val.is<dom_flt>()) {
                json_val_v.SetDouble(val.get_unchecked<dom_flt>());
            } else if (val.is<dom_str>()) {
                const auto &str = val.get_unchecked<dom_str>();
                json_val_v.SetString(str.c_str(), doc.GetAllocator());
            }
        }
    }

    inline auto parse_json(const std::string &content) -> ::rustfp::Result<dom_val, std::string> {
        return etor<>::mix([&content] {
            rapidjson::Document doc;
            doc.Parse<rapidjson::kParseCommentsFlag | rapidjson::kParseTrailingCommasFlag>(content.c_str());

            // accept empty content
            return !doc.HasParseError() || content.empty()
                ? details::parse_json_impl(doc)
                : ::rustfp::Err(fmt::format("Error in parsing JSON content: {}", content));
        });
    }
    
    inline auto parse_json_from_stream(std::istream &istr) -> ::rustfp::Result<dom_val, std::string> {
        std::stringstream fileStrStream;
        fileStrStream << istr.rdbuf();
        return parse_json(fileStrStream.str());
    }

    inline auto parse_json_from_file(const std::string &file_path) -> ::rustfp::Result<dom_val, std::string> {
        std::ifstream file_stream(file_path);

        if (!file_stream){
            return ::rustfp::Err(fmt::format("Cannot open file at '{}' for JSON parsing", file_path));
        }

        return parse_json_from_stream(file_stream);
    }

    template <class Ser>
    auto parse_from_json_content(Ser &ser, const std::string &content) -> ::rustfp::Result<Ser &, std::string> {
        return parse_json(content)
            .and_then([&ser](const dom_val &val) { return parse_value(ser, val); });
    }

    template <class Ser>
    auto parse_from_json_content_and_ret(const std::string &content) -> ::rustfp::Result<Ser, std::string> {
        Ser ser;

        return parse_from_json_content(ser, content)
            .map([](Ser &ser) { return std::move(ser); });
    }

    template <class Ser>
    auto parse_from_json_stream(Ser &ser, std::istream &istr) -> ::rustfp::Result<Ser &, std::string> {
        return parse_json_from_stream(istr)
            .and_then([&ser](const dom_val &val) { return parse_value(ser, val); });
    }

    template <class Ser>
    auto parse_from_json_stream_and_ret(std::istream &istr) -> ::rustfp::Result<Ser, std::string> {
        Ser ser;

        return parse_from_json_stream(ser, istr)
            .map([](Ser &ser) { return std::move(ser); });
    }

    template <class Ser>
    auto parse_from_json_file(Ser &ser, const std::string &file_path) -> ::rustfp::Result<Ser &, std::string> {
        return parse_json_from_file(file_path)
            .and_then([&ser](const dom_val &val) { return parse_value(ser, val); });
    }

    template <class Ser>
    auto parse_from_json_file_and_ret(const std::string &file_path) -> ::rustfp::Result<Ser, std::string> {
        Ser ser;

        return parse_from_json_file(ser, file_path)
            .map([](Ser &ser) { return std::move(ser); });
    }

    inline auto serialize_json(const dom_val &val) -> std::string {
        rapidjson::Document doc;
        details::serialize_json_impl(val, doc, doc);

        rapidjson::StringBuffer buf;
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buf);
        doc.Accept(writer);

        return std::string(buf.GetString(), buf.GetSize());
    }

    inline auto serialize_json_into_stream(const dom_val &val, std::ostream &ostr) -> ::rustfp::Result<::rustfp::unit_t, std::string> {
        const auto serializedStr = serialize_json(val);
        ostr << serializedStr;
        return ::rustfp::Ok(::rustfp::Unit);
    }

    inline auto serialize_json_into_file(const dom_val &val, const std::string &file_path) -> ::rustfp::Result<::rustfp::unit_t, std::string> {
        std::ofstream file_stream(file_path);

        if (!file_stream) {
            return ::rustfp::Err(fmt::format("Cannot open file at '{}' for JSON serialization", file_path));
        }

        return serialize_json_into_stream(val, file_stream);
    }

    template <class Ser>
    auto serialize_into_json_content(const Ser &ser) -> std::string {
        auto val = details::make_json_dom_val();
        SerializeValue(ser, val);
        return serialize_json(val);
    }

    template <class Ser>
    auto serialize_into_json_stream(const Ser &ser, std::ostream &ostr) -> ::rustfp::Result<const Ser &, std::string> {
        auto val = details::make_json_dom_val();
        SerializeValue(ser, val);

        return serialize_json_into_stream(val, ostr)
            .map([&ser](auto) { return std::cref(ser); });
    }

    template <class Ser>
    auto serialize_into_json_file(const Ser &ser, const std::string &file_path) -> ::rustfp::Result<const Ser &, std::string> {
        auto val = details::make_json_dom_val();
        SerializeValue(ser, val);

        return serialize_json_into_file(val, file_path)
            .map([&ser](auto) { return std::cref(ser); });
    }
}
