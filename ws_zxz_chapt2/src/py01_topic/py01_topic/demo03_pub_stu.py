import rclpy
from rclpy.node import Node
from base_interfaces.msg import Student

class PubStu(Node):
    
    def __init__(self):
        super().__init__('minimal_publisher_py')
        self.publisher_ = self.create_publisher(Student, 'topic', 10)
        timer_period = 0.5 
        # 全局变量前加self
        self.stu = Student()
        self.stu.age = 23
        self.timer = self.create_timer(timer_period, self.timer_callback)

    def timer_callback(self):
        self.stu.name = "dashao"
        self.stu.height = 170.9
        self.stu.age += 1
        self.publisher_.publish(self.stu)


def main(args=None):
    rclpy.init(args=args)

    pub_node = PubStu()

    rclpy.spin(pub_node)

    rclpy.shutdown()


if __name__ == '__main__':
    main()