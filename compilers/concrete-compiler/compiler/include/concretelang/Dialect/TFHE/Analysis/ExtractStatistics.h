// Part of the Concrete Compiler Project, under the BSD3 License with Zama
// Exceptions. See
// https://github.com/zama-ai/concrete-compiler-internal/blob/main/LICENSE.txt
// for license information.

#ifndef CONCRETELANG_DIALECT_TFHE_ANALYSIS_EXTRACT_STATISTICS_H
#define CONCRETELANG_DIALECT_TFHE_ANALYSIS_EXTRACT_STATISTICS_H

#include <mlir/IR/BuiltinOps.h>
#include <mlir/IR/Operation.h>
#include <mlir/Pass/Pass.h>

#include <concretelang/Support/CompilationFeedback.h>

namespace mlir {
namespace concretelang {
namespace TFHE {

struct ExtractTFHEStatisticsPass
    : public PassWrapper<ExtractTFHEStatisticsPass, OperationPass<ModuleOp>> {

  CompilationFeedback &feedback;

  ExtractTFHEStatisticsPass(CompilationFeedback &feedback)
      : feedback{feedback} {};

  void runOnOperation() override {
    WalkResult walk =
        getOperation()->walk([&](Operation *op, const WalkStage &stage) {
          if (stage.isBeforeAllRegions()) {
            std::optional<StringError> error = this->enter(op);
            if (error.has_value()) {
              op->emitError() << error->mesg;
              return WalkResult::interrupt();
            }
          }

          if (stage.isAfterAllRegions()) {
            std::optional<StringError> error = this->exit(op);
            if (error.has_value()) {
              op->emitError() << error->mesg;
              return WalkResult::interrupt();
            }
          }

          return WalkResult::advance();
        });

    if (walk.wasInterrupted()) {
      signalPassFailure();
    }
  }

  std::optional<StringError> enter(Operation *op);

  std::optional<StringError> exit(Operation *op);

  size_t iterations = 1;
};

} // namespace TFHE
} // namespace concretelang
} // namespace mlir

#endif