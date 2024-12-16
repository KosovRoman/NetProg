#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

const int PORT = 8080;  // Порт для службы echo
const int BUFFER_SIZE = 1024;

int main() {
    int sockfd, newSockfd;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    char buffer[BUFFER_SIZE];

    // Создание TCP-сокета
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "Ошибка при создании сокета" << std::endl;
        return 1;
    }

    // Настройка адреса сервера
    std::memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;  // Слушаем на всех интерфейсах

    // Привязка сокета к адресу
    if (bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Ошибка при привязке сокета" << std::endl;
        close(sockfd);
        return 1;
    }

    // Ожидание входящих соединений
    if (listen(sockfd, 5) < 0) {
        std::cerr << "Ошибка при прослушивании порта" << std::endl;
        close(sockfd);
        return 1;
    }

    std::cout << "Сервер запущен на порту " << PORT << "..." << std::endl;

    // Прием подключения от клиента
    newSockfd = accept(sockfd, (struct sockaddr*)&clientAddr, &clientAddrLen);
    if (newSockfd < 0) {
        std::cerr << "Ошибка при принятии соединения" << std::endl;
        close(sockfd);
        return 1;
    }

    std::cout << "Подключение от клиента: " << inet_ntoa(clientAddr.sin_addr) << std::endl;

    // Чтение данных от клиента и отправка их обратно
    int recvLen;
    while ((recvLen = recv(newSockfd, buffer, BUFFER_SIZE - 1, 0)) > 0) {
        buffer[recvLen] = '\0';  // Завершаем строку
        std::cout << "Получено от клиента: " << buffer << std::endl;

        // Отправка данных обратно клиенту (эхо)
        if (send(newSockfd, buffer, recvLen, 0) < 0) {
            std::cerr << "Ошибка при отправке данных" << std::endl;
            close(newSockfd);
            close(sockfd);
            return 1;
        }
    }

    if (recvLen < 0) {
        std::cerr << "Ошибка при получении данных" << std::endl;
    }

    // Закрытие соединения
    close(newSockfd);
    close(sockfd);

    return 0;
}

