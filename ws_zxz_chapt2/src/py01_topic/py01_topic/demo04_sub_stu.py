import rclpy
from rclpy.node import Node
from base_interfaces.msg import Student

class SubStu(Node):

    def __init__(self):
        super().__init__('sub_stu')
        self.subscription = self.create_subscription(Student, 'topic', self.listener_callback, 10)

    def listener_callback(self, stu):
        self.get_logger().info('订阅的消息(py): name=%s,age=%d,height=%.2f' % (stu.name, stu.age, stu.height))


def main(args=None):
    rclpy.init(args=args)

    minimal_subscriber = SubStu()
    rclpy.spin(minimal_subscriber)

    rclpy.shutdown()


if __name__ == '__main__':
    main()