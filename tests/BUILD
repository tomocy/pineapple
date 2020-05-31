load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "pineapple",
    srcs = ["pineapple_test.cc"],
    deps = [
        "//src:pineapple",
        "@gtest//:gtest_main",
    ],
)

cc_test(
    name = "flag_parser",
    srcs = ["flag_parser_test.cc"],
    deps = [
        "//src:flag_parser",
        "@gtest//:gtest_main",
    ],
)
