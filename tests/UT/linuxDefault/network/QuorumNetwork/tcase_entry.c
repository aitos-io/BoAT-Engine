/******************************************************************************
 * Copyright (C) 2018-2021 aitos.io
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/

#include "check.h"
#include "tcase_network.h"
#include <stdio.h>
#include <fcntl.h>
#include <stdio.h>

Suite *make_quorumNetworkUtTest_suite(void);

int main(int argc, char *argv[])
{
    SRunner *sr = NULL;
    int failed_number = 0;
    int ret = 0;

    /* new adding test suite should create in here */
    Suite *suite_networkCreate = make_quorumNetworkUtTest_suite();

    /* create srunner and add first suite to it.
    The first suite in a suite runner is always added in function srunner_create,
    here set suite_wallet as first adding suite. */
    sr = srunner_create(suite_networkCreate);
    // srunner_add_suite(sr, suite_transaction);
    // srunner_add_suite(sr, suite_nodesDiscover);

    /* set generate test log in running path */
    srunner_set_log(sr, "test_statistics_report.txt");

    /* start to run all test case */
    srunner_run_all(sr, CK_NORMAL);
    /* get the failed unit test number */
    failed_number = srunner_ntests_failed(sr);

    srunner_free(sr);

    /* return failed unit number, this return value can be used in CI, CI tools according
    to this return value determine the test is passed or not  */
    return failed_number;
}