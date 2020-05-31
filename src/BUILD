load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "flag_parser",
    srcs = ["flag_parser.cc"],
    hdrs = ["flag_parser.h"],
    visibility = [
        "//tests:__pkg__",
    ],
    deps = [
        "//src:pineapple",
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
)
