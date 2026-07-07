import rclpy
from rclpy.node import Node
from rclpy.logging import get_logger
from base_interfaces.srv import AddTwoInts
import sys

class MinimalClient(Node):

    def __init__(self):
        super().__init__('minimal_client_py')
        self.get_logger().info("客户端已创建")

        self.client = self.create_client(AddTwoInts, 'add_ints')
        
        while not self.client.wait_for_service(timeout_sec=1.0):
            self.get_logger().info('服务连接中，请稍候...')
            
        self.req = AddTwoInts.Request()

    def send_request(self):
        # sys获取参数
        self.req.num1 = int(sys.argv[1])
        self.req.num2 = int(sys.argv[2])
        
        self.future = self.client.call_async(self.req)


def main():
    if len(sys.argv) != 3:
        get_logger("rclpy").error("请输入两个整型参数")
        return
          
    rclpy.init()

    client_node = MinimalClient()

    client_node.send_request()

    rclpy.spin_until_future_complete(client_node, client_node.future)

    try:
        response = client_node.future.result()
        client_node.get_logger().info(f"响应结果:{client_node.req.num1}+{client_node.req.num2}={response.sum}") # type: ignore
    except Exception as e:
        client_node.get_logger().info(
            '服务请求失败： %r' % (e,))


    rclpy.shutdown()


if __name__ == '__main__':
    main()