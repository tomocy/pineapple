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
    name = "flag_lexer",
    srcs = ["flag_lexer_test.cc"],
    deps = [
        "//src:flag_lexer",
        "@gtest//:gtest_main",
    ],
)
