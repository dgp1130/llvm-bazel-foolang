load("//:foo.bzl", "foo_binary")

# Build the compiler.
cc_binary(
    name = "compiler",
    srcs = ["compiler.cpp"],
    deps = ["@llvm"],
)

# Run the custom compiler on a source file.
foo_binary(
    name = "hello",
    src = "hello.foo",
)
