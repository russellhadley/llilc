//===--------------- include/Jit/JitPassManagerBuilder.h --------*- C++ -*-===//
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
/// \brief Declaration of the JIT pass manager builder object.
///
//===----------------------------------------------------------------------===//

#ifndef JITPASSMANAGERBUILDER_H
#define JITPASSMANAGERBUILDER_H

#include "llvm/IR/LegacyPassManager.h"
#include "Reader/options.h"

struct LLILCJitContext;

/// \brief Memory manager for LLILC
///
/// This class extends \p RTDyldMemoryManager to obtain memory from the
/// CoreCLR's EE for the persistent jit outputs (code, data, and unwind
/// information). Each jit request instantiates its own memory manager.
class JitPassManagerBuilder {

public:
  ::OptLevel OptLevel;

  bool DoInsertStatepoints;

public:
  /// Construct a new \p JitPassManagerBuilder
  JitPassManagerBuilder(LLILCJitContext &C);

  /// Destroy an \p JitPassManagerBuilder
  ~JitPassManagerBuilder();

  void populatePassManager(llvm::legacy::PassManagerBase &PM);
};

#endif // JITPASSMANAGERBUILDER_H
