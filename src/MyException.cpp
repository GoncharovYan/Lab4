#include <MyException.hpp>
MyException::MyException(std::string textError) { m_textError = std::move(textError); }
const char *MyException::what() const noexcept { return m_textError.c_str(); }

