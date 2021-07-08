#pragma once
#include <System.hpp>
#include <MyException.hpp>
#include <json.hpp>
#include <httplib.h>
#include <unordered_map>
#include <unordered_set>


using JS = nlohmann::json;

class BRS{
public:
    BRS();
    void listen();
    void init();

private:
    static void processReq(const std::function<void(const httplib::Request &req, JS& ans)>& action, const httplib::Request &req, httplib::Response &res);
    static JS creatErrorAns(const std::string& err);
    static JS creatAns(const std::string& answer);
private:
    System system;
    httplib::Server svr;
    std::unordered_map<std::string, std::function<void(const httplib::Request &req, JS& ans)>> reqs;
    std::unordered_set<std::string> reqsName = { "getTeachersTasks",
                                                 "getCountOfSubjectsTasks",
                                                 "getStudentsTasks",
                                                 "getStudentsMiddleMarkBySubj",
                                                 "getStudentsMiddleMark",
                                                 "getCountOfCompletedTasksOnSubjAndStud",
                                                 "getCountOfCompletedTasksOnSubj",
                                                 "getUncompletedOpenedTasks",
                                                 "createTask",
                                                 "closeTask",
                                                 "rateAnswer",
                                                 "getListUncompletedTasks",
                                                 "submitTask",
                                                 "createPerson" };
};