#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to find the account details in the file
int findAccount(char* filename, int accNo, char* name, float* balance, int* pin) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        return 0; // File not found
    }

    int fileAccNo, filePin;
    char key[20], value[100];

    while (fscanf(file, "%s %s", key, value) != EOF) {
        if (strcmp(key, "accno") == 0) {
            fileAccNo = atoi(value);
        } else if (strcmp(key, "name") == 0) {
            strcpy(name, value);
        } else if (strcmp(key, "bank_balance") == 0) {
            *balance = atof(value);
        } else if (strcmp(key, "pin") == 0) {
            filePin = atoi(value);
        }
    }

    fclose(file);

    if (fileAccNo == accNo) {
        *pin = filePin;
        return 1; // Account found
    }

    return 0; // Account not found
}

// Function to update account details in the file
void updateAccount(char* filename, int accNo, const char* name, float balance, int pin) {
    FILE* file = fopen(filename, "a");
    fprintf(file, "accno=%d\nname=%s\nbank_balance=%.2f\npin=%d\n", accNo, name, balance, pin);
    fclose(file);
}

// Function to withdraw money from the account
void withdrawMoney(char* filename, int accNo, float amount) {
    char name[100];
    float balance;
    int pin;

    if (findAccount(filename, accNo, name, &balance, &pin)) {
        if (amount <= balance) {
            balance -= amount;
            printf("Withdrawal successful. Remaining balance: %.2f\n", balance);
            updateAccount(filename, accNo, name, balance, pin);
        } else {
            printf("Insufficient funds. Withdrawal failed.\n");
        }
    } else {
        printf("Account not found. Withdrawal failed.\n");
    }
}

// Function to display account balance
void balanceEnquiry(char* filename, int accNo) {
    char name[100];
    float balance;
    int pin;

    if (findAccount(filename, accNo, name, &balance, &pin)) {
        printf("Account balance: %.2f\n", balance);
    } else {
        printf("Account not found. Balance enquiry failed.\n");
    }
}

int main() {
    char filename[] = "account_details.txt";

    int accNo, pin, option;
    float amount;

    printf("Enter your account number: ");
    scanf("%d", &accNo);

    if (!findAccount(filename, accNo, NULL, NULL, &pin)) {
        printf("Account not found. Exiting...\n");
        return 1;
    }

    printf("Enter your PIN: ");
    scanf("%d", &pin);

    if (!findAccount(filename, accNo, NULL, NULL, &pin)) {
        printf("Invalid account number or PIN. Exiting...\n");
        return 1;
    }

    do {
        printf("\n1. Withdraw Money\n2. Balance Enquiry\n0. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &option);

        switch (option) {
            case 1:
                printf("Enter the amount to withdraw: ");
                scanf("%f", &amount);
                withdrawMoney(filename, accNo, amount);
                break;

            case 2:
                balanceEnquiry(filename, accNo);
                break;

            case 0:
                printf("Exiting...\n");
                break;

            default:
                printf("Invalid option. Try again.\n");
        }
    } while (option != 0);

    return 0;
}
