#ifndef LOAN_RECORD
#define LOAN_RECORD

struct Loan
{
    int loan_id; // 0, 1, 2 ....
    int customer_id; //Customer id who requested it
    long int req_loan_amount; //Amount requested for
    long int app_loan_amount; //Approved loan amount
    char loan_status; // S- Submitted, P- Pending approval,  A- Approved, R- Rejected
    int emp_id; // Employee assigned to
};

#endif