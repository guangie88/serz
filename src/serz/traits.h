/**
 * Contains specialized type traits to be used within the domain of serz.
 * @author Chen Weiguang
 * @version 0.1.0
 */

#pragma once

#include "rustfp/traits.h"

namespace serz {
    template <class T>
    using special_decay_t = ::rustfp::special_decay_t<T>;
}
