#pragma once
#include "Message.h"
#include "User.h"
#include "Template.h"

class Chat {
private:
    Array<Message> chat{}; // ��� �� ���� ��� ������ ���������, ���������� � ����� �� ������� (��� ��)
    std::string name{};    // ��� ���� - ��� �������� ����� � �����������, ������������ ������ ��� ������ � �������� ������/�������
public:
    Chat();
    Chat(std::string filename);

    // ������� ���������� � �������� ����� �/�� �����
    void load_chat();
    void load_chat(std::string file);

    void save_chat();
    void save_chat(std::string file);
    // ���������� � ���� ���� ���������� ���������
    void save_last_msg(Message& msg);

    bool messaging(User& user, std::string reciever_path); // ������� ���������, ���� ������������ � ���������� ����, �� ���������� ���� ���������� � ���� ��������� ����
    // ���� ��� ������������, �� ����������� ��� ����� - ��� ����������� � ���������� � ��������������� ������

    void print_chat();                                     // ����� ���� �� �����
};