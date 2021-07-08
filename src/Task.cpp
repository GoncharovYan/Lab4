#include <Task.hpp>

std::string Task::getName() { return m_name; }
std::string Task::getSubject() { return m_subject; }
std::string Task::getTeacherId() { return m_teacherId; }

bool Task::iSOpen() const { return m_isOpen; }

void Task::closeTask() { m_isOpen = false; }
void Task::addUser(const std::string& user) { m_ans[user] = Answer(); }

void Task::rateUserAns(const std::string& user, int ball) {
    if (m_ans.find(user)==m_ans.end())
        throw MyException("Этот пользователь не выполняет данную задачу");
    m_ans[user].setMark(ball);
}

void Task::addAnswer(const std::string &user, std::string ans) {
    if (m_ans.find(user) == m_ans.end())
        throw MyException("Этот пользователь не выполняет данную задачу");
    m_ans[user].setAnswer(std::move(ans));
}

bool Task::consistUser(const std::string &user) {
    if (m_ans.find(user) != m_ans.end()) { return true; }
    else { return false; }
}

int Task::userMark(const std::string &user) {
    return m_ans[user].getMark();
}

bool Task::userComplete(const std::string &user) {
    return m_ans[user].completeStatus();
}

int Task::numOfTaskOnItStatus(bool state) {
    int num = 0;
    for (auto ans : m_ans){
        if (ans.second.completeStatus() == state){
            ++num;
        }
    }
    return num;
}

std::string Task::getDescription() {
    return m_description;
}

int Answer::getMark() const { return m_mark; }
void Answer::setMark(int points) { m_mark = points; }
bool Answer::completeStatus() const { return m_isComplete; }
void Answer::setAnswer(std::string ans) {
    m_answer = std::move(ans);
    m_isComplete = true;
}




