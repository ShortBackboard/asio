/*
 * 异步使用计时器等待5s，当异步等待结束时调用自己定义的一个名为print的函数
*/

#include <iostream>
#include <boost/asio.hpp>// 引入头文件

// 当异步等待结束时调用
void print(const boost::system::error_code &/*e*/)
{
    std::cout << "hello, world!" << std::endl;
}


int main()
{
    boost::asio::io_context io;
    boost::asio::steady_timer t(io, boost::asio::chrono::seconds(5));

    // 异步等待，绑定函数print
    t.async_wait(&print);

    // 最后，必须在io context对象上调用io context::run()成员函数。
    io.run();

    return 0;
}

