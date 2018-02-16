//
//  yas_processing_connector_private.h
//

#pragma once

namespace yas::processing {
template <typename T, typename Enable = void>
struct enum_to_connector_index;

template <typename T>
struct enum_to_connector_index<T, typename std::enable_if_t<std::is_enum<T>::value>> {
    static connector_index_t to_index(T const &value) {
        return static_cast<connector_index_t>(value);
    }
};
}

template <typename T>
yas::processing::connector_index_t yas::processing::to_connector_index(T const &value) {
    return enum_to_connector_index<T>::to_index(value);
}
