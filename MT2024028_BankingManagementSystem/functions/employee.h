#ifndef EMP_FUNCTIONS
#define EMP_FUNCTIONS

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

#include "../record-struct/account.h"
#include "../record-struct/customer.h"
#include "./server-constants.h"

#include <sys/ipc.h>
#include <sys/sem.h>
int semIdentifier;

struct Employee loggedInEmployee;

bool emp_operation_handler(int connFD);
bool emp_login_handler(int connFD, struct Employee *emp);
bool add_account(int connFD);
int add_customer(int connFD,  int newAccountNumber);
bool modify_customer_info(int connFD);
bool change_emp_password(int connFD, struct Employee *emp);
bool process_loan(int connFD);
bool view_assgn_loan(int connFD);
bool assign_loan(int connFd);
bool act_customer(int connFd);

bool emp_operation_handler(int connFD)
{
    if (emp_login_handler(connFD, &loggedInEmployee))
    {
        ssize_t writeBytes, readBytes; 
        char readBuffer[1000], writeBuffer[1000]; 
        memset(writeBuffer, 0, sizeof(writeBuffer));
        strcpy(writeBuffer,"Hey ");
        strcat(writeBuffer, loggedInEmployee.name);
        strcat(writeBuffer, EMP_LOGIN_SUCCESS);
        while (1)
        {   
            strcat(writeBuffer, "\n");
            if(loggedInEmployee.role=='E'){
                //Employee Functionalities
            strcat(writeBuffer, EMP_MENU);
            writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
            if (writeBytes == -1)
            {
                perror("Error while writing EMP_MENU to client!");
                return false;
            }
            memset(writeBuffer, 0, sizeof(writeBuffer));

            readBytes = read(connFD, readBuffer, sizeof(readBuffer));
            if (readBytes == -1)
            {
                perror("Error while reading client's choice for EMP_MENU");
                return false;
            }

            int choice = atoi(readBuffer);
            switch (choice)
            {
            case 1:
            //Add new customer
                add_account(connFD);
                break;
            case 2:
            //Modify customer
                modify_customer_info(connFD);
                break;
            case 3:
                process_loan(connFD);
                break;
            case 4:
                view_assgn_loan(connFD);
                break;
            case 5:
            //Change Password
                if(!change_emp_password(connFD, &loggedInEmployee)) break;
            case 6:
            //Logout
                memset(writeBuffer,0,sizeof(writeBuffer));
                strcpy(writeBuffer,"Bye ");
                strcat(writeBuffer,loggedInEmployee.name);
                strcat(writeBuffer,LOGOUT);               
                writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
                return false;
            default:
                writeBytes = write(connFD, INVALID_MENU_CHOICE, strlen(INVALID_MENU_CHOICE));
                read(connFD, readBuffer, sizeof(readBuffer));
            }
            }
            else if(loggedInEmployee.role=='M'){

            strcat(writeBuffer, MANAGER_MENU);
            writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
            if (writeBytes == -1)
            {
                perror("Error while writing EMP_MENU to client!");
                return false;
            }
            memset(writeBuffer, 0, sizeof(writeBuffer));

            readBytes = read(connFD, readBuffer, sizeof(readBuffer));
            if (readBytes == -1)
            {
                perror("Error while reading client's choice for EMP_MENU");
                return false;
            }

            int choice = atoi(readBuffer);
            switch (choice)
            {
            case 1:
            //Activate/Deactivate Customer 
                act_customer(connFD);
                break;
            case 2:
            //Assign Loan to Emp
                assign_loan(connFD);
                break;
            case 3:
            //Review Customer Feedback
                 break;
            case 4:
            //Change Password
                if(!change_emp_password(connFD, &loggedInEmployee)) break;
            case 5:
            //Logout
                memset(writeBuffer,0,sizeof(writeBuffer));
                strcpy(writeBuffer,"Bye ");
                strcat(writeBuffer,loggedInEmployee.name);
                strcat(writeBuffer,LOGOUT);               
                writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
                return false;
            default:
                writeBytes = write(connFD, INVALID_MENU_CHOICE, strlen(INVALID_MENU_CHOICE));
                read(connFD, readBuffer, sizeof(readBuffer));
            }
            }
        }
    }
    else
    {
        //LOGIN FAILED
        return false;
    }
    return true;
}

bool emp_login_handler(int connFD, struct Employee *emp)
{
    ssize_t readBytes, writeBytes;           
    char readBuffer[1000], writeBuffer[1000]; 
    char tempBuffer[1000];

    int ID;

    bzero(readBuffer, sizeof(readBuffer));
    memset(writeBuffer, 0, sizeof(writeBuffer));

    strcpy(writeBuffer, EMP_LOGIN_WELCOME);
  
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

    bzero(tempBuffer, sizeof(tempBuffer));
    strcpy(tempBuffer, readBuffer);
    if (strchr(tempBuffer, '-') != NULL)
    {
        strtok(tempBuffer, "-");
        ID = atoi(strtok(NULL, "-"));
    }
	else
	{
        writeBytes = write(connFD, EMP_LOGIN_ID_DOESNT_EXIST, strlen(EMP_LOGIN_ID_DOESNT_EXIST));
        return false;
	}
    int empFileFD = open(EMP_FILE, O_RDONLY);
    if (empFileFD == -1)
    {
        perror("Error opening emp file in read mode!");
        return false;
    }

    off_t offset = lseek(empFileFD, ID * sizeof(struct Employee), SEEK_SET);
    if (offset >= 0)
    {
        struct flock lock = {F_RDLCK, SEEK_SET, ID * sizeof(struct Employee), sizeof(struct Employee), getpid()};

        int lockingStatus = fcntl(empFileFD, F_SETLKW, &lock);
        if (lockingStatus == -1)
        {
            perror("Error obtaining read lock on emp record!");
            return false;
        }

        readBytes = read(empFileFD, emp, sizeof(struct Employee));
        if (readBytes == -1)
        {
            perror("Error reading emp record from file!");
        }

        lock.l_type = F_UNLCK;
        fcntl(empFileFD, F_SETLK, &lock);
        if (strcmp(emp->login, readBuffer) == 0)
        {
            userFound = true;
            close(empFileFD);
        }
        else
        {
            writeBytes = write(connFD, EMP_LOGIN_ID_DOESNT_EXIST, strlen(EMP_LOGIN_ID_DOESNT_EXIST));
            return false;
        }
    }

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

            if (strcmp(readBuffer, emp->password) == 0)
            {
                return true;
            }
            else
            {
                memset(readBuffer, 0, sizeof(readBuffer));
                writeBytes = write(connFD, INVALID_PASSWORD, strlen(INVALID_PASSWORD));
                //read(connFD, readBuffer, sizeof(readBuffer));                
            }      
    }
    else
    {
        memset(writeBuffer, 0, sizeof(writeBuffer));
        writeBytes = write(connFD, INVALID_LOGIN, strlen(INVALID_LOGIN));
    }

    return false;
}

bool add_account(int connFD)
{
    ssize_t readBytes, writeBytes;
    char readBuffer[1000], writeBuffer[1000];

    struct Account newAccount, prevAccount;

    int accountFileDescriptor = open(ACCOUNT_FILE, O_RDONLY);
    if (accountFileDescriptor == -1 && errno == ENOENT)
    {
        // Account file was never created
        newAccount.accountNumber = 0;
    }
    else if (accountFileDescriptor == -1)
    {
        perror("Error while opening account file");
        return false;
    }
    else
    {
        int offset = lseek(accountFileDescriptor, -sizeof(struct Account), SEEK_END);
        if (offset == -1)
        {
            perror("Error seeking to last Account record!");
            return false;
        }

        struct flock lock = {F_RDLCK, SEEK_SET, offset, sizeof(struct Account), getpid()};
        int lockingStatus = fcntl(accountFileDescriptor, F_SETLKW, &lock);
        if (lockingStatus == -1)
        {
            perror("Error obtaining read lock on Account record!");
            return false;
        }

        readBytes = read(accountFileDescriptor, &prevAccount, sizeof(struct Account));
        if (readBytes == -1)
        {
            perror("Error while reading Account record from file!");
            return false;
        }

        lock.l_type = F_UNLCK;
        fcntl(accountFileDescriptor, F_SETLK, &lock);

        close(accountFileDescriptor);

        newAccount.accountNumber = prevAccount.accountNumber + 1;
    }
    
    newAccount.active = true;
    newAccount.balance = 0;

    memset(newAccount.transactions, -1, MAX_TRANSACTIONS * sizeof(int));

    accountFileDescriptor = open(ACCOUNT_FILE, O_CREAT | O_APPEND | O_WRONLY, S_IRWXU);
    if (accountFileDescriptor == -1)
    {
        perror("Error while creating / opening account file!");
        return false;
    }

    writeBytes = write(accountFileDescriptor, &newAccount, sizeof(struct Account));
    if (writeBytes == -1)
    {
        perror("Error while writing Account record to file!");
        return false;
    }

    close(accountFileDescriptor);

    add_customer(connFD,newAccount.accountNumber);

    memset(writeBuffer, 0, sizeof(writeBuffer));
    sprintf(writeBuffer, "%s%d", EMP_ADD_ACCOUNT_NUMBER, newAccount.accountNumber);
    strcat(writeBuffer, "\nRedirecting you to the main menu ...^");
    writeBytes = write(connFD, writeBuffer, sizeof(writeBuffer));
    readBytes = read(connFD, readBuffer, sizeof(read)); // Dummy read
    return true;
}

int add_customer(int connFD, int newAccountNumber)
{
    ssize_t readBytes, writeBytes;
    char readBuffer[1000], writeBuffer[1000];

    struct Customer newCustomer, previousCustomer;

    int customerFileDescriptor = open(CUSTOMER_FILE, O_RDONLY);
    if (customerFileDescriptor == -1 && errno == ENOENT)
    {
        // Customer file was never created
        newCustomer.id = 0;
    }
    else if (customerFileDescriptor == -1)
    {
        perror("Error while opening customer file");
        return -1;
    }
    else
    {
        int offset = lseek(customerFileDescriptor, -sizeof(struct Customer), SEEK_END);
        if (offset == -1)
        {
            perror("Error seeking to last Customer record!");
            return false;
        }

        struct flock lock = {F_RDLCK, SEEK_SET, offset, sizeof(struct Customer), getpid()};
        int lockingStatus = fcntl(customerFileDescriptor, F_SETLKW, &lock);
        if (lockingStatus == -1)
        {
            perror("Error obtaining read lock on Customer record!");
            return false;
        }

        readBytes = read(customerFileDescriptor, &previousCustomer, sizeof(struct Customer));
        if (readBytes == -1)
        {
            perror("Error while reading Customer record from file!");
            return false;
        }

        lock.l_type = F_UNLCK;
        fcntl(customerFileDescriptor, F_SETLK, &lock);

        close(customerFileDescriptor);

        newCustomer.id = previousCustomer.id + 1;
    }

    sprintf(writeBuffer, "%s%s", EMP_ADD_CUSTOMER, EMP_ADD_CUSTOMER_NAME);

    writeBytes = write(connFD, writeBuffer, sizeof(writeBuffer));
    if (writeBytes == -1)
    {
        perror("Error writing EMP_ADD_CUSTOMER_NAME message to client!");
        return false;
    }

    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1)
    {
        perror("Error reading customer name response from client!");
        ;
        return false;
    }

    strcpy(newCustomer.name, readBuffer);

    writeBytes = write(connFD, EMP_ADD_CUSTOMER_GENDER, strlen(EMP_ADD_CUSTOMER_GENDER));
    if (writeBytes == -1)
    {
        perror("Error writing EMP_ADD_CUSTOMER_GENDER message to client!");
        return false;
    }

    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1)
    {
        perror("Error reading customer gender response from client!");
        return false;
    }

    if (readBuffer[0] == 'M' || readBuffer[0] == 'F' || readBuffer[0] == 'O')
        newCustomer.gender = readBuffer[0];
    else
    {
        writeBytes = write(connFD, EMP_ADD_CUSTOMER_WRONG_GENDER, strlen(EMP_ADD_CUSTOMER_WRONG_GENDER));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        return false;
    }

    memset(writeBuffer, 0, sizeof(writeBuffer));
    strcpy(writeBuffer, EMP_ADD_CUSTOMER_AGE);
    writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
    if (writeBytes == -1)
    {
        perror("Error writing EMP_ADD_CUSTOMER_AGE message to client!");
        return false;
    }

    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1)
    {
        perror("Error reading customer age response from client!");
        return false;
    }

    int customerAge = atoi(readBuffer);
    if (customerAge == 0)
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
    newCustomer.age = customerAge;

    newCustomer.account = newAccountNumber;

    newCustomer.loan_id=-1;

    newCustomer.active=1;

    strcpy(newCustomer.login, newCustomer.name);
    strcat(newCustomer.login, "-");
    sprintf(writeBuffer, "%d", newCustomer.id);
    strcat(newCustomer.login, writeBuffer);

    char hashedPassword[1000];
    //strcpy(hashedPassword, crypt(AUTOGEN_PASSWORD, SALT_BAE));
    strcpy(hashedPassword, CUST_AUTOGEN_PASSWORD);
    char idStr[5]; // Enough space for an integer as a string
    snprintf(idStr, sizeof(idStr), "%d", newCustomer.id); // Convert id to string

    strcat(hashedPassword, idStr);
    strcpy(newCustomer.password, hashedPassword);

    customerFileDescriptor = open(CUSTOMER_FILE, O_CREAT | O_APPEND | O_WRONLY, S_IRWXU);
    if (customerFileDescriptor == -1)
    {
        perror("Error while creating / opening customer file!");
        return false;
    }
    writeBytes = write(customerFileDescriptor, &newCustomer, sizeof(newCustomer));
    if (writeBytes == -1)
    {
        perror("Error while writing Customer record to file!");
        return false;
    }

    close(customerFileDescriptor);

    memset(writeBuffer, 0, sizeof(writeBuffer));
    sprintf(writeBuffer, "%s%s-%d\n%s%s", EMP_ADD_CUSTOMER_AUTOGEN_LOGIN, newCustomer.name, newCustomer.id, EMP_ADD_CUSTOMER_AUTOGEN_PASSWORD, newCustomer.password);
    strcat(writeBuffer, "^");
    writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
    if (writeBytes == -1)
    {
        perror("Error sending customer loginID and password to the client!");
        return false;
    }

    readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read

    return newCustomer.id;
}

bool modify_customer_info(int connFD)
{
    ssize_t readBytes, writeBytes;
    char readBuffer[1000], writeBuffer[1000];

    struct Customer customer;

    int customerID;

    off_t offset;
    int lockingStatus;

    writeBytes = write(connFD, EMP_MOD_CUSTOMER_ID, strlen(EMP_MOD_CUSTOMER_ID));
    if (writeBytes == -1)
    {
        perror("Error while writing EMP_MOD_CUSTOMER_ID message to client!");
        return false;
    }
    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1)
    {
        perror("Error while reading customer ID from client!");
        return false;
    }

    customerID = atoi(readBuffer);

    int customerFileDescriptor = open(CUSTOMER_FILE, O_RDONLY);
    if (customerFileDescriptor == -1)
    {
        // Customer File doesn't exist
        memset(writeBuffer, 0, sizeof(writeBuffer));
        strcpy(writeBuffer, CUSTOMER_ID_DOESNT_EXIST);
        strcat(writeBuffer, "^");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1)
        {
            perror("Error while writing CUSTOMER_ID_DOESNT_EXIST message to client!");
            return false;
        }
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        return false;
    }
    
    offset = lseek(customerFileDescriptor, customerID * sizeof(struct Customer), SEEK_SET);
    if (errno == EINVAL)
    {
        // Customer record doesn't exist
        memset(writeBuffer, 0, sizeof(writeBuffer));
        strcpy(writeBuffer, CUSTOMER_ID_DOESNT_EXIST);
        strcat(writeBuffer, "^");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1)
        {
            perror("Error while writing CUSTOMER_ID_DOESNT_EXIST message to client!");
            return false;
        }
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        return false;
    }
    else if (offset == -1)
    {
        perror("Error while seeking to required customer record!");
        return false;
    }

    struct flock lock = {F_RDLCK, SEEK_SET, offset, sizeof(struct Customer), getpid()};

    // Lock the record to be read
    lockingStatus = fcntl(customerFileDescriptor, F_SETLKW, &lock);
    if (lockingStatus == -1)
    {
        perror("Couldn't obtain lock on customer record!");
        return false;
    }

    readBytes = read(customerFileDescriptor, &customer, sizeof(struct Customer));
    if (readBytes == -1)
    {
        perror("Error while reading customer record from the file!");
        return false;
    }

    // Unlock the record
    lock.l_type = F_UNLCK;
    fcntl(customerFileDescriptor, F_SETLK, &lock);

    close(customerFileDescriptor);

    writeBytes = write(connFD, EMP_MOD_CUSTOMER_MENU, strlen(EMP_MOD_CUSTOMER_MENU));
    if (writeBytes == -1)
    {
        perror("Error while writing EMP_MOD_CUSTOMER_MENU message to client!");
        return false;
    }
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1)
    {
        perror("Error while getting customer modification menu choice from client!");
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

    bzero(readBuffer, sizeof(readBuffer));
    switch (choice)
    {
    case 1:
        writeBytes = write(connFD, EMP_MOD_CUSTOMER_NEW_NAME, strlen(EMP_MOD_CUSTOMER_NEW_NAME));
        if (writeBytes == -1)
        {
            perror("Error while writing EMP_MOD_CUSTOMER_NEW_NAME message to client!");
            return false;
        }
        readBytes = read(connFD, &readBuffer, sizeof(readBuffer));
        if (readBytes == -1)
        {
            perror("Error while getting response for customer's new name from client!");
            return false;
        }
        strcpy(customer.name, readBuffer);
        break;
    case 2:
        writeBytes = write(connFD, EMP_MOD_CUSTOMER_NEW_AGE, strlen(EMP_MOD_CUSTOMER_NEW_AGE));
        if (writeBytes == -1)
        {
            perror("Error while writing EMP_MOD_CUSTOMER_NEW_AGE message to client!");
            return false;
        }
        readBytes = read(connFD, &readBuffer, sizeof(readBuffer));
        if (readBytes == -1)
        {
            perror("Error while getting response for customer's new age from client!");
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
        customer.age = updatedAge;
        break;
    case 3:
        writeBytes = write(connFD, EMP_MOD_CUSTOMER_NEW_GENDER, strlen(EMP_MOD_CUSTOMER_NEW_GENDER));
        if (writeBytes == -1)
        {
            perror("Error while writing EMP_MOD_CUSTOMER_NEW_GENDER message to client!");
            return false;
        }
        readBytes = read(connFD, &readBuffer, sizeof(readBuffer));
        if (readBytes == -1)
        {
            perror("Error while getting response for customer's new gender from client!");
            return false;
        }
        customer.gender = readBuffer[0];
        break;
    default:
        memset(writeBuffer, 0, sizeof(writeBuffer));
        strcpy(writeBuffer, INVALID_MENU_CHOICE);
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1)
        {
            perror("Error while writing INVALID_MENU_CHOICE message to client!");
            return false;
        }
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        return false;
    }

    customerFileDescriptor = open(CUSTOMER_FILE, O_WRONLY);
    if (customerFileDescriptor == -1)
    {
        perror("Error while opening customer file");
        return false;
    }
    offset = lseek(customerFileDescriptor, customerID * sizeof(struct Customer), SEEK_SET);
    if (offset == -1)
    {
        perror("Error while seeking to required customer record!");
        return false;
    }

    lock.l_type = F_WRLCK;
    lock.l_start = offset;
    lockingStatus = fcntl(customerFileDescriptor, F_SETLKW, &lock);
    if (lockingStatus == -1)
    {
        perror("Error while obtaining write lock on customer record!");
        return false;
    }

    writeBytes = write(customerFileDescriptor, &customer, sizeof(struct Customer));
    if (writeBytes == -1)
    {
        perror("Error while writing update customer info into file");
    }

    lock.l_type = F_UNLCK;
    fcntl(customerFileDescriptor, F_SETLKW, &lock);

    close(customerFileDescriptor);

    writeBytes = write(connFD, EMP_MOD_CUSTOMER_SUCCESS, strlen(EMP_MOD_CUSTOMER_SUCCESS));
    if (writeBytes == -1)
    {
        perror("Error while writing EMP_MOD_CUSTOMER_SUCCESS message to client!");
        return false;
    }
    readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read

    return true;
}

bool change_emp_password(int connFD, struct Employee* ptr)
{
    ssize_t readBytes, writeBytes;
    char readBuffer[1000], writeBuffer[1000], hashedPassword[1000];
    char newPassword[1000];
    
    // Lock the critical section
    // struct sembuf semOp = {0, -1, SEM_UNDO};
    // int semopStatus = semop(semIdentifier, &semOp, 1);
    // if (semopStatus == -1)
    // {
    //     perror("Error while locking critical section");
    //     return false;
    // }

    writeBytes = write(connFD, PASSWORD_CHANGE_OLD_PASS, strlen(PASSWORD_CHANGE_OLD_PASS));
    if (writeBytes == -1)
    {
        perror("Error writing PASSWORD_CHANGE_OLD_PASS message to client!");
        //unlock_critical_section(&semOp);
        return false;
    }

    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    //readBuffer[strcspn(readBuffer, "\n")] = 0;

    if (readBytes == -1)
    {
        perror("Error reading old password response from client");
        //unlock_critical_section(&semOp);
        return false;
    }
    
    //if (strcmp(crypt(readBuffer, SALT_BAE), ptr.password) == 0)
    if (strcmp(readBuffer, ptr->password) == 0)
    {
        // Password matches with old password
        writeBytes = write(connFD, PASSWORD_CHANGE_NEW_PASS, strlen(PASSWORD_CHANGE_NEW_PASS));
        if (writeBytes == -1)
        {
            perror("Error writing PASSWORD_CHANGE_NEW_PASS message to client!");
            //unlock_critical_section(&semOp);
            return false;
        }
        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));
        if (readBytes == -1)
        {
            perror("Error reading new password response from client");
            //unlock_critical_section(&semOp);
            return false;
        }
        
        //strcpy(newPassword, crypt(readBuffer, SALT_BAE));
        strcpy(newPassword, readBuffer);

        writeBytes = write(connFD, PASSWORD_CHANGE_NEW_PASS_RE, strlen(PASSWORD_CHANGE_NEW_PASS_RE));
        if (writeBytes == -1)
        {
            perror("Error writing PASSWORD_CHANGE_NEW_PASS_RE message to client!");
            //unlock_critical_section(&semOp);
            return false;
        }
        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));
        if (readBytes == -1)
        {
            perror("Error reading new password reenter response from client");
            //unlock_critical_section(&semOp);
            return false;
        }

        //if (strcmp(crypt(readBuffer, SALT_BAE), newPassword) == 0)
        if (strcmp(readBuffer, newPassword) == 0)
        {
            // New & reentered passwords match

            strcpy(ptr->password, newPassword);

            int employeeFileDescriptor = open(EMP_FILE, O_WRONLY);
            if (employeeFileDescriptor == -1)
            {
                perror("Error opening employee file!");
                //unlock_critical_section(&semOp);
                return false;
            }

            off_t offset = lseek(employeeFileDescriptor, ptr->id * sizeof(struct Employee), SEEK_SET);
            if (offset == -1)
            {
                perror("Error seeking to the employee record!");
                //unlock_critical_section(&semOp);
                return false;
            }

            struct flock lock = {F_WRLCK, SEEK_SET, offset, sizeof(struct Employee), getpid()};
            int lockingStatus = fcntl(employeeFileDescriptor, F_SETLKW, &lock);
            if (lockingStatus == -1)
            {
                perror("Error obtaining write lock on employee record!");
                //unlock_critical_section(&semOp);
                return false;
            }

            writeBytes = write(employeeFileDescriptor, ptr, sizeof(struct Employee));
            if (writeBytes == -1)
            {
                perror("Error storing updated employee password into employee record!");
                //unlock_critical_section(&semOp);
                return false;
            }

            lock.l_type = F_UNLCK;
            lockingStatus = fcntl(employeeFileDescriptor, F_SETLK, &lock);

            close(employeeFileDescriptor);

            writeBytes = write(connFD, PASSWORD_CHANGE_SUCCESS, strlen(PASSWORD_CHANGE_SUCCESS));
            readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read

            //unlock_critical_section(&semOp);

            return true;
        }
        else
        {
            // New & reentered passwords don't match
            writeBytes = write(connFD, PASSWORD_CHANGE_NEW_PASS_INVALID, strlen(PASSWORD_CHANGE_NEW_PASS_INVALID));
            readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        }
    }
    else
    {
        // Password doesn't match with old password
        writeBytes = write(connFD, PASSWORD_CHANGE_OLD_PASS_INVALID, strlen(PASSWORD_CHANGE_OLD_PASS_INVALID));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
    }

    //unlock_critical_section(&semOp);
    return false;
}

bool act_customer(int connFD){

    ssize_t readBytes, writeBytes;
    char readBuffer[1000], writeBuffer[1000];

    struct Customer customer;
    int customerID;

    off_t offset;
    int lockingStatus;

    writeBytes = write(connFD, MANAGER_ACT_CUSTOMER, strlen(MANAGER_ACT_CUSTOMER));
    if (writeBytes == -1)
    {
        perror("Error while writing MANAGER_ACT_CUSTOMER message to client!");
        return false;
    }
    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1)
    {
        perror("Error while reading customer ID from client!");
        return false;
    }

    customerID = atoi(readBuffer);

    int customerFileDescriptor = open(CUSTOMER_FILE, O_RDONLY);
    if (customerFileDescriptor == -1)
    {
        // Customer File doesn't exist
        memset(writeBuffer, 0, sizeof(writeBuffer));
        strcpy(writeBuffer, CUSTOMER_ID_DOESNT_EXIST);
        strcat(writeBuffer, "^");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1)
        {
            perror("Error while writing CUSTOMER_ID_DOESNT_EXIST message to client!");
            return false;
        }
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        return false;
    }
    
    offset = lseek(customerFileDescriptor, customerID * sizeof(struct Customer), SEEK_SET);
    if (errno == EINVAL)
    {
        // Customer record doesn't exist
        memset(writeBuffer, 0, sizeof(writeBuffer));
        strcpy(writeBuffer, CUSTOMER_ID_DOESNT_EXIST);
        strcat(writeBuffer, "^");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1)
        {
            perror("Error while writing CUSTOMER_ID_DOESNT_EXIST message to client!");
            return false;
        }
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        return false;
    }
    else if (offset == -1)
    {
        perror("Error while seeking to required customer record!");
        return false;
    }

    struct flock lock = {F_RDLCK, SEEK_SET, offset, sizeof(struct Customer), getpid()};

    // Lock the record to be read
    lockingStatus = fcntl(customerFileDescriptor, F_SETLKW, &lock);
    if (lockingStatus == -1)
    {
        perror("Couldn't obtain lock on customer record!");
        return false;
    }

    readBytes = read(customerFileDescriptor, &customer, sizeof(struct Customer));
    if (readBytes == -1)
    {
        perror("Error while reading customer record from the file!");
        return false;
    }

    // Unlock the record
    lock.l_type = F_UNLCK;
    fcntl(customerFileDescriptor, F_SETLK, &lock);
    close(customerFileDescriptor);

    char status[10];
    strcpy(status,customer.active==1?"ACTIVE":"INACTIVE");
    sprintf(writeBuffer,"Customer Name: %s\nAccount No: %d\nCustomer Status: %s\n",customer.name,customer.account,status);
    strcat(writeBuffer,MANAGER_ACT_CUSTOMER_MENU);

    writeBytes = write(connFD, writeBuffer, sizeof(writeBuffer));
    if (writeBytes == -1)
    {
        perror("Error while writing MANAGER_ACT_CUSTOMER_MENU message to client!");
        return false;
    }
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1)
    {
        perror("Error while getting customer modification menu choice from client!");
        return false;
    }

    int choice = atoi(readBuffer);
    // if (choice == 0)
    // { // A non-numeric string was passed to atoi
    //     memset(writeBuffer, 0, sizeof(writeBuffer));
    //     strcpy(writeBuffer, ERRON_INPUT_FOR_NUMBER);
    //     writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
    //     if (writeBytes == -1)
    //     {
    //         perror("Error while writing ERRON_INPUT_FOR_NUMBER message to client!");
    //         return false;
    //     }
    //     readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
    //     return false;
    // }

    // bzero(readBuffer, sizeof(readBuffer));
    memset(writeBuffer,0,sizeof(writeBuffer));

    switch (choice)
    {
    case 1:
        customer.active=1;
        sprintf(writeBuffer,"%s's account is now activated!\n",customer.name);

        break;
    case 2:
        customer.active=0;
        sprintf(writeBuffer,"%s's account is now deactivated!\n",customer.name);

        break;   
    default:
        memset(writeBuffer, 0, sizeof(writeBuffer));
        strcpy(writeBuffer, INVALID_MENU_CHOICE);
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1)
        {
            perror("Error while writing INVALID_MENU_CHOICE message to client!");
            return false;
        }
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        return false;
    }

    customerFileDescriptor = open(CUSTOMER_FILE, O_WRONLY);
    if (customerFileDescriptor == -1)
    {
        perror("Error while opening customer file");
        return false;
    }
    offset = lseek(customerFileDescriptor, customerID * sizeof(struct Customer), SEEK_SET);
    if (offset == -1)
    {
        perror("Error while seeking to required customer record!");
        return false;
    }

    lock.l_type = F_WRLCK;
    lock.l_start = offset;
    lockingStatus = fcntl(customerFileDescriptor, F_SETLKW, &lock);
    if (lockingStatus == -1)
    {
        perror("Error while obtaining write lock on customer record!");
        return false;
    }

    writeBytes = write(customerFileDescriptor, &customer, sizeof(struct Customer));
    if (writeBytes == -1)
    {
        perror("Error while writing update customer info into file");
    }

    lock.l_type = F_UNLCK;
    fcntl(customerFileDescriptor, F_SETLKW, &lock);
    close(customerFileDescriptor);

    strcat(writeBuffer,EMP_MOD_CUSTOMER_SUCCESS);
    writeBytes = write(connFD, writeBuffer, sizeof(writeBuffer));
    if (writeBytes == -1)
    {
        perror("Error while writing EMP_MOD_CUSTOMER_SUCCESS message to client!");
        return false;
    }
    readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read

    return true;
}

bool assign_loan(int connFD){
    //display unassigned loans
    //enter emp id to assign loan
    //check for emp id exists
    //enter loan id 
    //check loan id exists and empid=-1
    //enter empid in loan record
    //change loan status to pending approval

    ssize_t readBytes, writeBytes, bytesRead;
    char readBuffer[1000], writeBuffer[10000];

    struct Employee emp;
    struct Loan loan;
    int empID,loanID;

    off_t offset;
    int lockingStatus;

    int loanFD = open(LOAN_FILE, O_RDONLY);
    if (loanFD == -1)
        {
            perror("Error while opening loan file");
            return false;
        }

    strcpy(writeBuffer, "Unassigned Loan Details:\n");
    int f=0;
    while ((bytesRead = read(loanFD, &loan, sizeof(struct Loan))) > 0) {
        char buff[1000];
        if(loan.emp_id==-1) {
            f=1;
            sprintf(buff,"Loan ID: %d, Customer ID: %d, Requested Loan Amount: %ld, Approved Loan Amount: %ld, Employee ID: %d, Loan Status: %c\n",loan.loan_id, loan.customer_id, loan.req_loan_amount, loan.app_loan_amount,loan.emp_id,loan.loan_status);
            strcat(writeBuffer,buff);
        }
    }
    if (bytesRead == -1) {
        perror("Error reading loan");
    }
    close(loanFD);
    if(f==0){
        //No Loans unassigned
        strcat(writeBuffer,"No Unassigned Loans! Redirecting to main menu...^");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1)
        {
            perror("Error while writing MANGER_ASK_EMP message to client!");
            return false;
        }
        read(connFD,readBuffer,sizeof(readBuffer));//Dummy read
    }
    else{
        //Ask for Emp ID
        strcat(writeBuffer,MANAGER_ASK_EMP);
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1)
        {
            perror("Error while writing MANGER_ASK_EMP message to client!");
            return false;
        }

        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));
        if (readBytes == -1)
        {
            perror("Error while reading employee ID from client!");
            return false;
        }
        empID = atoi(readBuffer);

        //Ask for Loan ID
        writeBytes = write(connFD, MANAGER_ASK_LOAN, strlen(MANAGER_ASK_LOAN));
        if (writeBytes == -1)
        {
            perror("Error while writing MANGER_ASK_EMP message to client!");
            return false;
        }

        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));
        if (readBytes == -1)
        {
            perror("Error while reading loan ID from client!");
            return false;
        }
        loanID = atoi(readBuffer);

        //Enter Emp id in loan record
        int loanFD = open(LOAN_FILE, O_RDONLY, S_IRWXU);
        if (loanFD == -1)
        {
            perror("Error while creating / opening loan file!");
            return false;
        }
        int offset = lseek(loanFD, loanID*sizeof(struct Loan), SEEK_SET);
        
        struct flock lock = {F_RDLCK, SEEK_SET, offset, sizeof(struct Loan), getpid()};
        int lockingStatus = fcntl(loanFD, F_SETLKW, &lock);
        if (lockingStatus == -1)
        {
            perror("Error obtaining read lock on Loan record!");
            return false;
        }

        readBytes = read(loanFD, &loan, sizeof(struct Loan));
        if (readBytes == -1)
        {
            perror("Error while reading Loan record from file!");
            return false;
        }

        lock.l_type = F_UNLCK;
        fcntl(loanFD, F_SETLK, &lock);
        close(loanFD);

        loan.emp_id=empID;
        loan.loan_status='P';

        loanFD = open(LOAN_FILE, O_WRONLY, S_IRWXU);
        offset = lseek(loanFD, loanID*sizeof(struct Loan), SEEK_SET);
        writeBytes = write(loanFD, &loan, sizeof(struct Loan));
        if (writeBytes == -1)
        {
            perror("Error while writing loan record to file!");
            return false;
        }
        close(loanFD);

        memset(writeBuffer, 0, sizeof(writeBuffer));
        sprintf(writeBuffer, "Loan ID: %d is now assigned to Employee ID:%d\nLoan Status: %c",loan.loan_id,empID,loan.loan_status);
        strcat(writeBuffer, "^");

        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1)
        {
            perror("Error sending loan assigned details to the client!");
            return false;
        }

        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read

        return loan.loan_id;

    }
    return false;
}

bool view_assgn_loan(int connFD){
    ssize_t readBytes, writeBytes, bytesRead;
    char readBuffer[1000], writeBuffer[10000];

    struct Loan loan;
    int empID=loggedInEmployee.id;

    int loanFD = open(LOAN_FILE, O_RDONLY);
    if (loanFD == -1)
        {
            perror("Error while opening loan file");
            return false;
        }

    strcpy(writeBuffer, "Assigned Loan Details:\n");
    int f=0;
    while ((bytesRead = read(loanFD, &loan, sizeof(struct Loan))) > 0) {
        char buff[1000];
        if(loan.emp_id==empID) {
            f=1;
            sprintf(buff,"Loan ID: %d, Customer ID: %d, Requested Loan Amount: %ld, Approved Loan Amount: %ld, Employee ID: %d, Loan Status: %c\n",loan.loan_id, loan.customer_id, loan.req_loan_amount, loan.app_loan_amount,loan.emp_id,loan.loan_status);
            strcat(writeBuffer,buff);
        }
    }
    if (bytesRead == -1) {
        perror("Error reading loan");
    }
    close(loanFD);

    if(f==0){
        //No Loans assigned
        strcat(writeBuffer,"No Assigned Loans!");
    }

    strcat(writeBuffer,"Redirecting to main menu...^");
    writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1)
        {
            perror("Error while writing assigned loan details message to client!");
            return false;
        }
        read(connFD,readBuffer,sizeof(readBuffer));//Dummy read
    
    return false;
}

bool process_loan(int connFD){
    //view assigned loans whose status is pending
    //pick a loan id to process
    //if cust is active
        //if emp enters A then:
            //1.change loan status to approved
            //2.transfer requested amount to cust account
        //else
            //1.change status to rejected
    //display customer inactive
    //change status to rejected

        ssize_t readBytes, writeBytes, bytesRead;
        char readBuffer[1000], writeBuffer[10000];

        struct Loan loan;
        struct Customer customer;
        struct Account account;
        int custID,accountID;

        int empID=loggedInEmployee.id;   

    int loanFD = open(LOAN_FILE, O_RDONLY);
    if (loanFD == -1)
        {
            perror("Error while opening loan file");
            return false;
        }

    strcpy(writeBuffer, "Assigned Loans awaiting approval:\n");
    int f=0;
    while ((bytesRead = read(loanFD, &loan, sizeof(struct Loan))) > 0) {
        char buff[1000];
        if(loan.emp_id==empID && loan.loan_status=='P') {
            f=1;
            sprintf(buff,"Loan ID: %d, Customer ID: %d, Requested Loan Amount: %ld, Approved Loan Amount: %ld, Employee ID: %d, Loan Status: %c\n",loan.loan_id, loan.customer_id, loan.req_loan_amount, loan.app_loan_amount,loan.emp_id,loan.loan_status);
            strcat(writeBuffer,buff);
        }
    }
    if (bytesRead == -1) {
        perror("Error reading loan");
    }
    close(loanFD);

    if(f==0){
        //No Loans assigned
            memset(writeBuffer,0,sizeof(writeBuffer));
            strcat(writeBuffer,"No Pending for Approval Loans!^");
            writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
            if (writeBytes == -1)
            {
                perror("Error while writing loan not assigned to you message to client!");
                return false;
            }
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        return false;
    }
    else{    //Ask for Loan ID
        strcat(writeBuffer,EMP_ASK_LOAN);
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1)
        {
            perror("Error while writing EMP_ASK_LOAN message to client!");
            return false;
        }

        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));
        if (readBytes == -1)
        {
            perror("Error while reading loan ID from client!");
            return false;
        }
        int loanID = atoi(readBuffer);

        //Reading Loan Record
        loanFD = open(LOAN_FILE, O_RDONLY, S_IRWXU);
        if (loanFD == -1)
        {
            perror("Error while creating / opening loan file!");
            return false;
        }
        int offset = lseek(loanFD, loanID*sizeof(struct Loan), SEEK_SET);

        // Loan record doesn't exist
        if (errno == EINVAL) {

        memset(writeBuffer, 0, sizeof(writeBuffer));
        strcpy(writeBuffer, LOAN_ID_DOESNT_EXIST);
        strcat(writeBuffer, "^");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1)
        {
            perror("Error while writing LOAN_ID_DOESNT_EXIST message to client!");
            return false;
        }
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        return false;
    }
        else if (offset == -1)
        {
        perror("Error while seeking to required loan record!");
        return false;
        }
        
        //Read Loan Record
        struct flock lock = {F_RDLCK, SEEK_SET, offset, sizeof(struct Loan), getpid()};
        int lockingStatus = fcntl(loanFD, F_SETLKW, &lock);
        if (lockingStatus == -1)
        {
            perror("Error obtaining read lock on Loan record!");
            return false;
        }
        readBytes = read(loanFD, &loan, sizeof(struct Loan));
        if (readBytes == -1)
        {
            perror("Error while reading Loan record from file!");
            return false;
        }
        lock.l_type = F_UNLCK;
        fcntl(loanFD, F_SETLK, &lock);
        close(loanFD);

        //Entered Loan ID not assigned to Emp
        if(loggedInEmployee.id!=loan.emp_id){
            memset(writeBuffer,0,sizeof(writeBuffer));
            strcpy(writeBuffer,"Loan ID entered isn't assigned to you!^");
            writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
            if (writeBytes == -1)
            {
                perror("Error while writing loan not assigned to you message to client!");
                return false;
            }
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        return false;
        }

        custID=loan.customer_id;
        accountID=loan.customer_id;

        //Reading Account Record
	    int accFD = open(ACCOUNT_FILE, O_RDONLY, S_IRWXU);
        if (accFD == -1)
        {
            perror("Error while creating / opening account file!");
            return false;
        }
        offset = lseek(accFD, accountID*sizeof(struct Account), SEEK_SET);
        lock.l_type = F_RDLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = offset;
        lock.l_len = sizeof(struct Account);
        lock.l_pid = getpid();

        lockingStatus = fcntl(accFD, F_SETLKW, &lock);
        if (lockingStatus == -1)
        {
            perror("Error obtaining read lock on Account record!");
            return false;
        }
        readBytes = read(accFD, &account, sizeof(struct Account));
        if (readBytes == -1)
        {
            perror("Error while reading Account record from file!");
            return false;
        }
        lock.l_type = F_UNLCK;
        fcntl(accFD, F_SETLK, &lock);
        close(accFD);

        //Reading Customer Record
	    int custFD = open(CUSTOMER_FILE, O_RDONLY, S_IRWXU);
        if (custFD == -1)
        {
            perror("Error while creating / opening customer file!");
            return false;
        }
        offset = lseek(custFD, custID*sizeof(struct Customer), SEEK_SET);
        
        lock.l_type = F_RDLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = offset;
        lock.l_len = sizeof(struct Account);
        lock.l_pid = getpid();

        lockingStatus = fcntl(custFD, F_SETLKW, &lock);
        if (lockingStatus == -1)
        {
            perror("Error obtaining read lock on Customer record!");
            return false;
        }
        readBytes = read(custFD, &customer, sizeof(struct Customer));
        if (readBytes == -1)
        {
            perror("Error while reading Customer record from file!");
            return false;
        }
        lock.l_type = F_UNLCK;
        fcntl(custFD, F_SETLK, &lock);
        close(custFD);
        
    //If Customer Active then proceed
        if(customer.active){
        //Reading approval input

        memset(writeBuffer,0,sizeof(writeBuffer));
        strcpy(writeBuffer,EMP_ASK_APPROVAL);
        writeBytes=write(connFD,writeBuffer,strlen(writeBuffer));
        if (writeBytes == -1)
        {
            perror("Error while writing EMP_ASK_APPROVAL message to client!");
            return false;
        }

        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));
        if (readBytes == -1)
        {
            perror("Error while reading approval from client!");
            return false;
        }
        loan.loan_status=readBuffer[0];

        if(loan.loan_status=='A'){
            loan.app_loan_amount=loan.req_loan_amount;
            account.balance+=loan.app_loan_amount;

            //Writing to account file
            accFD = open(ACCOUNT_FILE, O_WRONLY, S_IRWXU);
            offset = lseek(accFD, accountID*sizeof(struct Account), SEEK_SET);
            writeBytes = write(accFD, &account, sizeof(struct Account));
            if (writeBytes == -1)
            {
                perror("Error while writing account record to file!");
                return false;
            }
            close(accFD);
        }
            //Writing to loan file
            loanFD = open(LOAN_FILE, O_WRONLY, S_IRWXU);
            offset = lseek(loanFD, loanID*sizeof(struct Loan), SEEK_SET);
            writeBytes = write(loanFD, &loan, sizeof(struct Loan));
            if (writeBytes == -1)
            {
                perror("Error while writing loan record to file!");
                return false;
            }
            close(loanFD);

        memset(writeBuffer, 0, sizeof(writeBuffer));
        sprintf(writeBuffer, "Loan ID: %d\nLoan Status: %c\nRequestedAmount: %ld\nApproved Amount: %ld\n",loan.loan_id,loan.loan_status,loan.req_loan_amount,loan.app_loan_amount);
        strcat(writeBuffer, "^");

        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1)
        {
            perror("Error sending loan processed details to the client!");
            return false;
        }
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        return true;
    }

    }
}

#endif