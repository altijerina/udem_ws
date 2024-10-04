#include "rclcpp/rclcpp.hpp"
#include "example_interfaces/srv/add_two_ints.hpp"

class NameOfNode : public rclcpp::Node
{
public:
    NameOfNode() : Node("name_of_node")
    {
        threads_.push_back(std::thread(std::bind(&NameOfNode::callNameOfService, this, 1, 4)));
        threads_.push_back(std::thread(std::bind(&NameOfNode::callNameOfService, this, 4, 5)));
    }

    void callNameOfService(int a, int b)
    {
            auto client = this->create_client<example_interfaces::srv::AddTwoInts>("name_of_node");
            while (!client->wait_for_service(std::chrono::seconds(1)))
            {
                RCLCPP_WARN(this->get_logger(), "Waiting for the server to be up..." );
            }

            auto request = std::make_shared<example_interfaces::srv::AddTwoInts::Request>();
            request->a = a;
            request->b = b;    

            auto future = client->async_send_request(request);        

            
            try{
                auto response = future.get();
                RCLCPP_INFO(this->get_logger(), "%d + %d = %d", int(a), int(b), int(response->sum));
            }
            catch(const std::exception &e)
            {
                RCLCPP_ERROR(this->get_logger(), "Service call failed.");
            }
            
    }

private:
    std::vector<std::thread> threads_;
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<NameOfNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}