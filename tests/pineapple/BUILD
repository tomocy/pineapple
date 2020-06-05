load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "app",
    srcs = ["app_test.cc"],
    deps = [
        "//src/pineapple",
        "@gtest//:gtest_main",
    ],
)

cc_test(
    name = "command",
    srcs = ["command_test.cc"],
    deps = [
        "//src/pineapple",
        "@gtest//:gtest_main",
    ],
)
