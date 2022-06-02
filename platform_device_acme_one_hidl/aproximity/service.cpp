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
#include <hidl/HidlSupport.h>
#include <hidl/HidlTransportSupport.h>
#include <utils/Errors.h>
#include <utils/StrongPointer.h>

#include "Aproximity.h"

using android::hardware::configureRpcThreadpool;
using android::hardware::joinRpcThreadpool;
using vendor::acme::one::aproximity::V1_0::implementation::Aproximity;
using namespace android;

int main() {
    configureRpcThreadpool(1, true);

    sp<Aproximity>  aproximity = new Aproximity();
    status_t status = aproximity->registerAsService("default");

    if (status != OK) {
        return status;
    }

    joinRpcThreadpool();
}

