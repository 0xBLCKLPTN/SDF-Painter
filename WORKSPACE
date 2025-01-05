load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "rules_cc",
    urls = ["https://github.com/bazelbuild/rules_cc/releases/download/0.0.17/rules_cc-0.0.17.tar.gz"],
    sha256 = "abc605dd850f813bb37004b77db20106a19311a96b2da1c92b789da529d28fe1",
    strip_prefix = "rules_cc-0.0.17",
)

# Load the correct symbols from rules_cc
load("@rules_cc//cc:repositories.bzl", "cc_repositories", "cc_repositories_register_toolchains")

cc_repositories()

cc_repositories_register_toolchains()

http_archive(
    name = "glfw",
    sha256 = "bbd2c42c660b725e9755eb417e40b373f0d4c03138c9b2e210d02cd308bd99cd",  # Replace with the actual SHA256 hash
    urls = ["https://github.com/glfw/glfw/releases/download/3.3.4/glfw-3.3.4.zip"],
)

http_archive(
    name = "glew",
    sha256 = "04de91e7e6763039bc11940095cd9c7f880baba82196a7765f727ac05a993c95",  # Replace with the actual SHA256 hash
    urls = ["https://github.com/nigels-com/glew/releases/download/glew-2.1.0/glew-2.1.0.tgz"],
)

http_archive(
    name = "opencl_headers",
    sha256 = "477e2b26125d99a9b2f20c68262f27ca3f3ca7899593a8af2b7fe077bdce18d1",  # Replace with the actual SHA256 hash
    urls = ["https://github.com/KhronosGroup/OpenCL-Headers/archive/refs/tags/v2021.04.29.tar.gz"],
)
