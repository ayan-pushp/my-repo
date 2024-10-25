#ifndef CUSTOMER_RECORD
#define CUSTOMER_RECORD

struct Customer
{
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
    int active; //1-Active, 2-Inactive
};

#endif