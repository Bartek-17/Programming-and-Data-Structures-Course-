#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "account.h"

void displayMenu()
{
    printf("1. Create New Account\n");
    printf("2. List All Accounts\n");
    printf("3. Search For Account\n");
    printf("4. Deposit Money\n");
    printf("5. Withdraw Money\n");
    printf("6. Transfer Money\n");
    printf("7. Take Loan\n");
    printf("8. Pay Debt\n");
    printf("9. Exit\n");
    printf("Enter your choice: ");
}

int main()
{
    int choice;
    while (1)
    {
        displayMenu();
        scanf("%d", &choice);
        getchar();

        switch (choice)
        {
        case 1:
            createAccount();
            break;
        case 2:
            listAccounts();
            break;
        case 3:
            searchAccount();
            break;
        case 4:
            depositMoney();
            break;
        case 5:
            withdrawMoney();
            break;
        case 6:
            transferMoney();
            break;
        case 7:
            takeLoan();
            break;
        case 8:
            payDebt();
            break;
        case 9:
            printf("Exiting...\n");
            return 0;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    }
    return 0;
}
