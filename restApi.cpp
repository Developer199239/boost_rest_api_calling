#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/beast.hpp>

namespace asio = boost::asio;
namespace beast = boost::beast;
using tcp = boost::asio::ip::tcp;

class NetworkManager {
public:
    NetworkManager(const std::string& baseUrl) : baseUrl_(baseUrl) {}

    std::string performHttpGet(const std::string& path) {
        try {
            asio::io_context io_context;
            tcp::resolver resolver(io_context);
            beast::tcp_stream stream(io_context);
            auto const results = resolver.resolve(baseUrl_, "http");
            stream.connect(results);

            beast::http::request<beast::http::string_body> req{beast::http::verb::get, path, 11};
            req.set(beast::http::field::host, baseUrl_);
            req.set(beast::http::field::user_agent, "Boost HTTP Client");
            beast::http::write(stream, req);

            beast::flat_buffer buffer;
            beast::http::response<beast::http::dynamic_body> res;
            beast::http::read(stream, buffer, res);

            return beast::buffers_to_string(res.body().data());
        } catch (std::exception const& e) {
            throw std::runtime_error("HTTP request error: " + std::string(e.what()));
        }
    }

    std::string performHttpPost(const std::string& path, const std::string& requestBody) {
        try {
            asio::io_context io_context;
            tcp::resolver resolver(io_context);
            beast::tcp_stream stream(io_context);
            auto const results = resolver.resolve(baseUrl_, "http");
            stream.connect(results);

            beast::http::request<beast::http::string_body> req{beast::http::verb::post, path, 11};
            req.set(beast::http::field::host, baseUrl_);
            req.set(beast::http::field::user_agent, "Boost HTTP Client");
            req.set(beast::http::field::content_type, "application/json; charset=UTF-8");
            req.body() = requestBody;

            req.prepare_payload();
            beast::http::write(stream, req);

            beast::flat_buffer buffer;
            beast::http::response<beast::http::dynamic_body> res;
            beast::http::read(stream, buffer, res);

            return beast::buffers_to_string(res.body().data());
        } catch (std::exception const& e) {
            throw std::runtime_error("HTTP request error: " + std::string(e.what()));
        }
    }

private:
    std::string baseUrl_;
};

int main() {
    //https://jsonplaceholder.typicode.com/guide/
    
    std::string baseUrl = "jsonplaceholder.typicode.com";
    std::string path = "/todos/1";

    try {
        NetworkManager networkManager(baseUrl);


        std::string response = networkManager.performHttpGet(path);
        std::cout << "Get Response:\n" << response << std::endl;


        std::cout << "Perform POST request" << std::endl;
            // Perform POST request
            std::string postRequestBody = R"({
            "title": "foo",
            "body": "bar",
            "userId": 1
        })";
        std::string postResponse = networkManager.performHttpPost("/posts", postRequestBody);

        std::cout << "Post Response:\n" << postResponse << std::endl;
    } catch (std::exception const& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
