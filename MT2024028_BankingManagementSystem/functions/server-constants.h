#ifndef SERVER_CONSTANTS
#define SERVER_CONSTANTS


#define INITIAL_PROMPT "==========================================================\nHello User!\nPlease enter your role\n1. Customer\n2. Admin\n3. Employee\n4. Manager\n\nPress any other number to exit..."

//COMMON TEXT

// LOGIN
#define LOGIN_ID "Enter your login ID"
#define PASSWORD "Enter your password \n# "
#define INVALID_LOGIN "Login ID entered doesn't exist!$"
#define INVALID_PASSWORD "Password entered doesn't match!$"
//#define SALT_BAE "666"

// GET ACCOUNT DETAILS
#define GET_ACCOUNT_NUMBER "Enter account number:"
#define GET_CUSTOMER_ID "Enter customer ID:"

#define ERRON_INPUT_FOR_NUMBER "Invalid number!\nRedirecting to main menu...!^"

#define INVALID_MENU_CHOICE "Invalid choice\nRedirecting to main menu...!^"

#define CUSTOMER_ID_DOESNT_EXIST "No customer found for this ID!"
#define CUSTOMER_LOGIN_ID_DOESNT_EXIST "No customer found for this login ID!$"

#define EMP_ID_DOESNT_EXIST "No employee found for this ID!"
#define EMP_LOGIN_ID_DOESNT_EXIST "No employee found for this login ID!$"

#define ACCOUNT_ID_DOESNT_EXIST "No account found for this account number!"

#define TRANSACTIONS_NOT_FOUND "No transactions found for this account!^"

// ADMIN TEXT

// LOGIN WELCOME
#define ADMIN_LOGIN_WELCOME "Please enter your admin credentials:"
#define ADMIN_LOGIN_SUCCESS "Welcome ADMIN!"

// ADMIN MENU
#define ADMIN_MENU "1. Add New Employee\n2. Modify Employee Details\n3. Logout\nPlease enter your choice"
// ADD EMPLOYEE
#define ADMIN_ADD_EMPLOYEE "Enter details for the employee\n"
#define ADMIN_ADD_EMPLOYEE_NAME "Employee Name?"
#define ADMIN_ADD_EMPLOYEE_GENDER "Gender?\n(M for male, F for female and O for others)"
#define ADMIN_ADD_EMPLOYEE_ROLE "Role?\n(E for Employee or M for Manager)"
#define ADMIN_ADD_EMPLOYEE_AGE "Age?"
#define ADMIN_ADD_EMPLOYEE_AUTOGEN_LOGIN "Login ID for the employee is : "
#define EMP_AUTOGEN_PASSWORD "epass" // append to end of next string macro
#define ADMIN_ADD_EMPLOYEE_AUTOGEN_PASSWORD "Password for the employee is : "

// MODIFY EMPLOYEE INFO
#define ADMIN_MOD_EMPLOYEE_ID "Enter the Employee ID to edit"
#define ADMIN_MOD_EMPLOYEE_MENU "What to modify?\nPlease select: 1. Name 2. Age 3. Gender 4. Role\nPress any other key to cancel"
#define ADMIN_MOD_EMPLOYEE_NEW_NAME "New name?"
#define ADMIN_MOD_EMPLOYEE_NEW_GENDER "New gender?(M for male, F for female and O for others)"
#define ADMIN_MOD_EMPLOYEE_NEW_AGE "New age?"
#define ADMIN_MOD_EMPLOYEE_NEW_ROLE "New Role?\n(E for Employee or M for Manager)"
#define ADMIN_MOD_EMPLOYEE_SUCCESS "Successfully modified!\nRedirecting to main menu!^"

#define ADMIN_LOGOUT "Logging you out ADMIN...!$"

//CUSTOMER TEXT

// LOGIN WELCOME
#define CUSTOMER_LOGIN_WELCOME "Dear Customer, please enter your credentials!"
#define CUSTOMER_LOGIN_SUCCESS ", Welcome! Enter your choice:"

#define CUSTOMER_LOGOUT ", logging you out...!$"

// CUSTOMER MENU
#define CUSTOMER_MENU "1. View Account Details\n2. View Balance\n3. Deposit Money\n4. Withdraw Money\n5. Transfer Money\n6. Apply Loan\n7. Change Password\n8. Add Feedback\n9. View Transactions\n10. Logout\nPlease enter your choice"

#define ACCOUNT_DEACTIVATED "Your account is deactivated!^"

#define DEPOSIT_AMOUNT "Amount to deposit?"
#define DEPOSIT_AMOUNT_INVALID "Invalid amount!^"
#define DEPOSIT_AMOUNT_SUCCESS "Successfully deposited money!^"

#define WITHDRAW_AMOUNT "Amount to withdraw?"
#define WITHDRAW_AMOUNT_INVALID "Entered invalid amount^"
#define WITHDRAW_AMOUNT_SUCCESS "Successfully withdrawn money!^"

#define PASSWORD_CHANGE_OLD_PASS "Enter old password"
#define PASSWORD_CHANGE_OLD_PASS_INVALID "The entered password doesn't match with the old password"
#define PASSWORD_CHANGE_NEW_PASS "Enter new password"
#define PASSWORD_CHANGE_NEW_PASS_RE "Reenter new password"
#define PASSWORD_CHANGE_NEW_PASS_INVALID "New password and reentered passwords don't match!^"
#define PASSWORD_CHANGE_SUCCESS "Password successfully changed!^"

// EMPLOYEE TEXT

// LOGIN WELCOME
#define EMP_LOGIN_WELCOME "Dear Employee, please enter your credentials:"
#define EMP_LOGIN_SUCCESS ", Welcome!\nEnter your choice:"

// EMP MENU
#define EMP_MENU "1. Add New Customer\n2. Modify Customer Details\n3. Process Loan\n4. View Assigned Loan\n5. Change Password\n6. Logout\nPlease enter your choice"

// ADD CUSTOMER
#define EMP_ADD_CUSTOMER "Enter the details of the customer\n"
#define EMP_ADD_CUSTOMER_NAME "Name?"
#define EMP_ADD_CUSTOMER_GENDER "Gender?\nEnter M for male, F for female and O for others"
#define EMP_ADD_CUSTOMER_AGE "Age?"
#define EMP_ADD_CUSTOMER_AUTOGEN_LOGIN "Login ID for the customer is : "
#define CUST_AUTOGEN_PASSWORD "cpass" // append to end of next string macro
#define EMP_ADD_CUSTOMER_AUTOGEN_PASSWORD "Password for the customer is : "
#define EMP_ADD_CUSTOMER_WRONG_GENDER "Invalid choice!\nRedirecting to main menu!^"

// ADD ACCOUNT
#define EMP_ADD_ACCOUNT_NUMBER "The newly created account's number is :"

// DELETE ACCOUNT
#define EMP_DEL_ACCOUNT_NO "Account number to delete?"
#define EMP_DEL_ACCOUNT_SUCCESS "Account has been successfully deleted!\nRedirecting to main menu ...^"
#define EMP_DEL_ACCOUNT_FAILURE "Account cannot be deleted since it still has some money\nRedirecting you to the main menu ...^"

// MODIFY CUSTOMER INFO
#define EMP_MOD_CUSTOMER_ID "Enter customer ID to edit"
#define EMP_MOD_CUSTOMER_MENU "What do you want to modify?\n1. Name 2. Age 3. Gender \nPress any other key to cancel"
#define EMP_MOD_CUSTOMER_NEW_NAME "New name?"
#define EMP_MOD_CUSTOMER_NEW_GENDER "New gender?"
#define EMP_MOD_CUSTOMER_NEW_AGE "New age?"
#define EMP_MOD_CUSTOMER_SUCCESS "Successfully modified customer!\nRedirecting to main menu...!^"
#define LOGOUT ", logging you out...!$"

// DATABASE FILES

#define EMP_FILE "./records/emp.bank"
#define ACCOUNT_FILE "./records/account.bank"
#define CUSTOMER_FILE "./records/customer.bank"
#define TRANSACTION_FILE "./records/transactions.bank"
#define LOAN_FILE "./records/loan.bank"

#endif