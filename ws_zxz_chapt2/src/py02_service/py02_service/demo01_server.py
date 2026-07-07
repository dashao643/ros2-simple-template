import rclpy
from rclpy.node import Node
from base_interfaces.srv import AddTwoInts

class MinimalService(Node):

    def __init__(self):
        super().__init__('minimal_service_py')
        self.srv = self.create_service(AddTwoInts, 'add_ints', self.add_two_ints_callback)
        self.get_logger().info("服务端启动！")

    def add_two_ints_callback(self, request, response):
        response.sum = request.num1 + request.num2
        self.get_logger().info('请求数据:(%d,%d),响应结果:%d' % (request.num1, request.num2, response.sum))
        return response


def main():
    rclpy.init()

    minimal_service = MinimalService()
    rclpy.spin(minimal_service)

    rclpy.shutdown()


if __name__ == '__main__':
    main()