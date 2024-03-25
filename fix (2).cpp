#include <string>
#include <stdexcept>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <cinttypes>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>
#include <any>

using nlohmann::json;

struct student_t
{
    std::string name;
    std::any group;
    std::any avg;
    std::any debt;
};

auto get_name(const json &j) -> std::string
{
    if (j.is_null())
        return nullptr;
    else if (j.is_string())
        return j.get<std::string>();
    else
        return nullptr;
}

auto get_debt(const json &j) -> std::any
{
    if (j.is_null())
        return nullptr;
    else if (j.is_string())
        return j.get<std::string>();
    else
        return j.get<std::vector<std::string>>();
}

auto get_avg(const json &j) -> std::any
{
    if (j.is_null())
        return nullptr;
    else if (j.is_string())
        return j.get<std::string>();
    else if (j.is_number_float())
        return j.get<double>();
    else
        return j.get<std::size_t>();
}

auto get_group(const json &j) -> std::any
{
    if (j.is_null())
        return nullptr;
    else if (j.is_string())
        return j.get<std::string>();
    else
        return j.get<std::size_t>();
}

void from_json(const json &j, student_t &s)
{
    s.name = get_name(j.at("name"));
    s.group = get_group(j.at("group"));
    s.avg = get_avg(j.at("avg"));
    s.debt = get_debt(j.at("debt"));
}

bool isItemsArray(const json &j)
{
    return !j.at("items").is_array() ? false : true;
}

bool isMetaValid(const json &j)
{
    return (static_cast<int>(j.at("_meta").at("count")) != j.at("items").size()) ? false : true;
}

void printHeader(std::ostream &os)
{
    os
        << "|" << std::left << std::setw(20) << " name"
        << " |"
        << std::left << std::setw(20) << " group"
        << " |"
        << std::left << std::setw(20) << " avg"
        << " |"
        << std::left << std::setw(20) << " debt"
        << " |" << std::endl
        << std::setfill('-') << std::left << std::setw(22) << "|"
        << std::left << std::setw(20) << "|"
        << std::left << std::setw(2) << "|"
        << std::left << std::setw(22) << "|"
        << "|" << std::endl;
}

void printStudent(const student_t &student, std::ostream &os)
{

    os << std::setfill(' ');
    os
        << "| " << std::left << std::setw(20)
        << student.name << "|";

    if (student.group.type() == typeid(std::nullptr_t))
    {
        os
            << std::left << std::setw(20)
            << " null"
            << " |";
    }
    else if (student.group.type() == typeid(std::string))
    {
        os
            << " " << std::left << std::setw(20)
            << std::any_cast<std::string>(student.group) << "|";
    }
    else
    {
        os
            << " " << std::left << std::setw(20)
            << std::any_cast<uint64_t>(student.group) << "|";
    }

    if (student.avg.type() == typeid(std::nullptr_t))
    {
        os
            << std::left << std::setw(20)
            << " null"
            << " |";
    }
    else if (student.avg.type() == typeid(std::string))
    {
        os
            << " " << std::left << std::setw(20)
            << std::any_cast<std::string>(student.avg) << "|";
    }
    else if (student.avg.type() == typeid(double))
    {
        os
            << " " << std::left << std::setw(20)
            << std::any_cast<double>(student.avg) << "|";
    }
    else
    {
        os
            << " " << std::left << std::setw(20)
            << std::any_cast<uint64_t>(student.avg) << "|";
    }

    if (student.debt.type() == typeid(std::nullptr_t))
    {
        os
            << std::left << std::setw(20)
            << " null"
            << " |";
    }
    else if (student.debt.type() == typeid(std::string))
    {
        os
            << " " << std::left << std::setw(20)
            << std::any_cast<std::string>(student.debt) << "|";
    }
    else
    {
        int dsize = std::any_cast<std::vector<std::string>>(student.debt).size();
        os
            << " " << std::left << std::setw(20)
            << std::to_string(dsize) + " items"
            << "|";
    }
    os
        << std::endl
        << std::setfill('-') << std::left << std::setw(22) << "|"
        << std::left << std::setw(22) << "|"
        << std::left << std::setw(22) << "|"
        << std::left << std::setw(22) << "|"
        << "|" << std::endl;
}

int main(int argc, char *argv[])
{
    const std::string jsonPath = argv[1];
    std::ifstream file{"./" + jsonPath};
    try
    {
         if (!file) {
            throw  "Error: unable to open file ";
        }
    }
      catch(const char* e)
    {
        std::cout << e << '\n';
        exit(0);
    }
    
    json data;
        file >> data;
    try {
       
    
        if (!isItemsArray(data)) {
            throw "Error: \"items\" expected to be array" ;
        }
        if (!isMetaValid(data)) {
            throw "Error: number of students doesn’t match \"meta\"";
            
        }
    }
      catch(const char* e)
    {
        std::cout << e << '\n';
       exit(0);
    }

    for (auto const &item : data.at("items"))
    {
        student_t student = item.get<student_t>();
        printStudent(student, std::cout);
    }
}