/*
 * 同步多线程程序中的完成处理程序，并行运行两个计时器
 *
*/

#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/thread/thread.hpp>


class Printer {
public:
    // 构造函数
    Printer(boost::asio::io_context& io)
        :m_strand(boost::asio::make_strand(io)), m_timer1(io, boost::asio::chrono::seconds(1)),
        m_timer2(io, boost::asio::chrono::seconds(1)), m_count(0)
    {
        // 绑定
        m_timer1.async_wait(boost::asio::bind_executor(m_strand, boost::bind(&Printer::print1, this)));
        m_timer2.async_wait(boost::asio::bind_executor(m_strand, boost::bind(&Printer::print2, this)));

    }

    // 析构函数
    ~Printer()
    {
        std::cout << "Final count is " << m_count << std::endl;
    }

    void print1()
    {
        if(m_count < 10)
        {
            std::cout << "Timer1: " << m_count << std::endl;
            ++m_count;

            m_timer1.expires_at(m_timer1.expiry() + boost::asio::chrono::seconds(1));
            m_timer1.async_wait(boost::asio::bind_executor(m_strand,
                                                           boost::bind(&Printer::print1, this)));
        }
    }

    void print2()
    {
        if(m_count < 10)
        {
            std::cout << "Timer2: " << m_count << std::endl;
            ++m_count;

            m_timer2.expires_at(m_timer2.expiry() + boost::asio::chrono::seconds(1));
            m_timer2.async_wait(boost::asio::bind_executor(m_strand,
                                                           boost::bind(&Printer::print2, this)));
        }
    }

private:
    boost::asio::strand<boost::asio::io_context::executor_type> m_strand;
    boost::asio::steady_timer m_timer1;
    boost::asio::steady_timer m_timer2;
    int m_count;
};



int main()
{
    boost::asio::io_context io;

    Printer p(io);

    boost::thread t(boost::bind(&boost::asio::io_context::run, &io));

    io.run();

    t.join();

    return 0;
}

