#include "mlir/IR/Attributes.h"
#include "mlir/IR/Builders.h"
#include "mlir/IR/BuiltinOps.h"
#include "mlir/IR/BuiltinTypes.h"
#include "mlir/IR/Region.h"
#include "mlir/IR/TypeUtilities.h"

#include "zamalang/Dialect/RT/IR/RTOps.h"
#include "zamalang/Dialect/RT/IR/RTTypes.h"

#define GET_OP_CLASSES
#include "zamalang/Dialect/RT/IR/RTOps.cpp.inc"

using namespace mlir::zamalang::RT;

void DataflowTaskOp::build(
    ::mlir::OpBuilder &builder, ::mlir::OperationState &result,
    ::mlir::TypeRange resultTypes, ::mlir::ValueRange operands,
    ::llvm::ArrayRef<::mlir::NamedAttribute> attributes) {
  result.addOperands(operands);
  result.addAttributes(attributes);
  Region *reg = result.addRegion();
  Block *body = new Block();
  reg->push_back(body);
  result.addTypes(resultTypes);
}
