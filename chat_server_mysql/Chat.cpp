#include "Chat.h"

Chat::Chat() {}
Chat::Chat(std::string filename) : name(filename) {}

void Chat::load_chat() {
    chat.read_from_file(name);
}
void Chat::load_chat(std::string file) {
    chat.read_from_file(file);
}

void Chat::save_chat() {
    chat.write_on_file(name);
}
void Chat::save_chat(std::string file) {
    chat.write_on_file(file);
}

void Chat::save_last_msg(Message& msg) {
    chat.write_on_file(name, msg);
}

bool Chat::messaging(User& user, std::string reciever_path) {
    std::string text{};

    std::cout << "Input message: ";
    std::getline(std::cin, text);

    if (!text.empty()) {
        if (text == "\\quit") return false; // 
        else {
            Message* message = new Message(user.get_name(), text);

            save_last_msg(*message);
            if (!(reciever_path == name)) {
                chat.write_on_file(reciever_path, *message);
            }

            load_chat();
            system("cls");
            print_chat();

            delete message;
            return true;
        }
    }
}

void Chat::print_chat() {
    for (int i = 0; i < chat.getSize(); i++) {
        std::cout << chat[i].get_auth() << ": " << chat[i].get_msg() << '\n';
    }
}