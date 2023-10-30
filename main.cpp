/*
 * 使计时器每秒触发一次以及如何将附加参数传递给处理程序函数。
 *
 * 要使用asio实现重复计时器，您需要在完成处理程序中更改计时器的过期时间，然后启动新的异步等待
*/

#include <iostream>
#include <boost/asio.hpp>// 引入头文件
#include <boost/bind.hpp>

// 当异步等待结束时调用，添加参数
// 在第count触发时，停止程序
void print(const boost::system::error_code &/*e*/, boost::asio::steady_timer* t, int* count)
{
    // 执行6次
    if(*count < 5)
    {
        std::cout << *count << std::endl;
        ++(*count);

        // 每隔1s，失效时间加1s
        t->expires_at(t->expiry() + boost::asio::chrono::seconds(1));

        // 使用error占位符，绑定处理函数
        t->async_wait(boost::bind(print, boost::asio::placeholders::error, t, count));
    }
}


int main()
{
    boost::asio::io_context io;

    int count = 0;

    boost::asio::steady_timer t(io, boost::asio::chrono::seconds(1));

    // 异步等待，调用绑定处理函数，并传入参数
    t.async_wait(boost::bind(print, boost::asio::placeholders::error, &t, &count));

    // 最后，必须在io context对象上调用io context::run()成员函数。
    io.run();

    std::cout << "Final count is " << count << std::endl;

    return 0;
}

