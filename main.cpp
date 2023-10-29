/*
 * 同步使用计时器等待5s
*/

#include <iostream>

// 引入头文件
#include <boost/asio.hpp>

int main()
{
    boost::asio::io_context io;
    boost::asio::steady_timer t(io, boost::asio::chrono::seconds(5));

    //
    t.wait();
    std::cout << "hello, asio!" << std::endl;

    return 0;
}

