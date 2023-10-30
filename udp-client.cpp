/*
 *  使用asio实现带有Udp的客户端应用程序
 *
 *  客户端
 *
*/

#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

int main(int argc, char* argv[])
{
    try
    {
        if (argc != 2)
        {
            std::cerr << "Usage: client <host>" << std::endl;
            return 1;
        }

        boost::asio::io_context io_context;

        udp::resolver resolver(io_context);

        // endpoint : ip + port
        udp::endpoint receiver_endpoint = *resolver.resolve(udp::v4(), argv[1], "daytime").begin();

        // 由于UDP是面向数据报的，我们将不使用流套接字
        udp::socket socket(io_context);
        socket.open(udp::v4());

        boost::array<char, 1> send_buf  = {{0}};
        socket.send_to(boost::asio::buffer(send_buf), receiver_endpoint);

        // 现在我们需要准备好接受服务器发回给我们的任何信息
        boost::array<char, 128> recv_buf;
        udp::endpoint sender_endpoint;
        size_t len = socket.receive_from(
            boost::asio::buffer(recv_buf), sender_endpoint);

        std::cout << "got message from UDP server: " << std::endl;

        std::cout.write(recv_buf.data(), len);
    }

    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}














