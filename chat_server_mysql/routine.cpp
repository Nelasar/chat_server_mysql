#define NOMINMAX
#include <limits>
#include <iostream>
#include <string>
#include <fstream>
#include <limits>
#include <filesystem>
#include "Network.cpp"
#include "database_routine.cpp"
#include "Chat.h"
#include "Logger.h"

#define NO_USER -1 // для функции поиска , означает, что пользователь не найден


// ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ
static bool active = true;                                      // флаг работы приложения, если false, то работа программы завершена
static Array<User> users;                                       // массив, в котором хранятся все зарегистрированные пользователи
static User* current_user;                                      // текущий пользователь, используется для входа в чат и при
static Chat* global_chat;                                       // переменная для работы с глобальным чатом
static std::string users_path_string = "C:\\ChatUsers\\";       // путь для хранения папок пользователей
static Logger* logger = new Logger;


inline void writer(const std::string& msg) {
    logger->write_log(msg);
}
inline void reader() {
    std::cout << "LAST LOGGED MESSAGE: " << logger->read_log();
}

// ФУНКЦИИ ДЛЯ ЧАТОВ
// всомогательная функция работы глобального чата для функции main_menu
inline void global_chatting() {
    system("cls");
    global_chat = new Chat("global.txt");
    global_chat->load_chat();
    global_chat->print_chat();

    bool messaging = true;

    while (messaging) {
        if (global_chat->messaging(*current_user, "global.txt")) continue;

        else messaging = false;
    }
}
// вспомогательная функция работы персонального чата для функции main_menu
inline void personal_chatting(User& other) {
    system("pause");

    std::string rec_path_string = users_path_string + current_user->get_login() + "\\" + other.get_login() + ".txt";
    std::string trans_path_string = users_path_string + other.get_login() + "\\" + current_user->get_login() + ".txt";

    std::cout << rec_path_string << '\n';
    std::cout << trans_path_string << '\n';

    if (!std::filesystem::exists(users_path_string + current_user->get_login())) {
        std::filesystem::create_directory(users_path_string + current_user->get_login()); // создаём для нового пользователя свою директорию
    }
    if (!std::filesystem::exists(users_path_string + other.get_login())) {
        std::filesystem::create_directory(users_path_string + other.get_login()); // создаём для нового пользователя свою директорию
    }

    system("pause");

    Chat* personal_chat = new Chat(rec_path_string);
    bool messaging = true;

    personal_chat->load_chat();

    while (messaging) {
        if (personal_chat->messaging(*current_user, trans_path_string)) continue;
        else messaging = false;
    }

    delete personal_chat;
    system("pause");
}

// ФУНКЦИИ ДЛЯ РЕГИСТРАЦИИ
// вспомогательная функция, проверяет если ли уже зарегистрированный пользователь с таким логином/именем
inline bool is_already_registered(std::string login, std::string username) {
    for (int i = 0; i < users.getSize(); i++) {
        if (users[i].get_login() == login) {
            std::cout << "User with this login already exists!\n";
            return true;
        }
        else if (users[i].get_name() == username) {
            std::cout << "User with this name already exists!\n";
            return true;
        }
        else return false;
    }
    return false;
}
// функция регистрации пользователя
inline bool registration() {
    system("cls");

    std::string nu_name{};
    std::string nu_pass{};
    std::string nu_login{};
    std::string nu_pass_repeat{}; // для верификации пароля

    bool flag = true;

    do {
        std::cout << "Enter your login. You will use to enter the chat:\n";
        std::cin >> nu_login;
        std::cout << "Input your Username. Other users will see this name\n";
        std::cin >> nu_name;

    } while (flag = is_already_registered(nu_login, nu_name), flag);

    std::cout << "Enter your password. You will use it to get access to the chat:\n";
    std::cin >> nu_pass;

    std::cout << "Repeat your password:\n";

    std::cin >> nu_pass_repeat;

    while (nu_pass != nu_pass_repeat) {
        std::cout << "Passwords do not match!\nRepeat your input:\n";
        std::cin >> nu_pass_repeat;
    }

    std::cout << "\nYour registration info:\n";
    std::cout << "Your username: " << nu_name << '\n';
    std::cout << "Your login: " << nu_login << '\n';
    std::cout << "Your password: " << nu_pass << '\n';

    User* new_user = new User(nu_login, nu_name, nu_pass); // Создаём нового пользователя по введённым данны м
    users.pushBack(*new_user); // и добавляем его в массив зарегистрированных пользователей

    std::cout << users;
    users.write_on_file("users.txt", (*new_user)); // сразу после этого сохраняем пользователя в файле

    std::cout << "Registration completed!\n";

    std::filesystem::create_directory("C:\\ChatUsers\\" + nu_login); // создаём для нового пользователя свою директорию

    system("pause");

    delete new_user; // чистим память 
    return false;
}

// ФУНКЦИИ ДЛЯ ВХОДА В СИСТЕМУ
// вспомогательная функция поиска пользователя среди зарегистрироанных по логину
inline int find_user_by_login(std::string login) {
    for (int i = 0; i < users.getSize(); i++) {
        if (users[i].get_login() == login) {
            return i;
        }
    }
    return NO_USER;
}
// функция валидации пароля пользователя
inline bool validate_password(std::string password, int index) {
    if (users[index].get_password() == password) {
        return true;
    }
    else return false;
}
// функция входа в систему
inline bool log_in() {
    system("cls");
    std::string login{};
    std::string pass{};

    std::cout << "Please input your log in info:\n";
    std::cout << "Input login:\n";
    std::cin >> login;

    int index = find_user_by_login(login);

    if (index == NO_USER) { // проверяем, есть ли такой пользователь в "базе" Вообще
        std::cout << "There is no user with login " << login << "!\n";
        system("pause");
        return false;
    }

    std::cout << "Input password:\n";
    std::cin >> pass;

    if (validate_password(pass, index)) { // если всё сошлось, то логинимся!
        current_user = new User(users[index]);

        std::cout << "Welcome!\n";
        system("pause");
        return true;
    }
    else { // иначе доступ запрещён
        std::cout << "Access denied!\n";
        system("pause");
        return false;
    }

    system("pause");
}
// Приветственное меню, можно зарегистрироваться, залогиниться и выйти
inline bool greetings() {
    system("cls");
    std::cout << "Hello!\n";
    std::cout << "To enter the chat (R)egister or (L)og in!\n";
    std::cout << "Q for quit\n";

    char option{};

    option = std::cin.get();

    switch (option) {
    case 'r':
    case 'R': {
        return registration();
        break;
    }
    case 'l':
    case 'L': {
        return log_in();
        break;
    }
    case 'q':
    case 'Q': {
        active = false;
        break;
    }
    default: {
        std::cout << "There is no such option!\n";
        system("pause");
        break;
    }
    }
}


#define MESSAGE_BUFFER 4096
#define PORT 7777

static char buffer[MESSAGE_BUFFER];
static char message[MESSAGE_BUFFER];
static int socket_file_descriptor;

#ifdef _WIN32
typedef int socklen_t;
#endif

static socklen_t message_size, length;
static const char* end_string = "end";

static WSADATA wsaData;

static struct sockaddr_in serveraddress, client;

// Главный цикл и меню 
enum MENU { // перечисление опций меню
    GLOBAL = 1,
    PERSONAL = 2,
    ONLINE = 3,
    LOGOUT = 4,
    QUIT = 5
};
// главное меню программы, доступно только после входа по логину и паролю.
inline bool main_menu() {
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize Winsock" << std::endl;
        system("pause");
    }
    socket_file_descriptor = static_cast<int>(socket(AF_INET, SOCK_DGRAM, 0));

    if (socket_file_descriptor == -1) {
        std::cerr << "Failed to create socket" << std::endl;
        system("pause");
#ifdef _WIN32
        WSACleanup();
#endif
        // return false;
    }

    serveraddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddress.sin_port = htons(PORT);
    serveraddress.sin_family = AF_INET;

    // BIND SOCKET TO ADDRESS AND PORT
    if (bind(socket_file_descriptor, reinterpret_cast<struct sockaddr*>(&serveraddress),
        sizeof(serveraddress)) == -1) {
        std::cerr << "Failed  to bind socket" << std::endl;
        system("pause");
#ifdef _WIN32
        closesocket(socket_file_descriptor);
        WSACleanup();
#endif
        // return true;
    }

    int option{};

    // Текстовое меню
    system("cls");
    std::cout << "Enjoy your time!\nChoose an option: ";
    std::cout << "1 - Enter Global Chat\n";
    std::cout << "2 - Enter Personal Chat\n";
    std::cout << "3 - Enter Online Chat\n";
    std::cout << "4 - Log Out\n";
    std::cout << "5 - Quit\n";
    std::cout << "Input '\\quit' in chats to leave the chat.\n";
    std::cout << "\nInput option: ";

    std::cin >> option;

    while (true) {
        if ((!std::cin.fail() && option < GLOBAL && option > QUIT) || (std::cin.fail())) {
            //std::cin.ignore(std::numeric_limits<std::streamsize>::max());
            std::cin.clear();

            std::cout << "Wrong input!\nTry again:\n";
            std::cin >> option;
        }
        else break;
    }

    switch (option) {
    case GLOBAL: {
        // глобальный чат
        global_chatting();

        return true;
        break;
    }
    case PERSONAL: {
        // персональный чат
        try {
            int index = 0;
            std::cout << "Who do you want chat with? Input index.\nInput zero (0) to exit this menu.\n";

            for (int i = 0; i < users.getSize(); i++) {
                std::cout << i + 1 << ". " << users[i].get_name() << '\n';
            }

            std::cin >> index;

            while (true) {
                if ((!std::cin.fail() && index < 0 && option > users.getSize() + 1) || (std::cin.fail())) {

                    //std::cin.ignore(std::numeric_limits<std::streamsize>::max());
                    std::cin.clear();
                    //std::cout << "Wrong input!\nTry again:\n";
                    //std::cin >> index;
                    throw bad_index();
                }
                else break;
            }


            if (!index) return true;
            else personal_chatting(users[index - 1]);

            return true;
        }
        catch (std::exception& e) {
            std::cout << e.what() << '\n';
            system("pause");
            break;
        }
        break;
    }
    case ONLINE: {
        std::cout << "SERVER IS LISTENING THROUGH THE PORT: " << PORT << " WITHIN A LOCAL SYSTEM" << std::endl;

        while (true) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            length = sizeof(client);
            // RECEIVE DATA FROM CLIENT
            message_size = recvfrom(socket_file_descriptor, buffer, sizeof(buffer) - 1, 0,
                reinterpret_cast<struct sockaddr*>(&client), &length);

            if (message_size == -1) {
                std::cerr << "Faild to receive data from client" << std::endl;
#ifdef _WIN32
                closesocket(socket_file_descriptor);
                WSACleanup();
#else
                close(socket_file_descriptor);
#endif
                return true;
            }

            buffer[message_size] = '\0';

            // CHECK FOR TERMINATION SIGNAL
            if (strcmp(buffer, end_string) == 0) {
                std::cout << "Server is quitting" << std::endl;
#ifdef _WIN32
                closesocket(socket_file_descriptor);
                WSACleanup();
#else
                close(socket_file_descriptor);
#endif
                return true;
            }

            std::cout << buffer << std::endl;
            // REPLY TO CLIENT
            std::cout << "Enter message:" << std::endl;


            Message msg(current_user->get_name());
            std::cin >> msg;
            //std::string full_message = msg.get_auth() + ": " + msg.get_msg();

            strcpy(message, msg.formated_message().c_str());


            // std::cin.getline(message, MESSAGE_BUFFER);

            sendto(socket_file_descriptor, message, strlen(message), 0,
                reinterpret_cast<sockaddr*>(&client), sizeof(client));

            std::cout << message << std::endl;
            std::cout << "Waiting for the reply from Client . . ." << std::endl;
        }

        std::cout << "Server is quitting" << std::endl;
        system("pause");

#ifdef _WIN32
        closesocket(socket_file_descriptor);
        WSACleanup();
#else
        close(socket_file_descriptor);
#endif
        return true;
        break;
    }
    case LOGOUT: {
        // разлогиниться
        delete current_user;
        delete global_chat;
        return false;
        break;
    }
    case QUIT: {
        // выйти из программы
        active = false;
        return false;
        break;
    }
    default: {
        std::cout << "Nu such option, try again!\n";
        system("pause");
        return true;
        break;
    }
    }
}

// главный цикл программы, зависит от переменной active
inline void main_loop() {
    if (!initialize_mysql()) {
        std::cout << "FAILED TO INITIALIZE DATABASE\nSHUTDOWN . . ." << std::endl;
    }

    bool loged = false;

    while (active) {
        if (!loged) {
            loged = greetings();
        }
        else loged = main_menu();
    }
}

static const char* login_command = "LOGGING";
static const char* registration_command = "REGISTER";
static const char* message_command = "MESSAGE";

static WSAData wsaDataMysql;
static SOCKET server_socket;
static sockaddr_in local_addr;

///////////////////////////////////
///////////////////////////////////
///////////////////////////////////
///////////////////////////////////
///////////////////////////////////
///////////////////////////////////
///////////////////////////////////

inline void authorization(int bsize, sockaddr_in client_addr, int client_addr_size) {
    strcpy(&buffer[0], "ACCEPT");
    sendto(server_socket, &buffer[0], bsize, 0, (sockaddr*)&client_addr, sizeof(client_addr));

    bsize = recvfrom(server_socket, &buffer[0], sizeof(buffer) - 1, 0, (sockaddr*)&client_addr, &client_addr_size);
    buffer[bsize] = 0;

    std::string login{ buffer };
    std::string logged_login = login + '\n';

    std::thread w1(writer, logged_login);
    w1.join();

    login.erase(std::remove(login.begin(), login.end(), '\n'), login.end());
    login = '\'' + login + '\'';

    std::string query = "SELECT password FROM Users WHERE login = " + login;
    mysql_query(&mysql, query.c_str());

    res = mysql_store_result(&mysql);
    row = mysql_fetch_row(res);

    char* password = row[0];
    std::cout << "char* password = " << password << std::endl;

    bsize = recvfrom(server_socket, &buffer[0], sizeof(buffer) - 1, 0, (sockaddr*)&client_addr, &client_addr_size);
    buffer[bsize] = '\0';

    std::cout << bsize << std::endl;
    std::cout << strcmp(buffer, password) << buffer << ' ' << password << std::endl;

    if (!strcmp(buffer, password)) {
        std::cout << "PASSWORDSS MATCH. ACCESS GRANTED." << std::endl;

        strcpy(&buffer[0], "GRANTED");
        std::cout << "SENDING: " << buffer << std::endl;
        sendto(server_socket, &buffer[0], sizeof("GRANTED"), 0, (sockaddr*)&client_addr, sizeof(client_addr));

        return;
    }
    else {
        std::cout << "DENIED" << std::endl;
        strcpy(&buffer[0], "DENIED\n");
        sendto(server_socket, &buffer[0], sizeof("DENIED"), 0, (sockaddr*)&client_addr, sizeof(client_addr));
    }
}

inline void registration(int bsize, sockaddr_in client_addr, int client_addr_size) {
    std::cout << "STARTING REGISTRATION PROCESS\n";

    strcpy(&buffer[0], "ACCEPT");
    sendto(server_socket, &buffer[0], bsize, 0, (sockaddr*)&client_addr, sizeof(client_addr));

    bsize = recvfrom(server_socket, &buffer[0], sizeof(buffer) - 1, 0, (sockaddr*)&client_addr, &client_addr_size);
    buffer[bsize] = 0;

    std::cout << "GOT LOGIN: " << buffer << std::endl;

    std::string login{ buffer };
    login.erase(std::remove(login.begin(), login.end(), '\n'), login.end());
    login = '\'' + login + '\'';

    bsize = recvfrom(server_socket, &buffer[0], sizeof(buffer) - 1, 0, (sockaddr*)&client_addr, &client_addr_size);
    buffer[bsize] = 0;

    std::cout << "GOT PASSWORD: " << buffer << std::endl;

    std::string password{ buffer };
    password.erase(std::remove(password.begin(), password.end(), '\n'), password.end());
    password = '\'' + password + '\'';

    std::string query = "INSERT INTO Users(id, login, password) VALUES (default, " + login + ", " + password + ")";
    std::cout << query << std::endl;

    int result = mysql_query(&mysql, query.c_str());
    std::cout << result << std::endl;

    strcpy(&buffer[0], "SUCCESS");
    sendto(server_socket, &buffer[0], sizeof("SUCCESS"), 0, (sockaddr*)&client_addr, sizeof(client_addr));

    return;
}

inline void messaging(int bsize, sockaddr_in client_addr, int client_addr_size) {
    std::cout << "BUFFER IN MESSAGING: " << buffer << std::endl;    
    std::string chat_history{};
    
    mysql_query(&mysql, "SELECT * FROM Chat");
    if (res = mysql_store_result(&mysql)) {
        while (row = mysql_fetch_row(res)) {
            for (int i = 0; i < mysql_num_fields(res); i++) {
                chat_history +=  row[i];
                chat_history += " -- ";
            }
            chat_history += "\n";
        }
    }
    else {
        std::cout << "MySQL Error: " << mysql_error(&mysql);
        return;
    }

    memset(&buffer[0], 0, sizeof(buffer));
    memcpy(buffer, chat_history.c_str(), chat_history.length());
    
    sendto(server_socket, &buffer[0], sizeof(buffer) - 1, 0, (sockaddr*)&client_addr, sizeof(client_addr));

    bsize = recvfrom(server_socket, &buffer[0], strlen(buffer), 0, (sockaddr*)&client_addr, &client_addr_size);
    buffer[bsize] = 0;

    std::string message{ buffer };

    std::thread r1(reader);
    std::thread w1(writer, message);
    std::thread r2(reader);

    r1.join();
    w1.join();
    r2.join();

    message = '\'' + message + '\'';

    std::string query = "INSERT INTO Chat(id, message, time) VALUES (default, " + message + ", default)";
    // std::cout << query << std::endl;
    std::string logged_query = query + '\n';

    int result = mysql_query(&mysql, query.c_str());

    return;
}

inline void main_server_loop() {
    if (!initialize_mysql()) {
        std::cout << "FAILED TO INITIALIZE DATABASE\nSHUTDOWN . . ." << std::endl;
        return;
    }

    std::cout << "UDP echo-Server\nWith MySQL database" << std::endl;

    if (WSAStartup(MAKEWORD(2, 2), &wsaDataMysql)) {
        std::cout << "WSAStartup error: " << WSAGetLastError() << std::endl;
        return;
    }

    server_socket = socket(AF_INET, SOCK_DGRAM, 0);

    if (server_socket == INVALID_SOCKET) {
        std::cout << "Socket() error: \n" << WSAGetLastError() << std::endl;
#ifdef _WIN32
        WSACleanup();
#endif
        return;
    }

    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = INADDR_ANY;
    local_addr.sin_port = htons(PORT);

    if (bind(server_socket, (sockaddr*)&local_addr, sizeof(local_addr)))
    {
        std::cout << "Bind error: \n" << WSAGetLastError() << std::endl;
#ifdef _WIN32
        closesocket(server_socket);
        WSACleanup();
#else
        close(my_sock);
#endif
        return;
    }


    while (true) {
        sockaddr_in client_addr;
        int client_addr_size = sizeof(client_addr);

        int bsize = recvfrom(server_socket, &buffer[0], sizeof(buffer) - 1, 0, (sockaddr*)&client_addr, &client_addr_size);
        if (bsize == SOCKET_ERROR) std::cout << "Failed to recover from client:\n" << WSAGetLastError() << std::endl;

        HOSTENT* hst;

        hst = gethostbyaddr((char*)&client_addr.sin_addr, 4, AF_INET);

        printf("+%s [%s:%d] new DATAGRAM!\n",
              (hst) ? hst->h_name : "Unknown host",
              inet_ntoa(client_addr.sin_addr),
              ntohs(client_addr.sin_port));

        std::cout << "REQUEST: " << buffer;

        std::string request{ buffer };
        request += '\n';

        std::thread w1(reader);
        std::thread r1(writer, request);
        r1.join();
        w1.join();

        buffer[bsize] = 0;

        if (!strcmp(buffer, login_command)) {
            authorization(bsize, client_addr, client_addr_size);
        }
        else if (!strcmp(buffer, registration_command)) {
            registration(bsize, client_addr, client_addr_size);
        }
        else if (!strcmp(buffer, message_command)) {
            messaging(bsize, client_addr, client_addr_size);
        }
        else {
            std::cout << "Unknown Request" << std::endl;
        }
    }
}
