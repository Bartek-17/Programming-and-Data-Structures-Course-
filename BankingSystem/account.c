#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define FILENAME "accounts.txt"
#define INTEREST_RATE 0.1
#define PESEL_LENGTH 11

#define HEADER "  Account_Nr  |      Name       |      Surname      |       Address        |    PESEL     |  Balance   | Loan_Balance | Loan_Interest \n"
#define LINE_FORMAT "  %-11d | %-15s | %-17s | %-20s | %-12s | %-10.2f | %-12.2f | %-13.2f\n"
#define SEPARATOR "--------------------------------------------------------------------------------------------------------------------------------------\n"

int confirmOperation()
{
    char confirmation[10];
    printf("Do you want to proceed with this operation? (yes): ");
    fgets(confirmation, sizeof(confirmation), stdin);
    if (strcmp(confirmation, "yes\n") == 0)
    {
        return 1;
    }
    else
    {
        printf("Operation cancelled.\n");
        return 0;
    }
}

char *trimWhitespace(char *str)
{
    char *end;

    // Trim leading space
    while (isspace((unsigned char)*str))
        str++;

    if (*str == 0) // All spaces
        return str;

    // Trim trailing space
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end))
        end--;

    // Write new null terminator character
    end[1] = '\0';

    return str;
}

int getNextAccountNumber()
{
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL)
    {
        return 1;
    }

    int maxAccountNumber = 0;
    char line[256];
    fgets(line, sizeof(line), file); // Read and ignore the header
    fgets(line, sizeof(line), file); // --||-- separator

    while (fgets(line, sizeof(line), file))
    {
        int accountNumber;
        sscanf(line, " %d", &accountNumber);
        if (accountNumber > maxAccountNumber)
        {
            maxAccountNumber = accountNumber;
        }
    }

    fclose(file);
    return maxAccountNumber + 1;
}

int isAllAlpha(const char *str)
{
    while (*str)
    {
        if (!isalpha((unsigned char)*str))
        {
            return 0;
        }
        str++;
    }
    return 1; // All characters are alphabetic
}

int isAllDigits(const char *str)
{
    while (*str)
    {
        if (!isdigit(*str))
        {
            return 0;
        }
        str++;
    }
    return 1; // All characters are digits
}

void createAccount()
{
    int accountNumber = getNextAccountNumber();
    char name[50], surname[50], address[50], pesel[12];
    double balance;

    while (1)
    {
        printf("Enter name: ");
        fgets(name, sizeof(name), stdin);
        name[strcspn(name, "\n")] = 0; // Remove newline character
        if (name[0] == '\0')
        {
            printf("Name cannot be empty.\n");
            continue;
        }
        if (!isAllAlpha(name))
        {
            printf("Name must contain alphabetic characters only.\n");
            continue;
        }

        if (strlen(name) > 15)
        {
            printf("Name len can be 15 at most.\n");
            int c;
            while ((c = getchar()) != '\n' && c != EOF)
            {
            }
            continue;
        }
        break;
    }

    while (1)
    {
        printf("Enter surname: ");
        fgets(surname, sizeof(surname), stdin);
        surname[strcspn(surname, "\n")] = 0;
        if (surname[0] == '\0')
        {
            printf("Surname cannot be empty.\n");
            continue;
        }
        if (!isAllAlpha(surname))
        {
            printf("Surname must contain alphabetic characters only.\n");
            continue;
        }

        if (strlen(surname) > 20)
        {
            printf("Surname len can be 20 at most.\n");
            int c;
            while ((c = getchar()) != '\n' && c != EOF)
            {
            }
            continue;
        }

        break;
    }

    while (1)
    {
        printf("Enter address: ");
        fgets(address, sizeof(address), stdin);
        address[strcspn(address, "\n")] = 0;

        if (strlen(surname) > 20)
        {
            printf("Surname len can be 20 at most.\n");
            int c;
            while ((c = getchar()) != '\n' && c != EOF)
            {
            }
            continue;
        }

        if (address[0] == '\0')
        {
            printf("Address cannot be empty.\n");
            continue;
        }

        break;
    }

    while (1)
    {
        printf("Enter PESEL: ");
        fgets(pesel, sizeof(pesel), stdin);
        pesel[strcspn(pesel, "\n")] = 0;
        if (pesel[0] == '\0')
        {
            printf("PESEL cannot be empty.\n");
            continue;
        }
        if (!isAllDigits(pesel))
        {
            printf("PESEL must consist of only digits\n");
            continue;
        }
        if (strlen(pesel) != PESEL_LENGTH)
        {
            printf("PESEL must be exactly %d digits long.\n", PESEL_LENGTH);
            continue;
        }
        break;
    }

    // Clear remaining input from pesel
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;

    while (1)
    {
        char balanceInput[50];
        int dotCount = 0;
        int isValid = 1;

        printf("Enter balance: ");
        fgets(balanceInput, sizeof(balanceInput), stdin);
        balanceInput[strcspn(balanceInput, "\n")] = 0;
        if (balanceInput[0] == '\0')
        {
            printf("Balance cannot be empty.\n");
            continue;
        }

        if (strlen(balanceInput) > 15)
        {
            printf("balance len can be 15 at most.\n");
            int c;
            while ((c = getchar()) != '\n' && c != EOF)
            {
            }
            continue;
        }

        for (int i = 0; balanceInput[i] != '\0'; i++)
        {
            if (!isdigit(balanceInput[i]))
            {
                if (balanceInput[i] == '.' && !dotCount)
                {
                    dotCount = 1;
                }
                else
                {
                    isValid = 0;
                    printf("Invalid balance input. Please enter ints/floats only.\n");
                    break;
                }
            }
        }

        if (isValid)
        {
            balance = atof(balanceInput); // Convert string to double
            break;
        }
    }

    FILE *file = fopen(FILENAME, "a");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    // fseek moves the file pointer to a specific location in the file
    fseek(file, 0, SEEK_END);
    if (ftell(file) == 0)
    { // cursor position in a file
        fprintf(file, HEADER);
        fprintf(file, SEPARATOR);
    }

    if (confirmOperation() == 1)
    {
        fprintf(file, LINE_FORMAT, accountNumber, name, surname, address, pesel, balance, 0.0, INTEREST_RATE);
        fclose(file);
        printf("Account Nr %d created\n", accountNumber);
    }
    else
    {
        fclose(file);
        return;
    }
}

void listAccounts()
{
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }
    char line[256];
    while (fgets(line, sizeof(line), file))
    {
        printf("%s", line);
    }
    fclose(file);
}

void searchAccount()
{
    int searchOption;
    char searchTerm[50];

    printf("Search by:\n");
    printf("1. Account Number\n");
    printf("2. Name\n");
    printf("3. Surname\n");
    printf("4. Address\n");
    printf("5. PESEL\n");
    printf("Enter your choice: ");
    scanf("%d", &searchOption);
    getchar();

    printf("Enter search term: ");
    fgets(searchTerm, sizeof(searchTerm), stdin);
    searchTerm[strcspn(searchTerm, "\n")] = 0;

    // Trim search term
    char *trimmedSearchTerm = trimWhitespace(searchTerm);

    FILE *file = fopen(FILENAME, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    printf(HEADER);
    printf(SEPARATOR);

    char line[256];
    int found = 0;

    while (fgets(line, sizeof(line), file))
    {
        int accountNum;
        char name[16], surname[18], address[21], pesel[12];
        double balance, loanBalance, loanInterestRate;

        // strstr searches one string in another one
        if (strstr(line, "Account_Nr") != NULL || strstr(line, "-------") != NULL)
        {
            continue;
        }

        int sscanfResult = sscanf(line, " %d | %15[^|] | %17[^|] | %20[^|] | %11[^|] | %lf | %lf | %lf",
                                  &accountNum, name, surname, address, pesel, &balance, &loanBalance, &loanInterestRate);

        if (sscanfResult != 8)
        {
            continue;
        }

        char *trimmedName = trimWhitespace(name);
        char *trimmedSurname = trimWhitespace(surname);
        char *trimmedAddress = trimWhitespace(address);
        char *trimmedPesel = trimWhitespace(pesel);

        switch (searchOption)
        {
        case 1:
        {
            int searchAccountNumber = atoi(trimmedSearchTerm);
            if (accountNum == searchAccountNumber)
            {
                found = 1;
            }
            break;
        }
        // case-insensitive comparisons
        case 2:
            if (strcasecmp(trimmedName, trimmedSearchTerm) == 0)
            {
                found = 1;
            }
            break;
        case 3:
            if (strcasecmp(trimmedSurname, trimmedSearchTerm) == 0)
            {
                found = 1;
            }
            break;
        case 4:
            if (strcasecmp(trimmedAddress, trimmedSearchTerm) == 0)
            {
                found = 1;
            }
            break;
        case 5:
            if (strcmp(trimmedPesel, trimmedSearchTerm) == 0)
            {
                found = 1;
            }
            break;
        default:
            printf("Invalid search option.\n");
            fclose(file);
            return;
        }

        if (found)
        {
            printf(LINE_FORMAT, accountNum, trimmedName, trimmedSurname, trimmedAddress, trimmedPesel, balance, loanBalance, loanInterestRate);
            found = 0; // reset for multiple matches
        }
    }

    if (!found)
    {
        printf("No matching account.\n");
    }

    fclose(file);
}

void depositMoney()
{
    char accountInput[50];
    int accountNumber;
    char depositInput[50];
    double depositAmount;
    int found = 0;

    printf("Enter account number: ");
    fgets(accountInput, sizeof(accountInput), stdin);
    accountInput[strcspn(accountInput, "\n")] = 0;

    if (!isAllDigits(accountInput))
    {
        printf("Invalid account number input. Please enter a valid number.\n");
        return;
    }
    accountNumber = atoi(accountInput);

    FILE *file = fopen(FILENAME, "r+");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    char line[256];
    long pos = 0;
    while (fgets(line, sizeof(line), file))
    {
        int accountNum;
        char name[16], surname[18], address[21], pesel[12];
        double balance, loanBalance, loanInterestRate;

        sscanf(line, " %d | %15[^|] | %17[^|] | %20[^|] | %11[^|] | %lf | %lf | %lf",
               &accountNum, name, surname, address, pesel, &balance, &loanBalance, &loanInterestRate);

        if (accountNum == accountNumber)
        {
            found = 1;
            printf("Enter deposit amount: ");
            fgets(depositInput, sizeof(depositInput), stdin);
            depositInput[strcspn(depositInput, "\n")] = 0;

            // Digit only check
            int dotCount = 0;
            for (int i = 0; depositInput[i] != '\0'; i++)
            {
                if (!isdigit(depositInput[i]))
                {
                    if (depositInput[i] == '.' && !dotCount)
                    {
                        dotCount = 1;
                    }
                    else
                    {
                        printf("Invalid deposit input. Please enter a valid number.\n");
                        return;
                    }
                }
            }

            depositAmount = atof(depositInput); // Convert string to double

            if (depositAmount < 0)
            {
                printf("Deposit cannot be negative.\n");
                break;
            }

            if (confirmOperation() == 0)
            {
                break;
            }

            balance += depositAmount;

            // Go to the beginning of the current line
            fseek(file, pos, SEEK_SET);

            fprintf(file, LINE_FORMAT, accountNum, name, surname, address, pesel,
                    balance, loanBalance, loanInterestRate);
            printf("Deposit successful.\n");
            break;
        }
        pos = ftell(file);
    }

    if (!found)
    {
        printf("Account number %d not found.\n", accountNumber);
    }

    fclose(file);
}

void withdrawMoney()
{
    char accountInput[50];
    int accountNumber;
    char withdrawInput[50];
    double withdrawAmount;
    int found = 0;

    printf("Enter account number: ");
    fgets(accountInput, sizeof(accountInput), stdin);
    accountInput[strcspn(accountInput, "\n")] = 0;

    if (!isAllDigits(accountInput))
    {
        printf("Invalid account number input. Please enter a valid number.\n");
        return;
    }
    accountNumber = atoi(accountInput);

    FILE *file = fopen(FILENAME, "r+");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    char line[256];
    long pos = 0;
    while (fgets(line, sizeof(line), file))
    {
        int accountNum;
        char name[16], surname[18], address[21], pesel[12];
        double balance, loanBalance, loanInterestRate;

        sscanf(line, " %d | %15[^|] | %17[^|] | %20[^|] | %11[^|] | %lf | %lf | %lf",
               &accountNum, name, surname, address, pesel, &balance, &loanBalance, &loanInterestRate);

        if (accountNum == accountNumber)
        {
            found = 1;
            printf("Enter withdrawal amount: ");
            fgets(withdrawInput, sizeof(withdrawInput), stdin);
            withdrawInput[strcspn(withdrawInput, "\n")] = 0;

            int dotCount = 0;
            for (int i = 0; withdrawInput[i] != '\0'; i++)
            {
                if (!isdigit(withdrawInput[i]))
                {
                    if (withdrawInput[i] == '.' && dotCount == 0)
                    {
                        dotCount = 1;
                    }
                    else
                    {
                        printf("Invalid withdrawal input. Please enter a valid number.\n");
                        return;
                    }
                }
            }

            withdrawAmount = atof(withdrawInput);

            if (withdrawAmount < 0)
            {
                printf("Withdrawal amount cannot be negative.\n");
                break;
            }

            if (withdrawAmount > balance)
            {
                printf("Insufficient funds for withdrawal.\n");
                break;
            }

            if (confirmOperation() == 0)
            {
                break;
            }

            balance -= withdrawAmount;

            fseek(file, pos, SEEK_SET);

            fprintf(file, LINE_FORMAT, accountNum, name, surname, address, pesel,
                    balance, loanBalance, loanInterestRate);
            printf("Withdrawal successful.\n");
            break;
        }
        pos = ftell(file);
    }

    if (!found)
    {
        printf("Account number %d not found.\n", accountNumber);
    }

    fclose(file);
}

void transferMoney()
{
    char sourceAccountInput[50], destAccountInput[50];
    int sourceAccountNumber, destAccountNumber;
    char transferInput[50];
    double transferAmount;
    int sourceFound = 0, destFound = 0;

    printf("Enter source account number: ");
    fgets(sourceAccountInput, sizeof(sourceAccountInput), stdin);
    sourceAccountInput[strcspn(sourceAccountInput, "\n")] = 0;

    if (!isAllDigits(sourceAccountInput))
    {
        printf("Invalid account number input. Please enter a valid number.\n");
        return;
    }
    sourceAccountNumber = atoi(sourceAccountInput);

    printf("Enter destination account number: ");
    fgets(destAccountInput, sizeof(destAccountInput), stdin);
    destAccountInput[strcspn(destAccountInput, "\n")] = 0;

    if (!isAllDigits(destAccountInput))
    {
        printf("Invalid account number input. Please enter a valid number.\n");
        return;
    }
    destAccountNumber = atoi(destAccountInput);

    if (sourceAccountNumber == destAccountNumber)
    {
        printf("Source and destination account numbers must be different.\n");
        return;
    }

    FILE *file = fopen(FILENAME, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    char line[256];
    int accountNum;
    char name[16], surname[18], address[21], pesel[13];
    double balance, loanBalance, loanInterestRate;

    int accountNumbers[100];
    char names[100][16];
    char surnames[100][18];
    char addresses[100][21];
    char pesels[100][12];
    double balances[100];
    double loanBalances[100];
    double loanInterestRates[100];

    int accountCount = 0;

    // Read all accounts into memory
    while (fgets(line, sizeof(line), file))
    {
        if (sscanf(line, " %d | %15[^|] | %17[^|] | %20[^|] | %11[^|] | %lf | %lf | %lf",
                   &accountNum, name, surname, address, pesel, &balance, &loanBalance, &loanInterestRate) == 8)
        {
            accountNumbers[accountCount] = accountNum;
            strcpy(names[accountCount], name);
            strcpy(surnames[accountCount], surname);
            strcpy(addresses[accountCount], address);
            strcpy(pesels[accountCount], pesel);
            balances[accountCount] = balance;
            loanBalances[accountCount] = loanBalance;
            loanInterestRates[accountCount] = loanInterestRate;

            if (accountNum == sourceAccountNumber)
            {
                sourceFound = 1;
            }
            if (accountNum == destAccountNumber)
            {
                destFound = 1;
            }

            accountCount++;
        }
    }
    fclose(file);

    if (!sourceFound)
    {
        printf("Source account number %d not found.\n", sourceAccountNumber);
        return;
    }

    if (!destFound)
    {
        printf("Destination account number %d not found.\n", destAccountNumber);
        return;
    }

    printf("Enter transfer amount: ");
    fgets(transferInput, sizeof(transferInput), stdin);
    transferInput[strcspn(transferInput, "\n")] = 0;

    int dotCount = 0;
    for (int i = 0; transferInput[i] != '\0'; i++)
    {
        if (!isdigit(transferInput[i]))
        {
            if (transferInput[i] == '.' && !dotCount)
            {
                dotCount = 1;
            }
            else
            {
                printf("Invalid transfer input. Please enter a valid number.\n");
                return;
            }
        }
    }

    transferAmount = atof(transferInput);

    if (transferAmount < 0)
    {
        printf("Transfer amount cannot be negative.\n");
        return;
    }

    if (confirmOperation() == 0)
    {
        return;
    }

    int sourceIndex = -1, destIndex = -1;

    for (int i = 0; i < accountCount; i++)
    {
        if (accountNumbers[i] == sourceAccountNumber)
        {
            sourceIndex = i;
        }
        if (accountNumbers[i] == destAccountNumber)
        {
            destIndex = i;
        }
    }

    if (transferAmount > balances[sourceIndex])
    {
        printf("Insufficient funds to transfer %.2f. Available balance is %.2f.\n", transferAmount, balances[sourceIndex]);
        return;
    }

    // Update balances
    balances[sourceIndex] -= transferAmount;
    balances[destIndex] += transferAmount;

    // Write all accounts back to the file
    file = fopen(FILENAME, "w");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    fprintf(file, HEADER);
    fprintf(file, SEPARATOR);
    for (int i = 0; i < accountCount; i++)
    {
        fprintf(file, LINE_FORMAT, accountNumbers[i], names[i], surnames[i],
                addresses[i], pesels[i], balances[i], loanBalances[i], loanInterestRates[i]);
    }
    fclose(file);

    printf("Transfer successful.\n");
}

void takeLoan()
{
    char accountInput[50];
    int accountNumber;
    char loanInput[50];
    double loanAmount;
    int found = 0;

    printf("Enter account number: ");
    fgets(accountInput, sizeof(accountInput), stdin);
    accountInput[strcspn(accountInput, "\n")] = 0;

    if (!isAllDigits(accountInput))
    {
        printf("Invalid account number input. Please enter a valid number.\n");
        return;
    }
    accountNumber = atoi(accountInput);

    FILE *file = fopen(FILENAME, "r+");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    char line[256];
    long pos = 0;
    while (fgets(line, sizeof(line), file))
    {
        int accountNum;
        char name[16], surname[18], address[21], pesel[12];
        double balance, loanBalance, loanInterestRate;

        sscanf(line, " %d | %15[^|] | %17[^|] | %20[^|] | %11[^|] | %lf | %lf | %lf",
               &accountNum, name, surname, address, pesel, &balance, &loanBalance, &loanInterestRate);

        if (accountNum == accountNumber)
        {
            found = 1;
            printf("Enter loan amount: ");
            fgets(loanInput, sizeof(loanInput), stdin);
            loanInput[strcspn(loanInput, "\n")] = 0;

            int dotCount = 0;
            for (int i = 0; loanInput[i] != '\0'; i++)
            {
                if (!isdigit(loanInput[i]))
                {
                    if (loanInput[i] == '.' && !dotCount)
                    {
                        dotCount = 1;
                    }
                    else
                    {
                        printf("Invalid loan input. Please enter a valid number.\n");
                        return;
                    }
                }
            }

            loanAmount = atof(loanInput);

            if (loanAmount < 0)
            {
                printf("Loan amount cannot be negative.\n");
                break;
            }

            if (confirmOperation() == 0)
            {
                break;
            }

            balance += loanAmount;
            loanBalance += loanAmount * (1 + INTEREST_RATE); // Apply interest

            // Go to the beginning of the current line
            fseek(file, pos, SEEK_SET);

            fprintf(file, LINE_FORMAT, accountNum, name, surname, address, pesel,
                    balance, loanBalance, loanInterestRate);
            printf("Loan taken successfully. Interest rate: %.2f%%.\n", INTEREST_RATE * 100);
            break;
        }

        // Save the current position
        pos = ftell(file);
    }

    if (!found)
    {
        printf("Account number %d not found.\n", accountNumber);
    }

    fclose(file);
}

void payDebt()
{
    char accountInput[50];
    int accountNumber;
    int found = 0;

    printf("Enter account number: ");
    fgets(accountInput, sizeof(accountInput), stdin);
    accountInput[strcspn(accountInput, "\n")] = 0;

    if (!isAllDigits(accountInput))
    {
        printf("Invalid account number input. Please enter a valid number.\n");
        return;
    }
    accountNumber = atoi(accountInput);

    FILE *file = fopen(FILENAME, "r+");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    char line[256];
    long pos = 0;
    while (fgets(line, sizeof(line), file))
    {
        int accountNum;
        char name[16], surname[18], address[21], pesel[12];
        double balance, loanBalance, loanInterestRate;

        sscanf(line, " %d | %15[^|] | %17[^|] | %20[^|] | %11[^|] | %lf | %lf | %lf",
               &accountNum, name, surname, address, pesel, &balance, &loanBalance, &loanInterestRate);

        if (accountNum == accountNumber)
        {
            found = 1;

            if (loanBalance == 0)
            {
                printf("No loan to pay.\n");
                break;
            }

            if (balance >= loanBalance)
            {
                balance -= loanBalance;
                loanBalance = 0;

                if (confirmOperation() == 0)
                {
                    break;
                }

                fseek(file, pos, SEEK_SET);

                fprintf(file, LINE_FORMAT, accountNum, name, surname, address, pesel,
                        balance, loanBalance, loanInterestRate);
                printf("Debt paid off successfully.\n");
            }
            else
            {
                printf("Not enough balance to pay off the debt. Account balance: %.2f, required: %.2f.\n", balance, loanBalance);
            }
            break;
        }

        pos = ftell(file);
    }

    if (!found)
    {
        printf("Account number %d not found.\n", accountNumber);
    }

    fclose(file);
}
