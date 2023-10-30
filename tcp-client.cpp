/*
 *  使用asio实现带有Tcp的客户端应用程序
 *
 *  客户端
 *
 *  ./client 127.0.0.1
 *
*/

#include <iostream>
#include <boost/asio.hpp>
#include <boost/array.hpp>

using boost::asio::ip::tcp;

int main(int argc, char* argv[])
{
    try
    {
        if (argc != 2)
        {
            std::cerr << "Usage: client <host>" << std::endl;
            return 1;
        }


        // 所有使用asio的程序都需要至少有一个I/O执行上下文
        boost::asio::io_context io_context;

        // 我们需要将指定为应用程序参数的服务器名转换为TCP端点。为此，我们使用ip::tcp::resolver对象。
        tcp::resolver resolver(io_context);

        // 解析器接受主机名和服务名，并将它们转换为端点列表
        tcp::resolver::results_type endpoints = resolver.resolve(argv[1], "daytime");

        // 创建并连接套接字
        tcp::socket socket(io_context);
        boost::asio::connect(socket, endpoints);

        // 连接已打开。现在我们需要做的就是读取来自日间服务的响应
        // 我们使用boost::数组保存接收到的数据
        for (;;)
        {
            boost::array<char, 128> buf;
            boost::system::error_code error;

            size_t len = socket.read_some(boost::asio::buffer(buf), error);

            if (error == boost::asio::error::eof)
                break; // Connection closed cleanly by peer.
            else if (error)
                throw boost::system::system_error(error); // Some other error.

            std::cout << "client got message from server: " << std::endl;
            std::cout.write(buf.data(), len);
        }

    }

    // 最后，处理可能已经抛出的任何异常
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}




















