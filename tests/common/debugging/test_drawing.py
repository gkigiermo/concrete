"""Test file for drawing"""

import tempfile
from pathlib import Path

from concrete.common.data_types.integers import Integer
from concrete.common.debugging import draw_graph
from concrete.common.values import EncryptedScalar
from concrete.numpy.compile import compile_numpy_function_into_op_graph_and_measure_bounds


def test_draw_graph_with_saving(default_compilation_configuration):
    """Tests drawing and saving a graph"""

    def function(x):
        return x + 42

    op_graph = compile_numpy_function_into_op_graph_and_measure_bounds(
        function,
        {"x": EncryptedScalar(Integer(7, True))},
        range(-5, 5),
        default_compilation_configuration,
    )

    with tempfile.TemporaryDirectory() as tmp:
        output_directory = Path(tmp)
        output_file = output_directory.joinpath("test.png")
        draw_graph(op_graph, save_to=output_file)
        assert output_file.exists()
