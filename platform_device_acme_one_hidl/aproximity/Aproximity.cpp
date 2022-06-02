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
#include <chrono>
#include "Aproximity.h"

using namespace std::chrono;

namespace vendor {
namespace acme {
namespace one {
namespace aproximity {
namespace V1_0 {
namespace implementation {

static int64_t now() {
    time_point now = system_clock().now();
    milliseconds nowMs =
        duration_cast<milliseconds>(now.time_since_epoch());
    return static_cast<int64_t>(nowMs.count());
}

Aproximity::Aproximity() {
    this->fd = open_sensor(this->params);
    if (this->fd < 0) {
        this->params.precision.min = -1;
        this->params.precision.range = -1;
        this->params.proximity.min = -1;
        this->params.proximity.range = -1;
    }
}

Aproximity::~Aproximity() {
    if (this->fd >= 0) {
        close_sensor(this->fd);
        this->fd = -1;
    }

    this->pollCallCount = 0;
    this->lastPollCalledMs = 0;
}

// Methods from ::vendor::acme::one::aproximity::V1_0::IAproximity follow.
Return<int32_t> Aproximity::poll(int32_t precision) {
    this->pollCallCount++;
    this->lastPollCalledMs = now(); 

    if (this->fd < 0) {
        return -1;
    }

    int shimPrecision = static_cast<int>(precision);
    int32_t result =
        static_cast<int32_t>(poll_sensor(this->fd, shimPrecision));

    return result;
}

Return<void> Aproximity::get_details(get_details_cb _hidl_cb) {
    ProximityDetails  result;

    result.precision.min = static_cast<int32_t>(this->params.precision.min);
    result.precision.max = static_cast<int32_t>(this->params.precision.range);
    result.proximity.min = static_cast<int32_t>(this->params.proximity.min);
    result.proximity.max = static_cast<int32_t>(this->params.proximity.range);

    _hidl_cb(result);
    return Void();
}

Return<void> Aproximity::summarize(summarize_cb _hidl_cb) {
    ProximitySummary  result;

    result.pollCallCount = this->pollCallCount;
    result.lastPollCalledMs = this->lastPollCalledMs;
    _hidl_cb(result);
    return Void();
}

Return<void> Aproximity::reset() {
    // No-op
    return Void();
}

Return<void> Aproximity::debug(const hidl_handle &handle,
                            const hidl_vec<hidl_string> & /*options*/) {
    if (handle == nullptr || handle->numFds < 1 || handle->data[0] < 0) {
        return Void();
    }

    int fd = handle->data[0];
    dprintf(fd, "HIDL:\n");
    dprintf(fd, "  Poll call count: %lu\n", this->pollCallCount);
    dprintf(fd, "  Last poll call:  %ld\n", this->lastPollCalledMs);
    fsync(fd);
    return Void();
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace aproximity
}  // namespace one
}  // namespace acme
}  // namespace vendor
