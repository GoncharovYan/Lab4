#include <BRS.hpp>

void BRS::listen() {
    svr.listen("0.0.0.0",10024);
}

void BRS::init() {
    system.start();
    for (const auto& reqName : reqsName){
        std::string name = "/" + reqName;
        svr.Post(name.c_str(),[this, reqName](const httplib::Request& req, httplib::Response &res){
            processReq(reqs[reqName], req, res);
        });
    }
}

void BRS::processReq(const std::function<void(const httplib::Request &, JS &)> &action,
                     const httplib::Request &req, httplib::Response &res) {
    JS answer;
    try {
        action(req, answer);
    } catch(MyException& e) {
        res.set_content(creatErrorAns(e.what()).dump(),"application/json");
        return;
    } catch(...) {
        res.set_content(creatErrorAns("Ошибка запроса").dump(),"application/json");
    }
    res.set_content(answer.dump(), "application/json");
}

JS BRS::creatErrorAns(const std::string& err) {
    JS ans;
    ans["info"] = err;
    ans["type"] = "error";
    return ans;
}

JS BRS::creatAns(const std::string& answer) {
    JS ans;
    ans["info"] = answer;
    ans["type"] = "successful action";
    return ans;
}

BRS::BRS() {
    reqs["getTeachersTasks"] = [this](const httplib::Request &req, JS& ans){
        JS j = JS::parse(req.body);
        std::string loginSender = j["sender"]["login"];
        std::string loginTeacher = j["teacher"]["login"];
        auto tasks = system.getTasksByTeacher({loginSender}, {loginTeacher});
        ans = creatAns("Список задач успешно получен");
        int i=0;
        for(const auto& task : tasks){
            ans["body"][std::to_string(i)]["name"] = task->getName();
            ans["body"][std::to_string(i)]["description"] = task->getDescription();
            ++i;
        }
    };
    reqs["getCountOfSubjectsTasks"] = [this](const httplib::Request &req, JS& ans){
        JS j = JS::parse(req.body);
        std::string loginSender = j["sender"]["login"];
        std::string subjectName = j["subj"]["name"];
        auto num = system.getCountOfTasksBySubj({loginSender}, {subjectName});
        ans = creatAns("Кол-во задач по предмету успешно получено");
        ans["body"]["number"] = num;
    };
    reqs["getStudentsTasks"] = [this](const httplib::Request &req, JS& ans){
        JS j = JS::parse(req.body);
        std::string loginSender = j["sender"]["login"];
        std::string loginStudent = j["student"]["login"];
        auto tasks = system.getTasksByStudent({loginSender}, {loginStudent});
        ans = creatAns("Список задач успешно получен");
        int i = 0;
        for(const auto& task : tasks){
            ans["body"][std::to_string(i)]["subj"] = task->getSubject();
            ans["body"][std::to_string(i)]["name"] = task->getName();
            ans["body"][std::to_string(i)]["description"] = task->getDescription();
            ++i;
        }
    };
    reqs["getStudentsMiddleMarkBySubj"] = [this](const httplib::Request &req, JS& ans){
        JS j = JS::parse(req.body);
        std::string loginSender = j["sender"]["login"];
        std::string loginStudent = j["student"]["login"];
        std::string subjectName = j["subj"]["name"];
        auto mark = system.getMiddleMarkByStudentAndSubj({loginSender},
                                                         {loginStudent}, subjectName);
        ans = creatAns("Средний балл по предмету успешно получен");
        ans["body"]["mark"] = mark;
    };
    reqs["getStudentsMiddleMark"] = [this](const httplib::Request &req, JS& ans){
        JS j = JS::parse(req.body);
        std::string loginSender = j["sender"]["login"];
        std::string loginStudent = j["student"]["login"];
        auto mark = system.getMiddleMarkByStudent({loginSender}, {loginStudent});
        ans = creatAns("Средний балл  успешно получен");
        ans["body"]["mark"] = mark;
    };
    reqs["getCountOfCompletedTasksOnSubjAndStud"] = [this](const httplib::Request &req, JS& ans){
        JS j = JS::parse(req.body);
        std::string loginSender = j["sender"]["login"];
        std::string loginStudent = j["student"]["login"];
        std::string subjectName = j["subj"]["name"];
        std::string stateOfTask = j["state"];
        int num = 0;
        if (stateOfTask == "complete") {
            num = system.getCountOfCompletedTasksByStudAndSubj({loginSender},
                                                               {loginStudent}, subjectName);
            ans = creatAns("Количество сданных задач по предмету успешно получено");
            ans["body"]["num"] = num;
        } else if (stateOfTask == "unComplete") {
            num = system.getNumberOfUncompletedTasksByStudAndSubj({loginSender},
                                                                  {loginStudent}, subjectName);
            ans = creatAns("Количество не сданных задач по предмету успешно получено");
            ans["body"]["num"] = num;
        } else throw MyException("Задача может быть только сданной/несданной complete/unComplete)");
    };
    reqs["getCountOfCompletedTasksOnSubj"] = [this](const httplib::Request &req, JS& ans){
        JS j = JS::parse(req.body);
        std::string loginSender = j["sender"]["login"];
        std::string subjectName = j["subj"]["name"];
        std::string stateOfTask = j["state"];
        int num = 0;
        if (stateOfTask == "complete"){
            num = system.getCountOfCompletedTasksBySubj({loginSender}, subjectName);
            ans = creatAns("Количество сданных задач  успешно получен");
            ans["body"]["num"] = num;
        } else if (stateOfTask == "unComplete"){
            num = system.getCountOfUncompletedTasksBySubj({loginSender}, subjectName);
            ans = creatAns("Количество не сданных задач  успешно получен");
            ans["body"]["num"] = num;
        } else throw MyException("Задача может быть только сданной/несданной complete/unComplete)");
    };
    reqs["getUncompletedOpenedTasks"] = [this](const httplib::Request &req, JS& ans){
        JS j = JS::parse(req.body);
        std::string loginSender = j["sender"]["login"];
        std::string loginStudent = j["student"]["login"];
        auto tasks = system.getTasksInProgressByStud({loginSender}, {loginStudent});
        ans = creatAns("Список задач успешно получен");
        int i=0;
        for(const auto& task : tasks){
            ans["body"][std::to_string(i)]["subj"] = task->getSubject();
            ans["body"][std::to_string(i)]["name"] = task->getName();
            ans["body"][std::to_string(i)]["description"] = task->getDescription();
            ++i;
        }
    };
    ////////////////////////////////////////////
    reqs["createTask"] = [this](const httplib::Request &req, JS& ans){
        JS j = JS::parse(req.body);
        std::string loginSender = j["sender"]["login"];
        std::string taskName = j["task"]["name"];
        std::string taskDescription = j["task"]["description"];
        std::string taskSubject = j["task"]["subj"];
        std::vector<std::string> users = j["task"]["person"];
        auto task = Task(taskName,taskDescription,loginSender,
                         taskSubject);
        for (const auto& user : users){
            task.addUser(user);
        }
        system.createTask({loginSender}, task);
        ans = creatAns("Задача успешно добавлена");
    };
    reqs["closeTask"] = [this](const httplib::Request &req, JS& ans){
        JS j = JS::parse(req.body);
        std::string loginSender = j["sender"]["login"];
        std::string taskName = j["task"]["name"];
        std::string taskSubject = j["task"]["subj"];
        system.closeTask({loginSender}, {taskName, taskSubject});
        ans = creatAns("Задача успешно закрыта");
    };
    reqs["rateAnswer"] = [this](const httplib::Request &req, JS& ans){
        JS j = JS::parse(req.body);
        std::string loginSender = j["sender"]["login"];
        std::string loginStudent = j["student"]["login"];
        std::string taskName = j["task"]["name"];
        std::string taskSubject = j["task"]["subj"];
        int mark = j["mark"];
        system.rateTask({loginSender}, {taskName, taskSubject},
                        {loginStudent}, mark);
        ans = creatAns(" Решение успешно оценено");
    };
    reqs["getListUncompletedTasks"] = [this](const httplib::Request &req, JS& ans){
        JS j = JS::parse(req.body);
        std::string loginSender = j["sender"]["login"];
        std::string subjectName = j["subj"]["name"];
        auto tasks = system.getTasksInProgressByTeacher({loginSender}, subjectName);
        ans = creatAns("Список задач успешно получен");
        int i = 0;
        for(const auto& task : tasks){
            ans["body"][std::to_string(i)]["name"] = task->getName();
            ans["body"][std::to_string(i)]["description"] = task->getDescription();
            ++i;
        }
    };
    ///////////////////////////////////////////////////////////
    reqs["submitTask"] = [this](const httplib::Request &req, JS& ans){
        JS j = JS::parse(req.body);
        std::string loginSender = j["sender"]["login"];
        std::string taskName = j["task"]["name"];
        std::string taskSubject = j["task"]["subj"];
        std::string taskAnswer = j["task"]["ans"];
        system.submitTask({loginSender}, {taskName, taskSubject},
                          taskAnswer);
        ans = creatAns("Решение успешно добавлено");
    };
    //////////////////////////////////////////////////////////
    reqs["createPerson"] = [this](const httplib::Request &req, JS& ans){
        JS j = JS::parse(req.body);
        std::string loginSender = j["sender"]["login"];
        std::string personType = j["person"]["type"];
        std::string personLogin = j["person"]["login"];
        std::string personFName = j["person"]["fName"];
        std::string personSName = j["person"]["sName"];
        int personAge = j["person"]["age"];
        std::string personSex = j["person"]["sex"];
        if (personType=="Admin"){
            system.createAdmin({loginSender}, {personLogin, personFName,
                                               personSName, personSex, personAge});
            ans = creatAns("Администратор успешно добавлен");
        } else if(personType=="Teacher") {
            std::string personFaculty = j["person"]["faculty"];
            std::string personPosition = j["person"]["position"];
            system.createTeacher({loginSender},
                                 {personLogin, personFName, personSName,
                                  personSex, personAge, personFaculty,personPosition});
            ans = creatAns("Учитель успешно добавлен");
        } else if(personType=="Student") {
            std::string personFaculty = j["person"]["faculty"];
            int personGroup = j["person"]["group"];
            int personCourse = j["person"]["course"];
            system.createStudent({loginSender},
                                 {personLogin, personFName, personSName,
                                  personSex, personAge, personFaculty, personGroup, personCourse});
            ans = creatAns("Студент успешно добавлен");
        } else {
            throw MyException("Неверный тип пользователя");
        }
    };
}