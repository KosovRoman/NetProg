#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

const int BUFFER_SIZE = 1024;
const int PORT = 1313;  // стандартный порт для службы daytime

int main() {
    int sockfd;
    struct sockaddr_in serverAddr;
    char buffer[BUFFER_SIZE];

    // Создание UDP-сокета
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        std::cerr << "Ошибка при создании сокета" << std::endl;
        return 1;
    }

    // Настройка адреса сервера
    std::memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);

    
    if (inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr) <= 0) {
        std::cerr << "Ошибка в IP адресе сервера" << std::endl;
        close(sockfd);
        return 1;
    }

    // Отправка пустого сообщения для запроса времени
    if (sendto(sockfd, nullptr, 0, 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Ошибка при отправке данных" << std::endl;
        close(sockfd);
        return 1;
    }

    // Получение ответа от сервера
    socklen_t addrLen = sizeof(serverAddr);
    int recvLen = recvfrom(sockfd, buffer, BUFFER_SIZE - 1, 0, (struct sockaddr*)&serverAddr, &addrLen);
    if (recvLen < 0) {
        std::cerr << "Ошибка при получении данных" << std::endl;
        close(sockfd);
        return 1;
    }

    // Завершение строки и вывод времени
    buffer[recvLen] = '\0';
    std::cout << "Текущее время с сервера: " << buffer << std::endl;

    close(sockfd);
    return 0;
}
