#include <string>
#include <fstream>
#include "routine.cpp"
#include "Chat.h"
#include "database_routine.cpp"

int main()
{
    users.read_from_file("users.txt");      // �������� ���������� ������������� ��� ���������� ������
    std::filesystem::create_directory("C:\\ChatUsers"); // �������� ���������� ��� ����� �������� �������� ���������� � ������ �����

    // std::cout << "List of users:\n";
    // std::cout << users;
    // system("pause");
    //main_loop();

    main_server_loop();

    delete logger;

    return 0;
}