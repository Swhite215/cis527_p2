/*
 * multiThreadServer.c -- a multithreaded server
 */

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <map>

using namespace std;

#define PORT 9999 // port we're listening on
#define MAX_LINE 256

fd_set master; // master file descriptor list
int listener;  // listening socket descriptor
int fdmax;

// Global Address Book Variables
struct Address
{
    int id;
    string firstName;
    string lastName;
    string phone;
};

std::vector<Address> addressBook;

// Global Username, Password, and Logged In Variables
struct User
{
    std::string username;
    std::string password;
    bool logged_in;
    std::string ip_address;
    int file_descriptor;
};
std::vector<User> users;
std::map<int,std::string> fd_IP;

// the child thread
void *ChildThread(void *newfd)
{
    char buf[MAX_LINE];
    int nbytes;
    int i, j;
    int childSocket = (long)newfd;

    while (1)
    {
        // handle data from a client
        if ((nbytes = recv(childSocket, buf, sizeof(buf), 0)) <= 0)
        {
            // got error or connection closed by client
            if (nbytes == 0)
            {
                // connection closed
                cout << "multiThreadServer: socket " << childSocket << " hung up" << endl;
            }
            else
            {
                perror("recv");
            }
            close(childSocket);           // bye!
            FD_CLR(childSocket, &master); // remove from master set
            pthread_exit(0);
        }
        else
        {

            // Validate Commands
            char *chunk;
            chunk = strtok(buf, " ");

            // Handle Login Command
            if (strcmp(chunk, "LOGIN") == 0)
            {

                string username;
                string password;

                bool inputError;

                int elementCount = 0;

                while (chunk != NULL)
                {
                    if (elementCount == 0)
                    {
                        chunk = strtok(NULL, " ");
                        username = chunk;
                        elementCount++;
                    }
                    else if (elementCount == 1)
                    {
                        chunk = strtok(NULL, " ");
                        password = chunk;

                        if (password.back() == '\n')
                        {
                            password.pop_back();
                        }

                        elementCount++;
                        chunk = strtok(NULL, " ");
                    }
                    else if (elementCount == 2)
                    {
                        inputError = true;
                        break;
                    }
                }
                // Check Error Flag - Send Error Message
                if (inputError)
                {
                    char sbuf[MAX_LINE] = "301 Message Format Error\n";
                    send(childSocket, sbuf, strlen(sbuf) + 1, 0);
                    continue;
                }

                // Check For Username in Vector
                bool loggedInUser = false;
                std::vector<User>::iterator it = users.begin();
                while (it != users.end())
                {
                    // Validate Username and Password
                    if (it->username == username && it->password == password)
                    {
                        // Set Logged In Value
                        it->logged_in = true;
                        it->file_descriptor = childSocket;
                        loggedInUser = true;
                        break;
                    }
                    else
                    {
                        it++;
                    }
                }

                if (loggedInUser) {
                    // Form Message
                    string fullMessage = "200 OK\n";

                    // Prepare Buffer
                    char sbuf[MAX_LINE];
                    strcpy(sbuf, fullMessage.c_str());

                    // Send Buffer
                    send(childSocket, sbuf, strlen(sbuf) + 1, 0);
                } else {
                    // Form Message
                    string fullMessage = "410 Wrong User ID or Password\n";

                    // Prepare Buffer
                    char sbuf[MAX_LINE];
                    strcpy(sbuf, fullMessage.c_str());

                    // Send Buffer
                    send(childSocket, sbuf, strlen(sbuf) + 1, 0);
                }
                
            } // Paste Server Code*
            else if (strcmp(chunk, "LOGOUT\n") == 0){
                std::vector<User>::iterator it = users.begin();
                bool logout = false;
                while (it != users.end())
                {
                    // Validate Username and Password
                    if (it->file_descriptor == childSocket)
                    {
                        // Set Logged In Value
                        it->logged_in = false;
                        logout = true;
                        break;
                    }
                    else
                    {
                        it++;
                    }
                }
                if(logout){
                    
                    string fullMessage = "200 OK\n";

                    // Prepare Buffer
                    char sbuf[MAX_LINE];
                    strcpy(sbuf, fullMessage.c_str());

                    // Send Buffer
                    send(childSocket, sbuf, strlen(sbuf) + 1, 0);
                } else {
                    // Form Message
                    string fullMessage = "400 bad request - user is not logged in\n";

                    // Prepare Buffer
                    char sbuf[MAX_LINE];
                    strcpy(sbuf, fullMessage.c_str());

                    // Send Buffer
                    send(childSocket, sbuf, strlen(sbuf) + 1, 0);
                }
            } else if (strcmp(chunk, "WHO\n") == 0) {
                // Iterate Through Vector
                string concatRecords;

                bool foundActiveUsers = false;
                std::vector<User>::iterator it = users.begin();
                while (it != users.end())
                {
                    // Validate Username and Password
                    if (it->logged_in)
                    {
                        // Grab IP Address
                        std::string ip = fd_IP[childSocket];
                        string record = it->username + "    " + ip + "\n";
                        concatRecords += record;
                        foundActiveUsers = true;
                        it++;
                    }
                    else
                    {
                        it++;
                    }
                }

                // Form Response String and Send
                if (foundActiveUsers) {
                    string success = "200 OK";
                    string finalMessage = success + "\n" + "The list of the active users: \n" + concatRecords;

                    // Send Single Message
                    char sbuf[finalMessage.length()];
                    strcpy(sbuf, finalMessage.c_str());
                    send(childSocket, sbuf, strlen(sbuf) + 1, 0);
                } else {
                     // Prepare Message
                     string fullMessage = "404 NOT FOUND - No users are active. \n";
                     
                     // Prepare Buffer
                    char sbuf[MAX_LINE];
                    strcpy(sbuf, fullMessage.c_str());

                    // Send Buffer
                    send(childSocket, sbuf, strlen(sbuf) + 1, 0);
                }
            }
            
            // // we got some data from a client
            // cout << buf;
            // for (j = 0; j <= fdmax; j++)
            // {
            //     // send to everyone!
            //     if (FD_ISSET(j, &master))
            //     {
            //         // except the listener and ourselves
            //         if (j != listener && j != childSocket)
            //         {
            //             if (send(j, buf, nbytes, 0) == -1)
            //             {
            //                 perror("send");
            //             }
            //         }
            //     }
            // }
        }
    }
}

// Function Prototypes
bool writeAddressBookToFile(std::vector<Address> addressBook, string writePath);
void setUpAddressBook(std::string path);
void setUpUser(std::string path);

int main(void)
{

    // Set Up Address Book
    std::string path = "/Users/swhit210/E.C.R.I.C_Development/cis527_p2";
    setUpAddressBook(path);
    setUpUser(path);

    struct sockaddr_in myaddr;     // server address
    struct sockaddr_in remoteaddr; // client address
    int newfd;                     // newly accept()ed socket descriptor
    int yes = 1;                   // for setsockopt() SO_REUSEADDR, below
    socklen_t addrlen;

    pthread_t cThread;

    FD_ZERO(&master); // clear the master and temp sets

    // get the listener
    if ((listener = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        exit(1);
    }

    // lose the pesky "address already in use" error message
    if (setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
    {
        perror("setsockopt");
        exit(1);
    }

    // bind
    myaddr.sin_family = AF_INET;
    myaddr.sin_addr.s_addr = INADDR_ANY;
    myaddr.sin_port = htons(PORT);
    memset(&(myaddr.sin_zero), '\0', 8);
    if (bind(listener, (struct sockaddr *)&myaddr, sizeof(myaddr)) == -1)
    {
        perror("bind");
        exit(1);
    }

    // listen
    if (listen(listener, 10) == -1)
    {
        perror("listen");
        exit(1);
    }

    // add the listener to the master set
    FD_SET(listener, &master);

    // keep track of the biggest file descriptor
    fdmax = listener; // so far, it's this one

    addrlen = sizeof(remoteaddr);

    // main loop
    for (;;)
    {
        // handle new connections

        if ((newfd = accept(listener, (struct sockaddr *)&remoteaddr, &addrlen)) == -1)
        {
            perror("accept");
            exit(1);
        }
        else
        {
            FD_SET(newfd, &master); // add to master set
            cout << "multiThreadServer: new connection from "
                 << inet_ntoa(remoteaddr.sin_addr)
                 << " socket " << newfd << endl;

            if (newfd > fdmax)
            { // keep track of the maximum
                fdmax = newfd;
            }

            // Add File Descriptor and IP Address to Mapping
            fd_IP[newfd] = inet_ntoa(remoteaddr.sin_addr);

            if (pthread_create(&cThread, NULL, ChildThread, (void *)(intptr_t)newfd) < 0)
            {
                perror("pthread_create");
                exit(1);
            }
        }
    }
    return 0;
}

void setUpAddressBook(std::string path)
{

    string value;
    std::fstream addressFile;
    std::string PATH = path;
    std::string filename = "address.dat";
    addressFile.open(PATH + "/" + filename);

    if (addressFile)
    { // Check For Error

        int elementCount = 0;
        int id;
        string name;
        string last;
        string phone;

        // Read Each Line - Portion
        while (addressFile >> value)
        { // If a value was read, execute the code
            if (elementCount == 0)
            {
                id = std::stoi(value);
                elementCount++;
            }
            else if (elementCount == 1)
            {
                name = value;
                elementCount++;
            }
            else if (elementCount == 2)
            {
                last = value;
                elementCount++;
            }
            else if (elementCount == 3)
            {
                phone = value;

                // Generate Address Structure
                Address addressToAdd = {id, name, last, phone};

                // Add Address to Vector
                addressBook.push_back(addressToAdd);

                // Reset Count
                elementCount = 0;
            }
        }

        addressFile.close();
    }
    else
    {
        std::cout << "Error opening file for reading!" << std::endl;
        exit(1);
    }
}
void setUpUser(std::string path)
{
    string value;
    std::fstream userFile;
    std::string PATH = path;
    std::string filename = "shadow.dat";
    userFile.open(PATH + "/" + filename);

    if (userFile)
    { // Check For Error

        int elementCount = 0;
        std::string username;
        std::string password;

        // Read Each Line - Portion
        while (userFile >> value)
        { // If a value was read, execute the code
            if (elementCount == 0)
            {
                username = value;
                elementCount++;
            }
            else if (elementCount == 1)
            {
                password = value;
                User userToAdd = {username, password, false, " ", -1};

                // Add user to Vector
                users.push_back(userToAdd);

                // Reset Count
                elementCount = 0;
            }
        }

        userFile.close();
    }
    else
    {
        std::cout << "Error opening file for reading!" << std::endl;
        exit(1);
    }
}