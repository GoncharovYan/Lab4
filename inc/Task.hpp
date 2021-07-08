#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include <utility>
#include <MyException.hpp>

class Answer {
public:
     Answer()=default;
    ~Answer()=default;
    int getMark() const;
    void setAnswer(std::string ans);
    void setMark(int ball);
    bool completeStatus() const;

private:
    std::string m_answer{};
    int m_mark{};
    bool m_isComplete{false};
};

class Task {
public:
    Task(std::string name,std::string subject)
        : m_name(std::move(name)), m_subject(std::move(subject)){
    };
    Task(std::string name, std::string description, std::string teacherLogin, std::string subject) :
            m_description(std::move(description)), m_name(std::move(name)), m_isOpen(true),
            m_subject(std::move(subject)), m_teacherId(std::move(teacherLogin)){}
    ~Task()=default;
    std::string getName();
    std::string getSubject();
    std::string getTeacherId();
    std::string getDescription();
    bool iSOpen() const;
    bool userComplete(const std::string& user);
    void closeTask();
    void addUser(const std::string& user);
    bool consistUser(const std::string& user);
    void rateUserAns(const std::string& user, int ball);
    void addAnswer(const std::string& user, std::string ans);
    int  numOfTaskOnItStatus(bool state);
    int userMark(const std::string& user);

private:
    bool m_isOpen;
    std::string m_name;
    std::string m_subject;
    std::string m_teacherId;
    std::string m_description;
    std::unordered_map<std::string, Answer> m_ans;
};

