load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "gtest",
    sha256 = "94c634d499558a76fa649edb13721dce6e98fb1e7018dfaeba3cd7a083945e91",
    strip_prefix = "googletest-release-1.10.0",
    url = "https://github.com/google/googletest/archive/release-1.10.0.zip",
)

http_archive(
    name = "flags",
    build_file = "//src:BUILD",
    sha256 = "236be26dd0a0455c8c408315865f777ea3e6fe339edaca4e29769e513d47b2dd",
    strip_prefix = "cpp-flags-1.0",
    url = "https://github.com/tomocy/cpp-flags/archive/v1.0.zip",
)
