#ifndef ECOLOGGICAL_EXCEPTION_H
#define ECOLOGGICAL_EXCEPTION_H

#include <string>

class EcologgicalException : public std::exception {
private:
    std::string _msg;
public:
    explicit EcologgicalException(const std::string& message) : _msg(message) { };

    const char* what() const noexcept override {
        return _msg.c_str();
    }
};

#endif //ECOLOGGICAL_EXCEPTION_H
