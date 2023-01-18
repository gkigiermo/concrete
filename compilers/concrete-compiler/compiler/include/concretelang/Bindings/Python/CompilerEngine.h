// Part of the Concrete Compiler Project, under the BSD3 License with Zama
// Exceptions. See
// https://github.com/zama-ai/concrete-compiler-internal/blob/main/LICENSE.txt
// for license information.

#ifndef CONCRETELANG_BINDINGS_PYTHON_COMPILER_ENGINE_H
#define CONCRETELANG_BINDINGS_PYTHON_COMPILER_ENGINE_H

#include "concretelang/Support/CompilerEngine.h"
#include "concretelang/Support/JITSupport.h"
#include "concretelang/Support/Jit.h"
#include "concretelang/Support/LibrarySupport.h"
#include "mlir-c/IR.h"

/// MLIR_CAPI_EXPORTED is used here throughout the API, because of the way the
/// python extension is built using MLIR cmake functions, which will cause
/// undefined symbols during runtime if those aren't present.

/// Wrapper of the mlir::concretelang::LambdaArgument
struct lambdaArgument {
  std::shared_ptr<mlir::concretelang::LambdaArgument> ptr;
};
typedef struct lambdaArgument lambdaArgument;

/// Hold a list of lambdaArgument to represent execution arguments
struct executionArguments {
  lambdaArgument *data;
  size_t size;
};
typedef struct executionArguments executionArguments;

// JIT Support bindings ///////////////////////////////////////////////////////

struct JITSupport_Py {
  mlir::concretelang::JITSupport support;
};
typedef struct JITSupport_Py JITSupport_Py;

MLIR_CAPI_EXPORTED JITSupport_Py jit_support(std::string runtimeLibPath);

MLIR_CAPI_EXPORTED std::unique_ptr<mlir::concretelang::JitCompilationResult>
jit_compile(JITSupport_Py support, const char *module,
            mlir::concretelang::CompilationOptions options);

MLIR_CAPI_EXPORTED mlir::concretelang::ClientParameters
jit_load_client_parameters(JITSupport_Py support,
                           mlir::concretelang::JitCompilationResult &);

MLIR_CAPI_EXPORTED mlir::concretelang::CompilationFeedback
jit_load_compilation_feedback(JITSupport_Py support,
                              mlir::concretelang::JitCompilationResult &);

MLIR_CAPI_EXPORTED std::shared_ptr<mlir::concretelang::JITLambda>
jit_load_server_lambda(JITSupport_Py support,
                       mlir::concretelang::JitCompilationResult &);

MLIR_CAPI_EXPORTED std::unique_ptr<concretelang::clientlib::PublicResult>
jit_server_call(JITSupport_Py support, mlir::concretelang::JITLambda &lambda,
                concretelang::clientlib::PublicArguments &args,
                concretelang::clientlib::EvaluationKeys &evaluationKeys);

// Library Support bindings ///////////////////////////////////////////////////

struct LibrarySupport_Py {
  mlir::concretelang::LibrarySupport support;
};
typedef struct LibrarySupport_Py LibrarySupport_Py;

MLIR_CAPI_EXPORTED LibrarySupport_Py
library_support(const char *outputPath, const char *runtimeLibraryPath,
                bool generateSharedLib, bool generateStaticLib,
                bool generateClientParameters, bool generateCompilationFeedback,
                bool generateCppHeader);

MLIR_CAPI_EXPORTED std::unique_ptr<mlir::concretelang::LibraryCompilationResult>
library_compile(LibrarySupport_Py support, const char *module,
                mlir::concretelang::CompilationOptions options);

MLIR_CAPI_EXPORTED mlir::concretelang::ClientParameters
library_load_client_parameters(LibrarySupport_Py support,
                               mlir::concretelang::LibraryCompilationResult &);

MLIR_CAPI_EXPORTED mlir::concretelang::CompilationFeedback
library_load_compilation_feedback(
    LibrarySupport_Py support, mlir::concretelang::LibraryCompilationResult &);

MLIR_CAPI_EXPORTED concretelang::serverlib::ServerLambda
library_load_server_lambda(LibrarySupport_Py support,
                           mlir::concretelang::LibraryCompilationResult &);

MLIR_CAPI_EXPORTED std::unique_ptr<concretelang::clientlib::PublicResult>
library_server_call(LibrarySupport_Py support,
                    concretelang::serverlib::ServerLambda lambda,
                    concretelang::clientlib::PublicArguments &args,
                    concretelang::clientlib::EvaluationKeys &evaluationKeys);

MLIR_CAPI_EXPORTED std::string
library_get_shared_lib_path(LibrarySupport_Py support);

MLIR_CAPI_EXPORTED std::string
library_get_client_parameters_path(LibrarySupport_Py support);

// Client Support bindings ///////////////////////////////////////////////////

MLIR_CAPI_EXPORTED std::unique_ptr<concretelang::clientlib::KeySet>
key_set(concretelang::clientlib::ClientParameters clientParameters,
        std::optional<concretelang::clientlib::KeySetCache> cache);

MLIR_CAPI_EXPORTED std::unique_ptr<concretelang::clientlib::PublicArguments>
encrypt_arguments(concretelang::clientlib::ClientParameters clientParameters,
                  concretelang::clientlib::KeySet &keySet,
                  llvm::ArrayRef<mlir::concretelang::LambdaArgument *> args);

MLIR_CAPI_EXPORTED lambdaArgument
decrypt_result(concretelang::clientlib::KeySet &keySet,
               concretelang::clientlib::PublicResult &publicResult);

// Serialization ////////////////////////////////////////////////////////////

MLIR_CAPI_EXPORTED mlir::concretelang::ClientParameters
clientParametersUnserialize(const std::string &json);

MLIR_CAPI_EXPORTED std::string
clientParametersSerialize(mlir::concretelang::ClientParameters &params);

MLIR_CAPI_EXPORTED std::unique_ptr<concretelang::clientlib::PublicArguments>
publicArgumentsUnserialize(
    mlir::concretelang::ClientParameters &clientParameters,
    const std::string &buffer);

MLIR_CAPI_EXPORTED std::string publicArgumentsSerialize(
    concretelang::clientlib::PublicArguments &publicArguments);

MLIR_CAPI_EXPORTED std::unique_ptr<concretelang::clientlib::PublicResult>
publicResultUnserialize(mlir::concretelang::ClientParameters &clientParameters,
                        const std::string &buffer);

MLIR_CAPI_EXPORTED std::string
publicResultSerialize(concretelang::clientlib::PublicResult &publicResult);

MLIR_CAPI_EXPORTED concretelang::clientlib::EvaluationKeys
evaluationKeysUnserialize(const std::string &buffer);

MLIR_CAPI_EXPORTED std::string evaluationKeysSerialize(
    concretelang::clientlib::EvaluationKeys &evaluationKeys);

/// Parse then print a textual representation of an MLIR module
MLIR_CAPI_EXPORTED std::string roundTrip(const char *module);

/// Terminate/Init dataflow parallelization
MLIR_CAPI_EXPORTED void terminateDataflowParallelization();
MLIR_CAPI_EXPORTED void initDataflowParallelization();

/// Create a lambdaArgument from a tensor of different data types
MLIR_CAPI_EXPORTED lambdaArgument lambdaArgumentFromTensorU8(
    std::vector<uint8_t> data, std::vector<int64_t> dimensions);
MLIR_CAPI_EXPORTED lambdaArgument lambdaArgumentFromTensorU16(
    std::vector<uint16_t> data, std::vector<int64_t> dimensions);
MLIR_CAPI_EXPORTED lambdaArgument lambdaArgumentFromTensorU32(
    std::vector<uint32_t> data, std::vector<int64_t> dimensions);
MLIR_CAPI_EXPORTED lambdaArgument lambdaArgumentFromTensorU64(
    std::vector<uint64_t> data, std::vector<int64_t> dimensions);
/// Create a lambdaArgument from a scalar
MLIR_CAPI_EXPORTED lambdaArgument lambdaArgumentFromScalar(uint64_t scalar);
/// Check if a lambdaArgument holds a tensor
MLIR_CAPI_EXPORTED bool lambdaArgumentIsTensor(lambdaArgument &lambda_arg);
/// Get tensor data from lambdaArgument
MLIR_CAPI_EXPORTED std::vector<uint64_t>
lambdaArgumentGetTensorData(lambdaArgument &lambda_arg);
/// Get tensor dimensions from lambdaArgument
MLIR_CAPI_EXPORTED std::vector<int64_t>
lambdaArgumentGetTensorDimensions(lambdaArgument &lambda_arg);
/// Check if a lambdaArgument holds a scalar
MLIR_CAPI_EXPORTED bool lambdaArgumentIsScalar(lambdaArgument &lambda_arg);
/// Get scalar value from lambdaArgument
MLIR_CAPI_EXPORTED uint64_t lambdaArgumentGetScalar(lambdaArgument &lambda_arg);

/// Compile the textual representation of MLIR modules to a library.
MLIR_CAPI_EXPORTED std::string library(std::string libraryPath,
                                       std::vector<std::string> modules);

#endif // CONCRETELANG_BINDINGS_PYTHON_COMPILER_ENGINE_H