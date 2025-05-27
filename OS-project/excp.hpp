#include <stdexcept> 
#ifndef EXCP_HPP  
#define EXCP_HPP 
class MyException : public std::runtime_error {
public:
    MyException(const std::string& message)
        : std::runtime_error("exception: " + message) {}
};
#endif