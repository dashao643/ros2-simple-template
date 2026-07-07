from setuptools import find_packages, setup

package_name = 'py01_topic'

setup(
    name=package_name,
    version='0.0.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='dashao',
    maintainer_email='1127545598@qq.com',
    description='TODO: Package description',
    license='TODO: License declaration',
    extras_require={
        'test': [
            'pytest',
        ],
    },
    entry_points={
        'console_scripts': [
            'demo01_pub = py01_topic.demo01_pub:main',
            'demo02_sub = py01_topic.demo02_sub:main',
            'demo03_pub_stu = py01_topic.demo03_pub_stu:main',
            'demo04_sub_stu = py01_topic.demo04_sub_stu:main'
        ],
    },
)
