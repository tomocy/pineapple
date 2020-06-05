"""Load dependencies needed to compile the pineapple library as a 3rd-party consumer."""

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def pineapple_deps(visibility = None):
    """Loads common dependencies needed to compile the pineapple library."""

    http_archive(
        name = "flags",
        sha256 = "5da3ddda4ccb87285c3855a08c77475948b25f55804da02dd1151a583f2cfb7b",
        strip_prefix = "cpp-flags-1.2",
        url = "https://github.com/tomocy/cpp-flags/archive/v1.2.zip",
    )
