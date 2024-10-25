#ifndef ADMIN_FUNCTIONS
#define ADMIN_FUNCTIONS

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h> 
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h> 
#include <errno.h> 
#include <crypt.h>

#include "../record-struct/emp.h"
#include "./server-constants.h"

#define ADMIN_LOGIN_ID "admin"
#define ADMIN_PASSWORD "admin1234"

bool admin_operation_handler(int connFD);
bool admin_login_handler(int connFD);
int add_employee(int connFD);
bool modify_emp_info(int connFD);

bool admin_operation_handler(int connFD)
{

    if (admin_login_handler(connFD))
    {
        ssize_t writeBytes, readBytes;
        char readBuffer[1000], writeBuffer[1000];
        memset(writeBuffer, 0, sizeof(writeBuffer));
        strcpy(writeBuffer, ADMIN_LOGIN_SUCCESS);
        while (1)
        {
            strcat(writeBuffer, "\n");
            strcat(writeBuffer, ADMIN_MENU);
            writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
            if (writeBytes == -1)
            {
                perror("Error while writing ADMIN_MENU to client!");
                return false;
            }
            memset(writeBuffer, 0, sizeof(writeBuffer));

            readBytes = read(connFD, readBuffer, sizeof(readBuffer));
            if (readBytes == -1)
            {
                perror("Error while reading client's choice for ADMIN_MENU");
                return false;
            }

            int choice = atoi(readBuffer);
            switch (choice)
            {
            case 1:
                //Add new Employee
                add_employee(connFD);
                break;
            case 2:
                //Modify Eployee details
                modify_emp_info(connFD);
                break;
            case 3:
                //Logout
                writeBytes = write(connFD, ADMIN_LOGOUT, strlen(ADMIN_LOGOUT));
                return false;
                break;
            default:
                writeBytes = write(connFD, INVALID_MENU_CHOICE, strlen(INVALID_MENU_CHOICE));
                read(connFD, readBuffer, sizeof(readBuffer));
            }
        }
    }
    else
    {
        // ADMIN LOGIN FAILED
        return false;
    }
    return true;
}

bool admin_login_handler(int connFD){
    ssize_t readBytes, writeBytes; 
    char readBuffer[1000], writeBuffer[1000];
    char tempBuffer[1000];

    int ID;

    bzero(readBuffer, sizeof(readBuffer));
    memset(writeBuffer, 0, sizeof(writeBuffer));

    strcpy(writeBuffer, ADMIN_LOGIN_WELCOME);

   // Append the request for LOGIN ID message
    strcat(writeBuffer, "\n");
    strcat(writeBuffer, LOGIN_ID);

    writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
    if (writeBytes == -1)
    {
        perror("Error writing WELCOME & LOGIN_ID message to the client!");
        return false;
    }

    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1)
    {
        perror("Error reading login ID from client!");
        return false;
    }

    bool userFound = false;
    if (strcmp(readBuffer, ADMIN_LOGIN_ID) == 0)
            userFound = true;

    if (userFound)
    {
        memset(writeBuffer, 0, sizeof(writeBuffer));
        writeBytes = write(connFD, PASSWORD, strlen(PASSWORD));
        if (writeBytes == -1)
        {
            perror("Error writing PASSWORD message to client!");
            return false;
        }

        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));
        if (readBytes == 1)
        {
            perror("Error reading password from the client!");
            return false;
        }

        if (strcmp(readBuffer, ADMIN_PASSWORD) == 0)
                return true;

        memset(writeBuffer, 0, sizeof(writeBuffer));
        writeBytes = write(connFD, INVALID_PASSWORD, strlen(INVALID_PASSWORD));
    }
    else
    {
        memset(writeBuffer, 0, sizeof(writeBuffer));
        writeBytes = write(connFD, INVALID_LOGIN, strlen(INVALID_LOGIN));
    }

    return false;
}

int add_employee(int connFD)
{
    ssize_t readBytes, writeBytes;
    char readBuffer[1000], writeBuffer[1000];

    struct Employee newEmployee, previousEmployee;

    int employeeFileDescriptor = open(EMP_FILE, O_RDONLY);
    if (employeeFileDescriptor == -1 && errno == ENOENT)
    {
        // Employee file was never created
        newEmployee.id = 0;
    }
    else if (employeeFileDescriptor == -1)
    {
        perror("Error while opening employee file");
        return -1;
    }
    else
    {   //Reading last created employee's id so that new_empid= old_empid + 1

        int offset = lseek(employeeFileDescriptor, -sizeof(struct Employee), SEEK_END);
        if (offset == -1)
        {
            perror("Error seeking to last Employee record!");
            return false;
        }

        struct flock lock = {F_RDLCK, SEEK_SET, offset, sizeof(struct Employee), getpid()};
        int lockingStatus = fcntl(employeeFileDescriptor, F_SETLKW, &lock);
        if (lockingStatus == -1)
        {
            perror("Error obtaining read lock on Employee record!");
            return false;
        }

        readBytes = read(employeeFileDescriptor, &previousEmployee, sizeof(struct Employee));
        if (readBytes == -1)
        {
            perror("Error while reading Employee record from file!");
            return false;
        }

        lock.l_type = F_UNLCK;
        fcntl(employeeFileDescriptor, F_SETLK, &lock);

        close(employeeFileDescriptor);

        newEmployee.id = previousEmployee.id + 1;
    }

    sprintf(writeBuffer, "%s%s", ADMIN_ADD_EMPLOYEE, ADMIN_ADD_EMPLOYEE_NAME);
    writeBytes = write(connFD, writeBuffer, sizeof(writeBuffer));
    if (writeBytes == -1)
    {
        perror("Error writing ADMIN_ADD_EMPLOYEE_NAME message to client!");
        return false;
    }

    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1)
    {
        perror("Error reading employee name response from client!");
        return false;
    }

    strcpy(newEmployee.name, readBuffer);

    writeBytes = write(connFD, ADMIN_ADD_EMPLOYEE_GENDER, strlen(ADMIN_ADD_EMPLOYEE_GENDER));
    if (writeBytes == -1)
    {
        perror("Error writing ADMIN_ADD_EMPLOYEE_GENDER message to client!");
        return false;
    }

    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1)
    {
        perror("Error reading employee gender response from client!");
        return false;
    }

    if (readBuffer[0] == 'M' || readBuffer[0] == 'F' || readBuffer[0] == 'O')
        newEmployee.gender = readBuffer[0];
    else
    {
        writeBytes = write(connFD, INVALID_MENU_CHOICE, strlen(INVALID_MENU_CHOICE));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        return false;
    }

        writeBytes = write(connFD, ADMIN_ADD_EMPLOYEE_ROLE, strlen(ADMIN_ADD_EMPLOYEE_ROLE));
    if (writeBytes == -1)
    {
        perror("Error writing ADMIN_ADD_EMPLOYEE_GENDER message to client!");
        return false;
    }

    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1)
    {
        perror("Error reading employee role response from client!");
        return false;
    }

    if (readBuffer[0] == 'M' || readBuffer[0] == 'E')
        newEmployee.role = readBuffer[0];
    else
    {
        writeBytes = write(connFD, INVALID_MENU_CHOICE, strlen(INVALID_MENU_CHOICE));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        return false;
    }
        
        
    memset(writeBuffer, 0, sizeof(writeBuffer));
    strcpy(writeBuffer, ADMIN_ADD_EMPLOYEE_AGE);
    writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
    if (writeBytes == -1)
    {
        perror("Error writing ADMIN_ADD_EMPLOYEE_AGE message to client!");
        return false;
    }

    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1)
    {
        perror("Error reading employee age response from client!");
        return false;
    }

    int employeeAge = atoi(readBuffer);
    if (employeeAge == 0)
    {
        // Either client has sent age as 0 (which is invalid) or has entered a non-numeric string
        memset(writeBuffer, 0, sizeof(writeBuffer));
        strcpy(writeBuffer, ERRON_INPUT_FOR_NUMBER);
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1)
        {
            perror("Error while writing ERRON_INPUT_FOR_NUMBER message to client!");
            return false;
        }
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        return false;
    }
    newEmployee.age = employeeAge;

    strcpy(newEmployee.login, newEmployee.name);
    strcat(newEmployee.login, "-");
    sprintf(writeBuffer, "%d", newEmployee.id);
    strcat(newEmployee.login, writeBuffer);

    char hashedPassword[1000];
    //strcpy(hashedPassword, crypt(AUTOGEN_PASSWORD, SALT_BAE));
    strcpy(hashedPassword, EMP_AUTOGEN_PASSWORD);
    char idStr[5];
    snprintf(idStr, sizeof(idStr), "%d", newEmployee.id); // Convert id to string
    strcat(hashedPassword, idStr);
    strcpy(newEmployee.password, hashedPassword);

    employeeFileDescriptor = open(EMP_FILE, O_CREAT | O_APPEND | O_WRONLY, S_IRWXU);
    if (employeeFileDescriptor == -1)
    {
        perror("Error while creating / opening employee file!");
        return false;
    }
    writeBytes = write(employeeFileDescriptor, &newEmployee, sizeof(newEmployee));
    if (writeBytes == -1)
    {
        perror("Error while writing Employee record to file!");
        return false;
    }

    close(employeeFileDescriptor);

    memset(writeBuffer, 0, sizeof(writeBuffer));
    sprintf(writeBuffer, "%s%s\n%s%s", ADMIN_ADD_EMPLOYEE_AUTOGEN_LOGIN, newEmployee.login, ADMIN_ADD_EMPLOYEE_AUTOGEN_PASSWORD, newEmployee.password);
    strcat(writeBuffer, "^");
    writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
    if (writeBytes == -1)
    {
        perror("Error sending employee loginID and password to the client!");
        return false;
    }

    readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read

    return newEmployee.id;
}

bool modify_emp_info(int connFD)
{
    ssize_t readBytes, writeBytes;
    char readBuffer[1000], writeBuffer[1000];

    struct Employee employee;

    int employeeID;

    off_t offset;
    int lockingStatus;

    writeBytes = write(connFD, ADMIN_MOD_EMPLOYEE_ID, strlen(ADMIN_MOD_EMPLOYEE_ID));
    if (writeBytes == -1)
    {
        perror("Error while writing ADMIN_MOD_EMPLOYEE_ID message to client!");
        return false;
    }
    memset(readBuffer, 0, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1)
    {
        perror("Error while reading employee ID from client!");
        return false;
    }

    employeeID = atoi(readBuffer);

    int employeeFileDescriptor = open(EMP_FILE, O_RDONLY);
    if (employeeFileDescriptor == -1)
    {
        // Employee File doesn't exist
        memset(writeBuffer, 0, sizeof(writeBuffer));
        strcpy(writeBuffer, EMP_ID_DOESNT_EXIST);
        strcat(writeBuffer, "^");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1)
        {
            perror("Error while writing EMPLOYEE_ID_DOESNT_EXIST message to client!");
            return false;
        }
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        return false;
    }
    
    offset = lseek(employeeFileDescriptor, employeeID * sizeof(struct Employee), SEEK_SET);
    if (errno == EINVAL)
    {
        // Employee record doesn't exist
        memset(writeBuffer, 0, sizeof(writeBuffer));
        strcpy(writeBuffer, EMP_ID_DOESNT_EXIST);
        strcat(writeBuffer, "^");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1)
        {
            perror("Error while writing EMPLOYEE_ID_DOESNT_EXIST message to client!");
            return false;
        }
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        return false;
    }
    else if (offset == -1)
    {
        perror("Error while seeking to required employee record!");
        return false;
    }

    struct flock lock = {F_RDLCK, SEEK_SET, offset, sizeof(struct Employee), getpid()};

    // Lock the record to be read
    lockingStatus = fcntl(employeeFileDescriptor, F_SETLKW, &lock);
    if (lockingStatus == -1)
    {
        perror("Couldn't obtain lock on employee record!");
        return false;
    }

    readBytes = read(employeeFileDescriptor, &employee, sizeof(struct Employee));
    if (readBytes == -1)
    {
        perror("Error while reading employee record from the file!");
        return false;
    }

    // Unlock the record
    lock.l_type = F_UNLCK;
    fcntl(employeeFileDescriptor, F_SETLK, &lock);

    close(employeeFileDescriptor);

    writeBytes = write(connFD, ADMIN_MOD_EMPLOYEE_MENU, strlen(ADMIN_MOD_EMPLOYEE_MENU));
    if (writeBytes == -1)
    {
        perror("Error while writing ADMIN_MOD_EMPLOYEE_MENU message to client!");
        return false;
    }
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1)
    {
        perror("Error while getting employee modification menu choice from client!");
        return false;
    }

    int choice = atoi(readBuffer);
    if (choice == 0)
    { // A non-numeric string was passed to atoi
        memset(writeBuffer, 0, sizeof(writeBuffer));
        strcpy(writeBuffer, ERRON_INPUT_FOR_NUMBER);
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1)
        {
            perror("Error while writing ERRON_INPUT_FOR_NUMBER message to client!");
            return false;
        }
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        return false;
    }

    memset(readBuffer, 0, sizeof(readBuffer));
    switch (choice)
    {
    case 1:
        writeBytes = write(connFD, ADMIN_MOD_EMPLOYEE_NEW_NAME, strlen(ADMIN_MOD_EMPLOYEE_NEW_NAME));
        if (writeBytes == -1)
        {
            perror("Error while writing ADMIN_MOD_EMPLOYEE_NEW_NAME message to client!");
            return false;
        }
        readBytes = read(connFD, &readBuffer, sizeof(readBuffer));
        if (readBytes == -1)
        {
            perror("Error while getting response for employee's new name from client!");
            return false;
        }
        strcpy(employee.name, readBuffer);
        break;
    case 2:
        writeBytes = write(connFD, ADMIN_MOD_EMPLOYEE_NEW_AGE, strlen(ADMIN_MOD_EMPLOYEE_NEW_AGE));
        if (writeBytes == -1)
        {
            perror("Error while writing ADMIN_MOD_EMPLOYEE_NEW_AGE message to client!");
            return false;
        }
        readBytes = read(connFD, &readBuffer, sizeof(readBuffer));
        if (readBytes == -1)
        {
            perror("Error while getting response for employee's new age from client!");
            return false;
        }
        int updatedAge = atoi(readBuffer);
        if (updatedAge == 0)
        {
            // Either client has sent age as 0 (which is invalid) or has entered a non-numeric string
            memset(writeBuffer, 0, sizeof(writeBuffer));
            strcpy(writeBuffer, ERRON_INPUT_FOR_NUMBER);
            writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
            if (writeBytes == -1)
            {
                perror("Error while writing ERRON_INPUT_FOR_NUMBER message to client!");
                return false;
            }
            readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
            return false;
        }
        employee.age = updatedAge;
        break;
    case 3:
        writeBytes = write(connFD, ADMIN_MOD_EMPLOYEE_NEW_GENDER, strlen(ADMIN_MOD_EMPLOYEE_NEW_GENDER));
        if (writeBytes == -1)
        {
            perror("Error while writing ADMIN_MOD_EMPLOYEE_NEW_GENDER message to client!");
            return false;
        }
        readBytes = read(connFD, &readBuffer, sizeof(readBuffer));
        if (readBytes == -1)
        {
            perror("Error while getting response for employee's new gender from client!");
            return false;
        }
        employee.gender = readBuffer[0];
        break;
    case 4:
        writeBytes = write(connFD, ADMIN_MOD_EMPLOYEE_NEW_ROLE, strlen(ADMIN_MOD_EMPLOYEE_NEW_ROLE));
        if (writeBytes == -1)
        {
            perror("Error while writing ADMIN_MOD_EMPLOYEE_NEW_ROLE message to client!");
            return false;
        }
        readBytes = read(connFD, &readBuffer, sizeof(readBuffer));
        if (readBytes == -1)
        {
            perror("Error while getting response for employee's new role from client!");
            return false;
        }
        employee.role = readBuffer[0];
        break;
    default:
        writeBytes = write(connFD, INVALID_MENU_CHOICE, strlen(INVALID_MENU_CHOICE));
       
        if (writeBytes == -1)
        {
            perror("Error while writing INVALID_MENU_CHOICE message to client!");
            return false;
        }
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        return false;
    }

    employeeFileDescriptor = open(EMP_FILE, O_WRONLY);
    if (employeeFileDescriptor == -1)
    {
        perror("Error while opening employee file");
        return false;
    }
    offset = lseek(employeeFileDescriptor, employeeID * sizeof(struct Employee), SEEK_SET);
    if (offset == -1)
    {
        perror("Error while seeking to required employee record!");
        return false;
    }

    lock.l_type = F_WRLCK;
    lock.l_start = offset;
    lockingStatus = fcntl(employeeFileDescriptor, F_SETLKW, &lock);
    if (lockingStatus == -1)
    {
        perror("Error while obtaining write lock on employee record!");
        return false;
    }

    writeBytes = write(employeeFileDescriptor, &employee, sizeof(struct Employee));
    if (writeBytes == -1)
    {
        perror("Error while writing update employee info into file");
    }

    lock.l_type = F_UNLCK;
    fcntl(employeeFileDescriptor, F_SETLKW, &lock);

    close(employeeFileDescriptor);

    writeBytes = write(connFD, ADMIN_MOD_EMPLOYEE_SUCCESS, strlen(ADMIN_MOD_EMPLOYEE_SUCCESS));
    if (writeBytes == -1)
    {
        perror("Error while writing ADMIN_MOD_EMPLOYEE_SUCCESS message to client!");
        return false;
    }
    readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read

    return true;
}


#endif