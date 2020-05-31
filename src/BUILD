load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "flag_lexer",
    srcs = ["flag_lexer.cc"],
    hdrs = ["flag_lexer.h"],
    visibility = [
        "//tests:__pkg__",
    ],
)

cc_library(
    name = "pineapple",
    srcs = ["pineapple.cc"],
    hdrs = ["pineapple.h"],
    visibility = [
        "//example:__pkg__",
        "//tests:__pkg__",
    ],
    deps = [
        "//src:flag_lexer",
    ],
)
