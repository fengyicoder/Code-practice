#include <cstdio>
#include "../pr.h"

void pr_test(const char* title) {
    printf("------------level %s-----------\r\n", title);
    PR_DEBUG("pr: %s\r\n", "debug");
    PR_INFO("pr: %s\r\n", "info");
    PR_WARN("pr: %s\r\n", "warn");
    PR_ERROR("pr: %s\r\n", "error");
    printf("\r\n");
}

int main() {
    std::ostringstream oss;
    std::string tid = tidToString(std::this_thread::get_id());
    PR_INFO("tid is:%s\n", tid.c_str());

    prSetLevel(PR_LEVEL_DEBUG);
    pr_test("debug");

    prSetLevel(PR_LEVEL_INFO);
    pr_test("info");

    prSetLevel(PR_LEVEL_WARN);
    pr_test("warn");

    prSetLevel(PR_LEVEL_ERROR);
    pr_test("error");
    return 0;
}