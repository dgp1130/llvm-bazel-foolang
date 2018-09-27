#include <iostream>
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/raw_ostream.h"

/*
 * Uses LLVM to compile a simple "Hello World!" program and prints it to stdout as LLVM IR.
 */
int main(int argc, char* argv[]) {
    std::unique_ptr<llvm::LLVMContext> context = llvm::make_unique<llvm::LLVMContext>();
    llvm::IRBuilder<> builder(*context);
    std::unique_ptr<llvm::Module> module = llvm::make_unique<llvm::Module>("Module", *context);
    std::map<std::string, llvm::Value*> namedValues;

    // Declare printf as an extern.
    std::vector<llvm::Type*> printfArgs = {
        llvm::PointerType::getInt8PtrTy(*context) // string type
    };
    llvm::FunctionType* printfType = llvm::FunctionType::get(
        llvm::IntegerType::getInt32Ty(*context),
        printfArgs,
        false /* isVarArgs */
    );
    llvm::Function* printf = llvm::Function::Create(
        printfType,
        llvm::Function::ExternalLinkage,
        "printf",
        module.get()
    );

    // Create the main function.
    const auto mainType = llvm::FunctionType::get(
        llvm::IntegerType::getInt32Ty(*context),
        std::vector<llvm::Type*>(),
        false /* isVarArgs */
    );
    const auto main = llvm::Function::Create(
        mainType,
        llvm::Function::ExternalLinkage,
        "main",
        module.get()
    );

    const auto bb = llvm::BasicBlock::Create(*context, "entry", main);
    builder.SetInsertPoint(bb);

    // Read stdin line-by-line and prepend "foo: " to each.
    std::string content = "";
    for (std::string line; std::getline(std::cin, line); /* no op */) {
      content += "foo: " + line + "\n";
    }

    // Call printf() with the "Hello World!" string.
    llvm::Value* helloWorld = builder.CreateGlobalStringPtr(content, "globalstr");
    std::vector<llvm::Value*> args;
    args.push_back(helloWorld);
    builder.CreateCall(printf, args);

    llvm::APInt retVal(32 /* bitSize */, (uint32_t) 0, true /* signed */);
    builder.CreateRet(llvm::ConstantInt::get(*context, retVal));

    llvm::verifyFunction(*main);

    // Print the LLVM IR to stdout.
    module->print(llvm::outs(), nullptr);
}
