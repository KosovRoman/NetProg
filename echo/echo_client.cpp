#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

const int PORT = 8080;  // Порт для службы echo
const int BUFFER_SIZE = 1024;

int main() {
    int sockfd;
    struct sockaddr_in serverAddr;
    char buffer[BUFFER_SIZE];

    // Создание TCP-сокета
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "Ошибка при создании сокета" << std::endl;
        return 1;
    }

    // Настройка адреса сервера
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr) <= 0) {
        std::cerr << "Ошибка в IP адресе сервера" << std::endl;
        close(sockfd);
        return 1;
    }

    // Подключение к серверу
    if (connect(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Ошибка при подключении к серверу" << std::endl;
        close(sockfd);
        return 1;
    }

    std::cout << "Введите сообщение для отправки на сервер: ";
    std::cin.getline(buffer, BUFFER_SIZE);

    // Отправка сообщения на сервер
    send(sockfd, buffer, strlen(buffer), 0);

    // Получение ответа от сервера
    int recvLen = recv(sockfd, buffer, BUFFER_SIZE - 1, 0);
    if (recvLen < 0) {
        std::cerr << "Ошибка при получении данных от сервера" << std::endl;
        close(sockfd);
        return 1;
    }

    buffer[recvLen] = '\0';
    std::cout << "Ответ от сервера: " << buffer << std::endl;

    close(sockfd);
    return 0;
}
