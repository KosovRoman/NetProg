#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <ctime>

const int BUFFER_SIZE = 1024;
const int PORT = 1313;  // Порт для службы daytime

int main() {
    int sockfd;
    struct sockaddr_in serverAddr, clientAddr;
    char buffer[BUFFER_SIZE];
    socklen_t addrLen = sizeof(clientAddr);

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
    serverAddr.sin_addr.s_addr = INADDR_ANY;  // Слушаем на всех интерфейсах

    // Привязка сокета к адресу
    if (bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Ошибка при привязке сокета" << std::endl;
        close(sockfd);
        return 1;
    }

    std::cout << "Сервер запущен на порту " << PORT << "..." << std::endl;

    // Основной цикл сервера
    while (true) {
        // Ожидание запроса от клиента
        int recvLen = recvfrom(sockfd, buffer, BUFFER_SIZE - 1, 0, (struct sockaddr*)&clientAddr, &addrLen);
        if (recvLen < 0) {
            std::cerr << "Ошибка при получении данных" << std::endl;
            close(sockfd);
            return 1;
        }

        // Получение текущего времени
        std::time_t currentTime = std::time(nullptr);
        char timeStr[BUFFER_SIZE];
        std::strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", std::localtime(&currentTime));

        // Отправка времени обратно клиенту
        if (sendto(sockfd, timeStr, std::strlen(timeStr), 0, (struct sockaddr*)&clientAddr, addrLen) < 0) {
            std::cerr << "Ошибка при отправке данных" << std::endl;
            close(sockfd);
            return 1;
        }

        std::cout << "Отправлено время: " << timeStr << " клиенту." << std::endl;
    }

    close(sockfd);
    return 0;
}

