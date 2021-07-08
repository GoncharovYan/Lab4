#pragma once
#include <Users.hpp>
#include <Task.hpp>
#include <MyException.hpp>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <vector>

class System{
public:
    System() = default;
    ~System() = default;
    void start();

    void createAdmin(Users sender, Admin adm);
    void createStudent(Users sender, Student stud);
    void createTeacher(Users sender, Teacher teach);

    void createTask(Users sender, Task t);
    void closeTask(Users sender,  Task t);
    void rateTask(Users sender, Task t, Users stud, int x);
    std::vector<std::shared_ptr<Task>> getTasksInProgressByTeacher(Users sender, const std::string& subject);

    void submitTask(Users sender, Task t, std::string ans);

    std::vector<std::shared_ptr<Task>> getTasksInProgressByStud(Users sender, Users stud);
    std::vector<std::shared_ptr<Task>> getTasksByTeacher(Users sender, Users p);
    std::vector<std::shared_ptr<Task>> getTasksByStudent(Users sender, Users p);
    double getMiddleMarkByStudentAndSubj(Users sender, Users stud, const std::string& subject);
    double getMiddleMarkByStudent(Users sender, Users stud);
    int getCountOfCompletedTasksByStudAndSubj(Users sender, Users stud, const std::string& subject);
    int getNumberOfUncompletedTasksByStudAndSubj(Users sender, Users stud, const std::string& subject);
    int getCountOfCompletedTasksBySubj(Users sender, const std::string& subject);
    int getCountOfUncompletedTasksBySubj(Users sender, const std::string& subject);
    int getCountOfTasksBySubj(Users sender, const std::string& subject);

private:
    std::shared_ptr<Users> getPersonFromBD(Users& p);
    static std::shared_ptr<Student> getStudent(const std::shared_ptr<Users>& p);
    static std::shared_ptr<Admin> getAdmin(const std::shared_ptr<Users>& p);
    static std::shared_ptr<Teacher> getTeacher(const std::shared_ptr<Users>& p);

private:
    std::unordered_map<std::string,std::shared_ptr<Users>> m_staff;
    std::unordered_map<std::string,std::shared_ptr<Task>> m_task;

};
