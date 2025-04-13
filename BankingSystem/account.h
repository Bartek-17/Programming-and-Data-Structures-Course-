#ifndef ACCOUNT_H
#define ACCOUNT_H

int getNextAccountNumber();
int isAllAlpha(const char *str);
int isAllDigits(const char *str);
char* trimWhitespace(char* str);
int confirmOperation();

void createAccount();
void listAccounts();
void searchAccount();
void depositMoney();
void withdrawMoney();
void transferMoney();
void takeLoan();
void payDebt();

#endif
