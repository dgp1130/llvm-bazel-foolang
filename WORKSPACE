# Simple example using Bazel to build a custom language implemented with LLVM.

WORKSPACE_NAME = "llvm_lang"

workspace(name = WORKSPACE_NAME)

# Load the LLVM repository.
load("//:llvm.bzl", "llvm")
llvm(
    name = "llvm",
    workspace_name = WORKSPACE_NAME,
)