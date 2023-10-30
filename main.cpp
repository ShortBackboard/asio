/*
 * 使用类成员函数作为完成处理程序函数，使计时器每秒触发一次
 *
 * 要使用asio实现重复计时器，您需要在完成处理程序中更改计时器的过期时间，然后启动新的异步等待
*/

#include <iostream>
#include <boost/asio.hpp>// 引入头文件
#include <boost/bind/bind.hpp>


class Printer {
public:
    // 构造函数
    Printer(boost::asio::io_context& io)
        :m_timer(io, boost::asio::chrono::seconds(1)), m_count(0)
    {
        // 绑定
        m_timer.async_wait(boost::bind(&Printer::print, this));
    }

    // 析构函数
    ~Printer()
    {
        std::cout << "Final count is " << m_count << std::endl;
    }

    void print()
    {
        if(m_count < 5)
        {
            std::cout << m_count << std::endl;
            ++m_count;

            m_timer.expires_at(m_timer.expiry() + boost::asio::chrono::seconds(1));
            m_timer.async_wait(boost::bind(&Printer::print, this));
        }
    }

private:
    boost::asio::steady_timer m_timer;
    int m_count;
};



int main()
{
    boost::asio::io_context io;

    Printer p(io);

    // 最后，必须在io context对象上调用io context::run()成员函数。
    io.run();

    return 0;
}

