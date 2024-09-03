import onnx
import onnx.helper as helper

input0 = helper.make_tensor_value_info('Mat0', onnx.TensorProto.UINT32, [32, 32])
input1 = helper.make_tensor_value_info('Mat1', onnx.TensorProto.UINT32, [32, 32])

output0 = helper.make_tensor_value_info('MatRes0', onnx.TensorProto.UINT32, [32, 32])

node0 = helper.make_node(
        'Add',
        ['Mat0', 'Mat1'],
        ['MatRes0']
)

graph = helper.make_graph(
    [node0],
    'MatMatAddTest',
    [input0, input1],
    [output0]
)

model = helper.make_model(graph, producer_name='XPUPROJECT')

onnx.save(model, 'MatMatAddTest.onnx')
