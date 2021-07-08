#include <System.hpp>
#include <utility>
#include <vector>

void System::start() {
    std::shared_ptr<Users> admin = std::make_shared<Admin>("admin");
    m_staff["admin"]=admin;
}

////////////////////////////////////Авторизация////////////////////////////////////////////////////
std::shared_ptr<Users> System::getPersonFromBD(Users& p) {
    auto sendIt = m_staff.find(p.getLogin());
    if (sendIt != m_staff.end()) { return sendIt->second; }
    throw MyException("Такого пользователя не существует");
}

std::shared_ptr<Student> System::getStudent(const std::shared_ptr<Users>& p) {
    std::shared_ptr<Student> stud = std::static_pointer_cast<Student>(p);
    if (stud) { return stud; }
    throw MyException("Этот пользователь не является студентом");
}

std::shared_ptr<Teacher> System::getTeacher(const std::shared_ptr<Users>& p) {
    std::shared_ptr<Teacher> teacher = std::static_pointer_cast<Teacher>(p);
    if (teacher) { return teacher; }
    throw MyException("Этот пользователь не является преподавателем");
}

std::shared_ptr<Admin> System::getAdmin(const std::shared_ptr<Users>& p) {
    std::shared_ptr<Admin> adm = std::static_pointer_cast<Admin>(p);
    if (adm) { return adm; }
    throw MyException("Этот пользователь не является админом");
}

//////////////////////////////////////////////Админ///////////////////////////////////
void System::createTeacher(Users sender, Teacher teach) {
    auto send = getAdmin(getPersonFromBD(sender));
    if (m_staff.find(teach.getLogin()) != m_staff.end())
        throw MyException("Логин занят");
    m_staff[teach.getLogin()]=std::make_shared<Teacher>(teach);
}

void System::createAdmin(Users sender, Admin adm) {
    auto send = getAdmin(getPersonFromBD(sender));
    if (m_staff.find(adm.getLogin()) != m_staff.end())
        throw MyException("Логин занят");
    m_staff[adm.getLogin()]=std::make_shared<Admin>(adm);

}

void System::createStudent(Users sender, Student stud) {
    auto send = getAdmin(getPersonFromBD(sender));
    if (m_staff.find(stud.getLogin()) != m_staff.end())
        throw MyException("Логин занят");
    m_staff[stud.getLogin()]=std::make_shared<Student>(stud);
}

///////////////////////////////////////Препод///////////////////////////////////
void System::createTask(Users sender, Task t) {
    auto send = getTeacher(getPersonFromBD(sender));
    if (m_task.find(t.getName()+t.getSubject())!=m_task.end()) throw MyException("Задача существует");
    m_task[t.getName() + t.getSubject()] = std::make_shared<Task>(t);
}

void System::closeTask(Users sender, Task t) {
    auto send = getTeacher(getPersonFromBD(sender));
    if (m_task.find(t.getName() + t.getSubject()) == m_task.end())
        throw MyException("Задача не существует");
    if (m_task.find(t.getName() + t.getSubject())->second->getTeacherId() != send->getLogin())
        throw MyException("Чужая задача");
    m_task[t.getName() + t.getSubject()]->closeTask();
}

void System::rateTask(Users sender, Task t, Users stud, int mark) {
    auto send = getTeacher(getPersonFromBD(sender));
    auto student = getStudent(getPersonFromBD(stud));
    if (m_task.find(t.getName() + t.getSubject()) == m_task.end())
        throw MyException("Задача не существует");
    if (m_task.find(t.getName() + t.getSubject())->second->getTeacherId() != send->getLogin())
        throw MyException("Чужая задача");
    m_task[t.getName() + t.getSubject()]->rateUserAns(student->getLogin(),mark);
}

std::vector<std::shared_ptr<Task>> System::getTasksInProgressByTeacher(Users sender,
                                                                   const std::string& subject) {
    auto send = getTeacher(getPersonFromBD(sender));
    std::vector<std::shared_ptr<Task>> notCompletedTask;
    for (const auto& task : m_task){
        if(task.second->getTeacherId() == send->getLogin() &&
                task.second->getSubject() == subject && task.second->iSOpen() == false){
            notCompletedTask.emplace_back(task.second);
        }
    }
    return notCompletedTask;
}

////////////////////////////////////////Студент////////////////////////////////////
void System::submitTask(Users sender, Task t, std::string ans) {
    auto send = getStudent(getPersonFromBD(sender));
    if (m_task.find(t.getName() + t.getSubject()) == m_task.end())
        throw MyException("Задачи не существует");
    m_task[t.getName() + t.getSubject()]->addAnswer(send->getLogin(), std::move(ans));
}

//////////////////////////////////////Вспомогательные///////////////////////////////
std::vector<std::shared_ptr<Task>> System::getTasksByTeacher(Users sender, Users p) {
    auto send = getPersonFromBD(sender);
    auto teacher = getTeacher(getPersonFromBD(p));
    std::vector<std::shared_ptr<Task>> tasksByTeacher;
    for (const auto& task : m_task){
        if (task.second->getTeacherId() == teacher->getLogin()){
            tasksByTeacher.emplace_back(task.second);
        }
    }
    return tasksByTeacher;
}

int System::getCountOfTasksBySubj(Users sender, const std::string& subject) {
    auto send = getPersonFromBD(sender);
    int num = 0;
    for (const auto& task : m_task){
        if (task.second->getSubject() == subject){
            ++num;
        }
    }
    return num;
}

std::vector<std::shared_ptr<Task>> System::getTasksByStudent(Users sender, Users p) {
    auto send = getPersonFromBD(sender);
    auto student = getStudent(getPersonFromBD(p));
    std::vector<std::shared_ptr<Task>> tasksByStudent;
    for (const auto& task : m_task){
        if (task.second->consistUser(student->getLogin())){
            tasksByStudent.emplace_back(task.second);
        }
    }
    return  tasksByStudent;
}

double System::getMiddleMarkByStudentAndSubj(Users sender, Users stud, const std::string& subject) {
    auto send = getPersonFromBD(sender);
    auto student = getStudent(getPersonFromBD(stud));
    double ball = 0;
    int num = 0;
    for (const auto& task : m_task){
        if (task.second->getSubject() == subject && task.second->consistUser(student->getLogin())){
            if (task.second->userComplete(student->getLogin())){
                ++num;
                ball += task.second->userMark(student->getLogin());
            }
        }
    }
    return ball/num;
}

double System::getMiddleMarkByStudent(Users sender, Users stud) {
    auto send = getPersonFromBD(sender);
    auto student = getStudent(getPersonFromBD(stud));
    double ball = 0;
    int num = 0;
    for (const auto& task : m_task){
        if (task.second->consistUser(student->getLogin())){
            if (task.second->userComplete(student->getLogin())){
                ++num;
                ball += task.second->userMark(student->getLogin());
            }
        }
    }
    return ball/num;
}

int System::getCountOfCompletedTasksByStudAndSubj(Users sender, Users stud, const std::string& subject) {
    auto send = getPersonFromBD(sender);
    auto student = getStudent(getPersonFromBD(stud));
    int counter=0;
    for (const auto& task:m_task){
        if (task.second->getSubject()==subject&&task.second->consistUser(student->getLogin())){
            if (task.second->userComplete(student->getLogin())){
                ++counter;
            }
        }
    }
    return counter;

}

int System::getNumberOfUncompletedTasksByStudAndSubj(Users sender, Users stud, const std::string& subject) {
    auto send = getPersonFromBD(sender);
    auto student = getStudent(getPersonFromBD(stud));
    int num=0;
    for (const auto& task : m_task){
        if (task.second->getSubject() == subject&&task.second->consistUser(student->getLogin())){
            if (!task.second->userComplete(student->getLogin())){
                ++num;
            }
        }
    }
    return num;
}

int System::getCountOfCompletedTasksBySubj(Users sender, const std::string& subject) {
    auto sendBy = getPersonFromBD(sender);
    int num = 0;
    for (const auto& task : m_task){
        if (task.second->getSubject() == subject){
            num += task.second->numOfTaskOnItStatus(true);
        }
    }
    return num;
}

int System::getCountOfUncompletedTasksBySubj(Users sender, const std::string& subject) {
    auto send = getPersonFromBD(sender);
    int num=0;
    for (const auto& task : m_task){
        if (task.second->getSubject() == subject){
            num += task.second->numOfTaskOnItStatus(false);
        }
    }
    return num;
}

std::vector<std::shared_ptr<Task>> System::getTasksInProgressByStud(Users sender, Users stud) {
    auto send = getPersonFromBD(sender);
    auto student = getStudent(getPersonFromBD(stud));
    std::vector<std::shared_ptr<Task>> tasksForStudent;
    for (const auto &task : m_task) {
        if (task.second->iSOpen() == true && !task.second->userComplete(student->getLogin())) {
            tasksForStudent.emplace_back(task.second);
        }
    }
    return tasksForStudent;
}
