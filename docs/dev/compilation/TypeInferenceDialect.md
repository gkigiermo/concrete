<!-- Autogenerated by mlir-tblgen; don't manually edit -->
# 'TypeInference' Dialect

TypeInference dialect
A dialect for debugging type inference



## Operation definition

### `TypeInference.propagate_downward` (::mlir::concretelang::TypeInference::PropagateDownwardOp)

Causes the type of the operand to be forwarded upon type inference.


#### Operands:

| Operand | Description |
| :-----: | ----------- |
&laquo;unnamed&raquo; | any type

#### Results:

| Result | Description |
| :----: | ----------- |
&laquo;unnamed&raquo; | any type

### `TypeInference.propagate_upward` (::mlir::concretelang::TypeInference::PropagateUpwardOp)

Causes the type of the result to be forwarded to the operands upon type inference.


#### Operands:

| Operand | Description |
| :-----: | ----------- |
&laquo;unnamed&raquo; | any type

#### Results:

| Result | Description |
| :----: | ----------- |
&laquo;unnamed&raquo; | any type

### `TypeInference.unresolved_conflict` (::mlir::concretelang::TypeInference::UnresolvedConflictOp)

Represents an unresolved conflict


#### Operands:

| Operand | Description |
| :-----: | ----------- |
&laquo;unnamed&raquo; | any type

#### Results:

| Result | Description |
| :----: | ----------- |
&laquo;unnamed&raquo; | any type
