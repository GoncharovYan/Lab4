#pragma once
#include <string>
#include <utility>

class Users{
public:
    Users(std::string login): m_login(std::move(login)){};
    Users(std::string login, std::string firstName,std::string secondName, std::string sex,int age):
        m_firstName(std::move(firstName)), m_secondName(std::move(secondName)), m_login(std::move(login)),
        m_sex(std::move(sex)), m_age(age){}
    ~Users()=default;
    std::string getLogin() { return m_login; }
private:
    std::string m_firstName;
    std::string m_login;
    std::string m_secondName;
    std::string m_sex;
    int m_age{};
};

class Student: public Users{
public:
    Student(std::string login): Users(std::move(login)){};
    Student(std::string login, std::string firstName,std::string secondName, std::string sex,int age,std::string faculty,int group, int course):
    Users(std::move(login),std::move(firstName),std::move(secondName),std::move(sex),age),m_group(group),m_course(course),m_faculty(std::move(faculty)){}
    ~Student()=default;
private:
    int m_group{};
    std::string m_faculty;
    int m_course{};
};

class Teacher: public Users{
public:
    Teacher(std::string login): Users(std::move(login)){};
    Teacher(std::string login, std::string firstName,std::string secondName, std::string sex,int age,std::string faculty,std::string position):
    Users(std::move(login),std::move(firstName),std::move(secondName),std::move(sex),age),m_faculty(std::move(faculty)), m_position(std::move(position)){};
    ~Teacher()=default;
private:
    std::string m_faculty;
    std::string m_position;
};

class Admin: public Users{
public:
    Admin(std::string login): Users(std::move(login)){};
    Admin(std::string login, std::string firstName,std::string secondName, std::string sex,int age):
        Users(std::move(firstName),std::move(secondName),
               std::move(login),std::move(sex),age){}
    ~Admin() = default;
};
