#ifndef FTPDOWNLOADER_H
#define FTPDOWNLOADER_H

#include <cstring>
#include <iostream>
#include <ostream>
#include <string>

#include <netdb.h>
#include <unistd.h>
#include <vector>

class FtpDownloader
{
public:
    FtpDownloader(const std::string& server, unsigned int port, const std::string& username = "anonymous", const std::string& password = "anonymous")
        : server_(server), port_(port), username_(username), password_(password) {}

    bool downloadFile(const std::string& remoteFilePath, const std::string& localFilePath) {

        // Download file to buffer
        m_buffer = downloadFileToBuffer(remoteFilePath);

        if (m_buffer.empty()) {
            // Handle error or return false based on your requirements
            return true;
        }

        // Save to file
        try {
            // Open file to write to
            FILE *fp = fopen(localFilePath.c_str(), "wb");
            if (fp == NULL)
            {
                std::cout << "File " << localFilePath << " cannot be opened" << std::endl;
                return true;
            }

            // Write the buffer content to the file
            fwrite(m_buffer.data(), sizeof(char), m_buffer.size(), fp);

            // Close file
            fclose(fp);

            std::cout << "File " << localFilePath << " saved successfully !" << std::endl;
        }
        catch (std::exception& e) {
            std::cout << "Exception: " << e.what() << std::endl;
            return true;
        }

        return false;
    }

   std::vector<char> downloadFileToBuffer(const std::string& remoteFilePath) {

        const int FTP_PORT = 21;        // Server Port
        const int SIZE = 1024;          // Size of Buffers
        char receiveBuff[SIZE];         // Buffer to send to the server
        char sendBuff[SIZE];            // Buffer to receive from server
        char pasvBuff[] = "pasv";       // Buffer to see if PASV Command was entered
        char quitBuff[] = "QUIT";       // Buffer to see if QUIT Command was entered
        char pasvMessage[100];          // String for PASV information
        int length = 0, i=0;
        int a1, a2, a3, a4, p1, p2, dataPort;       //PASV Information
        std::vector<char> buffer;

        std::cout << "downloadFileToBuffer() - start" << std::endl;

        try {
            // obtain host server info
            struct hostent *host;
            host = gethostbyname(server_.c_str());
            if (host == NULL) {
                std::cout << "TCP Client: gethostbyname error" << std::endl;
                return {};
            }

            // Add server information
            struct sockaddr_in server_adr;
            memset(&server_adr, 0, sizeof(server_adr));
            server_adr.sin_family = AF_INET;
            memcpy(&server_adr.sin_addr, host->h_addr, host->h_length);
            server_adr.sin_port = htons(port_);

            // Create socket to connect to server
            int origSock;                   // Original socket in client
            if ((origSock = socket(PF_INET, SOCK_STREAM, 0)) < 0)
            {
                std::cout << "TCP Client: Create socket error" << std::endl;
                return {};
            }

            // Connect to server
            if (connect(origSock, (struct sockaddr *)&server_adr, sizeof(server_adr)) < 0)
            {
                std::cout << "TCP Client: Connect error" << std::endl;
                return {};
            }

            // Receive welcome message from server
            length = recv(origSock, receiveBuff, SIZE, 0);
            receiveBuff[length] = '\0';
            std::cout << "Receive welcome message : " << receiveBuff << std::endl;

            // Send username to server
            std::string usernameMessage = "USER " + username_ + "\r\n";
            int rc = send(origSock, usernameMessage.c_str(), usernameMessage.size(), 0);
            if (rc == -1)
            {   perror("send");
                return {};
            }
            length = recv(origSock, receiveBuff, SIZE, 0);
            receiveBuff[length] = '\0';
            std::cout << "Receive response username : " << receiveBuff << std::endl;

            // Send password to server
            std::string passwordMessage = "PASS " + password_ + "\r\n";
            send(origSock, passwordMessage.c_str(), passwordMessage.size(), 0);
            length = recv(origSock, receiveBuff, SIZE, 0);
            receiveBuff[length] = '\0';
            std::cout << "Receive response password : " << receiveBuff << std::endl;

            // Send PASV command to server : enter passive mode
            std::string pasvMessageString = "PASV\r\n";
            send(origSock, pasvMessageString.c_str(), pasvMessageString.size(), 0);
            length = recv(origSock, pasvMessage, 100, 0);
            pasvMessage[length] = '\0';
            std::cout << "Receive response PASV : " << pasvMessage << std::endl;

            // Parse PASV information
            while (pasvMessage[i] != '(')
                i++;
            sscanf(pasvMessage+i, "(%d,%d,%d,%d,%d,%d)", &a1, &a2, &a3, &a4, &p1, &p2);
            dataPort = p1*256 + p2;
            std::cout << "Data Port: " << dataPort << std::endl;

            // Create socket to connect to server
            int dataSock;                   // Socket to connect to server
            if ((dataSock = socket(PF_INET, SOCK_STREAM, 0)) < 0)
            {
                std::cout << "TCP Client: Create socket error" << std::endl;
                return {};
            }

            // Add server information
            struct sockaddr_in data_adr;
            memset(&data_adr, 0, sizeof(data_adr));
            data_adr.sin_family = PF_INET;
            memcpy(&data_adr.sin_addr, host->h_addr, host->h_length);
            data_adr.sin_port = htons(dataPort);

            // Connect to server
            if (connect(dataSock, (struct sockaddr *)&data_adr, sizeof(data_adr)) < 0)
            {
                std::cout << "TCP Client: Connect error" << std::endl;
                return {};
            }

            // Send RETR command to server
            std::string retrMessage = "RETR " + remoteFilePath + "\r\n";
            send(origSock, retrMessage.c_str(), retrMessage.size(), 0);
            length = recv(origSock, receiveBuff, SIZE, 0);
            receiveBuff[length] = '\0';
            std::cout << receiveBuff << std::endl;

            // // Open file to write to
            // FILE *fp = fopen(localFilePath.c_str(), "w");
            // if (fp == NULL)
            // {
            //     std::cout << "File " << localFilePath << " cannot be opened" << std::endl;
            //     return {};
            // }

            // Receive file from server
            while ((length = recv(dataSock, sendBuff, SIZE, 0)) > 0)
            {
                // Append the received data to the buffer
                buffer.insert(buffer.end(), sendBuff, sendBuff + length);
            }

            // // Close file
            // fclose(fp);

            // Close sockets
            close(origSock);
            close(dataSock);
        }
        catch (std::exception& e) {
            std::cout << "Exception: " << e.what() << std::endl;
            return {};
        }

        std::cout << "Buffer retrieved successfully !" << std::endl;

        return buffer;
    }

private:

    std::string server_;
    unsigned int port_;
    std::string username_;
    std::string password_;

    std::vector<char> m_buffer;

public :
    std::vector<char> buffer() { return m_buffer; }
};

#endif // FTPDOWNLOADER_H
