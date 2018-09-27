# Bazel LLVM Example

This is an example project showing how it is possible to build an LLVM-based language with Bazel.

The LLVM pre-built binaries are downloaded and exposed at the `@llvm` Bazel target. This dependency can be used for any target which needs to link against LLVM.

`compiler.cpp` is a simple compiler built using LLVM which outputs a program which prints the original source code with "foo: " prepended to each line.

The `llvm.bzl` file contains all the build definitions for downloading and linking against LLVM. It also exposes an `llc` macro, which is used to transpile the LLVM IR into assembly. That assembly is then put into a `cc_binary()` which allows it to be assembled into an executable binary. The entire process is wrapped in the `foo_binary()` macro, which compiles a source file written in `foo` all the way to an executable binary. The example can be run with:

```sh
bazel run //:hello
```

This example exposes the ficticious foo language as a build step in Bazel and can be expanded upon. For example, tests could be written asserting the output of various input `foo` code. Another example would linking `foo` code against against other `cc_library()` rules as dependencies.

I would love to make this a proper module, something you can just add to your WORKSPACE and get a dependcy on `@llvm`, unfortunately I don't see a good way of doing that. Instead, I think the best way of using this is to just hard copy `llvm.bzl` and use this project as an example of how to use it.

## Version

Currently, I've only tested this on LLVM 3.9.1, but this can be overridden with any version from [releases.llvm.org](https://releases.llvm.org) using the `version = "X.X.X"` attribute on the `llvm` repository rule in the WORKSPACE file. You're mileage with any given version may vary. If you manage to get one working, don't hesitate to submit a pull request!

## Architecture

Currently, this downloads the pre-built 64-bit Ubuntu 16.04 version of the binaries, which limits the platform. LLVM is pretty portable, so this could probably be expanded to pick the right architecture based on context.
