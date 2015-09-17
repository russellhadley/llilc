//===---- lib/Jit/JitPassManagerBuilder.cpp ---------------------------*- C++
//-*-===//
//
// LLILC
//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license.
// See LICENSE file in the project root for full license information.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Implementation of JIT pass manager builder.
///
//===----------------------------------------------------------------------===//

#include "earlyincludes.h"
#include "JitPassManagerBuilder.h"
#include "jitpch.h"
#include "LLILCJit.h"
#include "llvm/Analysis/BasicAliasAnalysis.h"
#include "llvm/Transforms/Scalar.h"

using namespace llvm;

// Construct a new JitPassManagerBuilder
JitPassManagerBuilder::JitPassManagerBuilder(LLILCJitContext &JC) {
  OptLevel = JC.Options->OptLevel;
  DoInsertStatepoints = JC.Options->DoInsertStatepoints;
}

// Destroy a JitPassManagerBuilder
JitPassManagerBuilder::~JitPassManagerBuilder() {}

void JitPassManagerBuilder::populatePassManager(
    llvm::legacy::PassManagerBase &PM) {

  // Add basic phases for optimizations
  if ((OptLevel == ::OptLevel::BLENDED_CODE) ||
      (OptLevel == ::OptLevel::FAST_CODE) ||
      (OptLevel == ::OptLevel::SMALL_CODE)) {
    // Provide basic AliasAnalysis support for GVN.
    PM.add(createBasicAAWrapperPass());
    // Promote memory to registers.
    PM.add(createPromoteMemoryToRegisterPass());
    // Do simple "peephole" optimizations and bit-twiddling optzns.
    PM.add(createInstructionCombiningPass());
    // Reassociate expressions.
    PM.add(createReassociatePass());
    // Eliminate Common SubExpressions.
    PM.add(createGVNPass());
    // Simplify the control flow graph (deleting unreachable blocks, etc).
    PM.add(createCFGSimplificationPass());
  }

  // Insert safepoint processing
  if (DoInsertStatepoints) {
    PM.add(createPlaceSafepointsPass());
    PM.add(createRewriteStatepointsForGCPass(false));
  }
}
