#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <mutex>
#include <string>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

namespace asio = boost::asio;
namespace beast = boost::beast;
using tcp = boost::asio::ip::tcp;
namespace pt = boost::property_tree;


// Define a C++ model class that mirrors the JSON structure
struct MyModel {
    std::string title;
    std::string body;
    int userId;
    int id;
};

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
       std::lock_guard<std::mutex> lock(mutex_); // Lock the mutex
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
    std::mutex mutex_; 
};

int main() {
    //https://jsonplaceholder.typicode.com/guide/
    
    std::string baseUrl = "jsonplaceholder.typicode.com";
    std::string path = "/todos/1";

    // try {
    //     NetworkManager networkManager(baseUrl);


    //     std::string response = networkManager.performHttpGet(path);
    //     std::cout << "Get Response:\n" << response << std::endl;


    //     std::cout << "Perform POST request" << std::endl;
    //         // Perform POST request
    //         std::string postRequestBody = R"({
    //         "title": "foo",
    //         "body": "bar",
    //         "userId": 1
    //     })";
    //     std::string postResponse = networkManager.performHttpPost("/posts", postRequestBody);

    //     std::cout << "Post Response:\n" << postResponse << std::endl;
    // } catch (std::exception const& e) {
    //     std::cerr << "Error: " << e.what() << std::endl;
    //     return 1;
    // }


    try {
        NetworkManager networkManager(baseUrl);

        #pragma omp parallel for
        for (int i = 0; i < 1; ++i) {
            std::string postRequestBody = R"({
                "title": "foo",
                "body": "bar",
                "userId": 1
            })";
            std::string response = networkManager.performHttpPost("/posts", postRequestBody);

            #pragma omp critical
            {
                std::cout << "Response from iteration " << i << ":\n" << response << std::endl;

                // Create an empty property tree
                boost::property_tree::ptree pt;

                // Load the JSON data into the property tree
                std::istringstream jsonStream(response);
                boost::property_tree::read_json(jsonStream, pt);

                // Check if "title" key exists
                boost::optional<std::string> title1 = pt.get_optional<std::string>("title");
                
                if (title1) {
                    std::cout << "Title exists: " << *title1 << std::endl;
                } else {
                    std::cout << "Title does not exist." << std::endl;
                }

                // Access individual elements in the JSON
                std::string title = pt.get<std::string>("title");
                std::string body = pt.get<std::string>("body");
                int userId = pt.get<int>("userId");
                int id = pt.get<int>("id");

                // Print the parsed values
                std::cout << "Title: " << title << std::endl;
                std::cout << "Body: " << body << std::endl;
                std::cout << "User ID: " << userId << std::endl;
                std::cout << "ID: " << id << std::endl;

                // another way
                std::cout << "=====another way====" << std::endl;
                pt::ptree jsonTree;
    
                std::istringstream jsonStream2(response);
                pt::read_json(jsonStream2, jsonTree);

                // Create an instance of the model class and populate it from the property tree
                MyModel model;
                model.title = jsonTree.get<std::string>("title");
                model.body = jsonTree.get<std::string>("body");
                model.userId = jsonTree.get<int>("userId");
                model.id = jsonTree.get<int>("id");

                // Access and print the model's attributes
                std::cout << "Title: " << model.title << std::endl;
                std::cout << "Body: " << model.body << std::endl;
                std::cout << "User ID: " << model.userId << std::endl;
                std::cout << "ID: " << model.id << std::endl;

            }
        }
    } catch (std::exception const& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
