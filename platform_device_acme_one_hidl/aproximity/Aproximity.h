/*
 * Copyright (C) 2020 Larry Schiefer and Blake Meike
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#pragma once

#include <vendor/acme/one/aproximity/1.0/IAproximity.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>
#include "dev/proximity_sensor.h"

namespace vendor {
namespace acme {
namespace one {
namespace aproximity {
namespace V1_0 {
namespace implementation {

using ::android::hardware::hidl_array;
using ::android::hardware::hidl_handle;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::sp;

struct Aproximity : public IAproximity {
    Aproximity();
    ~Aproximity();

    // Methods from ::vendor::acme::one::aproximity::V1_0::IAproximity follow.
    Return<int32_t> poll(int32_t precision) override;
    Return<void> get_details(get_details_cb _hidl_cb) override;
    Return<void> summarize(summarize_cb _hidl_cb) override;
    Return<void> reset() override;

    // Methods from ::android::hidl::base::V1_0::IBase follow.
    Return<void> debug(const hidl_handle &handle,
                       const hidl_vec<hidl_string> &options) override;

private:
    uint64_t            pollCallCount;
    int64_t             lastPollCalledMs;
    int                 fd;
    proximity_params_t  params;
};

}  // namespace implementation
}  // namespace V1_0
}  // namespace aproximity
}  // namespace one
}  // namespace acme
}  // namespace vendor
