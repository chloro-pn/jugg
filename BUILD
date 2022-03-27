# https://docs.bazel.build/versions/master/be/c-cpp.html#cc_binary
cc_library(
  name = "core",
  srcs = glob(["src/**/*.h", "src/**/*.cpp"]),
  copts = [],
)

cc_binary(
  name = "jugg",
  srcs = ["tools/jugg.cc"],
  includes = ["src/"],
  deps = [
    ":core",
  ]
)

cc_binary(
  name = "token_print",
  srcs = ["tools/token_print.cc"],
  includes = ["src/"],
  deps = [
    ":core",
  ]
)