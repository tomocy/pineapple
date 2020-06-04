load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "pineapple",
    hdrs = ["pineapple.h"],
    visibility = [
        "//example:__pkg__",
        "//tests/pineapple:__pkg__",
    ],
    deps = [
        "//src/pineapple:app",
        "//src/pineapple:command",
        "//src/pineapple:context",
        "//src/pineapple:exceptions",
        "@flags//src/flags",
    ],
)

cc_library(
    name = "app",
    srcs = ["app.cc"],
    hdrs = ["app.h"],
    deps = [
        "//src/pineapple:command",
        "//src/pineapple:context",
        "//src/pineapple:exceptions",
        "@flags//src/flags",
    ],
)

cc_library(
    name = "command",
    srcs = ["command.cc"],
    hdrs = ["command.h"],
    deps = [
        "//src/pineapple:context",
        "//src/pineapple:exceptions",
        "@flags//src/flags",
    ],
)

cc_library(
    name = "context",
    srcs = ["context.cc"],
    hdrs = ["context.h"],
    deps = [
        "@flags//src/flags",
    ],
)

cc_library(
    name = "exceptions",
    srcs = ["exceptions.cc"],
    hdrs = ["exceptions.h"],
)
