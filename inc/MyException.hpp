#pragma once
#include <iostream>

class MyException : public std::exception {
public:
    explicit MyException(std::string textError);
    const char* what() const noexcept override;
private:
    std::string m_textError;
};