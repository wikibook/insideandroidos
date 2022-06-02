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
#include <vendor/acme/one/aproximity/1.0/IAproximity.h>
#include <vendor/acme/one/aproximity/1.0/types.h>
#include <utils/StrongPointer.h>
#include <getopt.h>

using vendor::acme::one::aproximity::V1_0::IAproximity;
using vendor::acme::one::aproximity::V1_0::ProximityDetails;
using vendor::acme::one::aproximity::V1_0::ProximitySummary;
using android::sp;

using namespace android::hardware;

static void detailsCb(const ProximityDetails& details) {
    printf("[details] precision min/max: %d/%d\n",
           details.precision.min,
           details.precision.max);
    printf("[details] proximity min/max: %d/%d\n",
           details.proximity.min,
           details.proximity.max);
}

static void summaryCb(const ProximitySummary& summary) {
    printf("[summary] poll call count: %lu, last poll call (ms): %ld\n",
           summary.pollCallCount,
           summary.lastPollCalledMs);
}

void printUsage(char *name) {
    printf("Usage: %s [-ds] [-g input_precision]\n", name);
    printf("\td:  Display the details of the sensor\n");
    printf("\tg:  Get the latest proximity reading at the precision\n");
    printf("\ts:  Print the summary details of the service\n");
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printUsage(argv[0]);
        return -1;
    }

    bool doDetails= false;
    bool doProximity = false;
    int precision = -1;
    bool doSummary = false;
    bool doDebug = false;
    int  currentOpt;

    //  Parse the command line options, determine what to do
    while ((currentOpt = getopt(argc, argv, "dg:s")) != -1) {
        switch (currentOpt) {
            case 'd':
                doDetails = true;
                break;
            case 'g':
                doProximity = true;
                precision = atoi(optarg);
                break;
            case 's':
                doSummary = true;
                break;
            case '?':
                if (isprint(optopt)) {
                    fprintf(stderr,
                            "Unknown option: '%c'\n",
                            optopt);
                } else {
                    fprintf(stderr,
                            "Unknown option character: '\\x%X'\n",
                            optopt);
                }

                printUsage(argv[0]);
                return -1;

            default:
                abort();
        }
    }

    if (!doDetails && !doProximity && !doSummary) {
        doDebug = true;
    }

    //  Get the HIDL to use (IAproximity)
    sp<IAproximity>  client = IAproximity::getService();
    if (client == nullptr) {
        fprintf(stderr, "Unable to get aproximity service interface\n");
        return -2;
    }

    if (doDetails) {
        Return<void> result = client->get_details(detailsCb);
        if (!result.isOk()) {
            fprintf(stderr,
                    "Unable to get proximity service details. Err: %s\n",
                    result.description().c_str());
            return -1;
        }
    }

    if (doProximity) {
        Return<int32_t> retProximity = client->poll(precision);
        if (!retProximity.isOk()) {
            fprintf(stderr,
                    "Unable to get proximity for precision (%d). Err: %s\n",
                    precision,
                    retProximity.description().c_str());
            return -1;
        }

        printf("[proximity] %d\n", static_cast<int32_t>(retProximity));
    }

    if (doSummary) {
        client->summarize(summaryCb);
    }

    return 0;
}
