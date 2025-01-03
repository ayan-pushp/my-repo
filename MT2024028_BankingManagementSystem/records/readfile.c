#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define MAX_TRANSACTIONS 10

struct Transaction {
    int transactionID;
    int accountNumber;
    bool operation; // 0 -> Withdraw, 1 -> Deposit
    long int oldBalance;
    long int newBalance;
    time_t transactionTime;
};

struct Account {
    int accountNumber;
    bool active; // 1 for active, 0 for inactive
    long int balance;
    int transactions[MAX_TRANSACTIONS];//Last MAX no of transaction
};

struct Employee {
    int id;//Employee ID
    char name[25];
    char gender; // M -> Male, F -> Female, O -> Other
    int age;
    char role; // E -> employee, M -> manager
    char login[30];//name-id
    char password[30];
};

struct Customer {
    int id; // 0, 1, 2 ....
    char name[25];
    char gender; // M- Male, F- Female, O- Other
    int age;
    // Login Credentials
    char login[30]; // Format : name-id (name will the first name)
    char password[30];
    // Bank data
    int account; // Account number of the customer
    int loan_id; //Loan id requested
    int active; //1-Active, 0-Inactive
};

struct Loan
{
    int loan_id; // 0, 1, 2 ....
    int customer_id; //Customer id who requested it
    long int req_loan_amount; //Amount requested for
    long int app_loan_amount; //Approved loan amount
    char loan_status; // S- Submitted, P- Pending approval,  A- Approved, R- Rejected
    int emp_id; // Employee assigned to
};

void readTransaction(int fd) {
    struct Transaction t;
    ssize_t bytesRead;

    printf("Reading Transactions:\n");
    while ((bytesRead = read(fd, &t, sizeof(struct Transaction))) > 0) {
     char timeStr[26]; // Buffer to hold the formatted time string
        ctime_r(&t.transactionTime, timeStr); // Convert time to string
        timeStr[strcspn(timeStr, "\n")] = '\0'; // Remove newline character

        printf("Transaction ID: %d, Account: %d, Operation: %d, Old Balance: %ld, New Balance: %ld, Time: %s\n",
               t.transactionID, t.accountNumber, t.operation, t.oldBalance, t.newBalance, timeStr);
    }

    if (bytesRead == -1) {
        perror("Error reading transaction");
    }
}

void readAccount(int fd) {
    struct Account a;
    ssize_t bytesRead;

    printf("\nReading Accounts:\n");
    while ((bytesRead = read(fd, &a, sizeof(struct Account))) > 0) {
        printf("Account Number: %d, Active: %d, Balance: %ld, Transactions: [",
               a.accountNumber, a.active, a.balance);
        for(int i=0;i<10;i++) printf(" %d",a.transactions[i]);
        printf("]\n");
    }

    if (bytesRead == -1) {
        perror("Error reading account");
    }
}

void readEmployee(int fd) {
    struct Employee e;
    ssize_t bytesRead;

    printf("\nReading Employees:\n");
    while ((bytesRead = read(fd, &e, sizeof(struct Employee))) > 0) {
        printf("Employee ID: %d, Name: %s, Gender: %c, Age: %d, Role: %c, Login: %s, Password: %s\n",
               e.id, e.name, e.gender, e.age, e.role,e.login,e.password);
    }

    if (bytesRead == -1) {
        perror("Error reading employee");
    }
}

void readCustomer(int fd) {
    struct Customer c;
    ssize_t bytesRead;

    printf("\nReading Customers:\n");
    while ((bytesRead = read(fd, &c, sizeof(struct Customer))) > 0) {
              printf("Customer ID: %d, Name: %s, Gender: %c, Age: %d, Login: %s, Password: %s, Account: %d, Loan ID: %d, Active: %d\n",c.id, c.name, c.gender, c.age,c.login,c.password,c.account,c.loan_id,c.active);
        
    }

    if (bytesRead == -1) {
        perror("Error reading customer");
    }
}

void readLoan(int fd) {
    struct Loan c;
    ssize_t bytesRead;

    printf("\nReading Loan:\n");
    while ((bytesRead = read(fd, &c, sizeof(struct Loan))) > 0) {
      printf("Loan ID: %d, Customer ID: %d, Requested Loan Amount: %ld, Approved Loan Amount: %ld, Employee ID: %d, Loan Status: %c\n",c.loan_id, c.customer_id, c.req_loan_amount, c.app_loan_amount,c.emp_id,c.loan_status);
    }

    if (bytesRead == -1) {
        perror("Error reading loan");
    }
}
int main(int argc, char *argv[]) {
    if (argc < 6) {
        fprintf(stderr, "Usage: %s <transaction_file> <account_file> <employee_file> <customer_file> <loan_file>\n", argv[0]);
        return 1;
    }

    int fd;

    // Read Transactions
    fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        perror("Error opening transaction file");
        return 1;
    }
    readTransaction(fd);
    close(fd);
    
    // Read Accounts
    fd = open(argv[2], O_RDONLY);
    if (fd == -1) {
        perror("Error opening account file");
        return 1;
    }
    readAccount(fd);
    close(fd);

    // Read Employees
    fd = open(argv[3], O_RDONLY);
    if (fd == -1) {
        perror("Error opening employee file");
        return 1;
    }
    readEmployee(fd);
    close(fd);

    // Read Customers
    fd = open(argv[4], O_RDONLY);
    if (fd == -1) {
        perror("Error opening customer file");
        return 1;
    }
    readCustomer(fd);
    close(fd);
    
    //Read Loans
    fd = open(argv[5], O_RDONLY);
    if (fd == -1) {
        perror("Error opening loan file");
        return 1;
    }
    readLoan(fd);
    close(fd);

    return 0;
}

