/*
 *  使用asio实现带有Tcp的服务器端应用程序
 *
 *  服务器端
 *
*/

#include <iostream>
#include <ctime>
#include <string>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

// 我们定义了make_daytime_string()函数来创建要发送回客户机的字符串
// 该函数将在我们所有的日间服务器应用程序中重用
std::string make_daytime_string()
{
    using namespace std; // For time_t, time and ctime;
    time_t now = time(0);
    return ctime(&now);
}

int main()
{
    std::cout << "begin to listen" << std::endl;
    try
    {
        boost::asio::io_context io_context;

        // 需要创建一个ip::tcp::acceptor对象来监听新的连接
        // 对于IP版本4，它被初始化为监听TCP端口13
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 13));

        // 这是一个迭代服务器，这意味着它将一次处理一个连接
        // 创建一个套接字，它将表示与客户端的连接，然后等待连接
        for (;;)
        {
            tcp::socket socket(io_context);
            acceptor.accept(socket);

            std::cout << "accepted one client" << std::endl;
            // 客户端正在访问我们的服务。确定当前时间并将此信息传输到客户端
            std::string message = make_daytime_string(); // 获得当前时间

            boost::system::error_code ignored_error;
            boost::asio::write(socket, boost::asio::buffer(message), ignored_error);
        }
    }

    // 最后，处理任何异常

    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
















