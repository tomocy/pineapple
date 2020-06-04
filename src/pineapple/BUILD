load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "pineapple",
    srcs = ["pineapple.cc"],
    hdrs = ["pineapple.h"],
    visibility = [
        "//example:__pkg__",
        "//tests/pineapple:__pkg__",
    ],
    deps = [
        "//src/pineapple:exceptions",
        "@flags//src/flags",
    ],
)

cc_library(
    name = "exceptions",
    srcs = ["exceptions.cc"],
    hdrs = ["exceptions.h"],
)
