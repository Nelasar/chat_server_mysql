#include <string>
#include <fstream>
#include "routine.cpp"
#include "Chat.h"
#include "database_routine.cpp"

int main()
{
    users.read_from_file("users.txt");      // считывем сохранённых пользователей для дальнейшей работы
    std::filesystem::create_directory("C:\\ChatUsers"); // создание директории для более удобного хранения ифнормации о личных чатах

    // std::cout << "List of users:\n";
    // std::cout << users;
    // system("pause");
    //main_loop();

    main_server_loop();

    delete logger;

    return 0;
}