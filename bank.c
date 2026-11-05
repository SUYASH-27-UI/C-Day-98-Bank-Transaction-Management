#include <stdio.h>
#include <string.h>

struct Account {
    int accountNumber;
    char name[50];
    float balance;
};

void addAccount() {
    struct Account a;

    printf("\nEnter Account Number: ");
    scanf("%d", &a.accountNumber);

    printf("Enter Account Holder Name: ");
    scanf(" %49[^\n]", a.name);

    printf("Enter Initial Balance: ");
    scanf("%f", &a.balance);

    if (a.balance < 0) {
        printf("Invalid initial balance!\n");
        return;
    }

    FILE *fp = fopen("accounts.txt", "a");

    if (fp == NULL) {
        printf("File error!\n");
        return;
    }

    fprintf(fp, "%d|%s|%.2f\n",
            a.accountNumber, a.name, a.balance);

    fclose(fp);

    printf("Account created successfully!\n");
}

void displayAccounts() {
    struct Account a;
    char line[200];

    FILE *fp = fopen("accounts.txt", "r");

    if (fp == NULL) {
        printf("No account records found.\n");
        return;
    }

    printf("\n===== ACCOUNT DETAILS =====\n");

    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%d|%49[^|]|%f",
                   &a.accountNumber,
                   a.name,
                   &a.balance) == 3) {

            printf("\nAccount Number: %d\n", a.accountNumber);
            printf("Name: %s\n", a.name);
            printf("Balance: Rs. %.2f\n", a.balance);
        }
    }

    fclose(fp);
}

void transaction(int type) {
    struct Account a;
    char line[200];
    int number, found = 0;
    float amount;

    FILE *fp = fopen("accounts.txt", "r");

    if (fp == NULL) {
        printf("Account file not found!\n");
        return;
    }

    FILE *temp = fopen("temp.txt", "w");

    if (temp == NULL) {
        fclose(fp);
        printf("File error!\n");
        return;
    }

    printf("Enter Account Number: ");
    scanf("%d", &number);

    printf("Enter Amount: ");
    scanf("%f", &amount);

    if (amount <= 0) {
        printf("Invalid amount!\n");
        fclose(fp);
        fclose(temp);
        remove("temp.txt");
        return;
    }

    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%d|%49[^|]|%f",
                   &a.accountNumber,
                   a.name,
                   &a.balance) == 3) {

            if (a.accountNumber == number) {
                found = 1;

                if (type == 1) {
                    a.balance += amount;
                    printf("Deposit successful!\n");
                }
                else if (a.balance >= amount) {
                    a.balance -= amount;
                    printf("Withdrawal successful!\n");
                }
                else {
                    printf("Insufficient balance!\n");
                }
            }

            fprintf(temp, "%d|%s|%.2f\n",
                    a.accountNumber, a.name, a.balance);
        }
    }

    fclose(fp);
    fclose(temp);

    if (found) {
        remove("accounts.txt");
        rename("temp.txt", "accounts.txt");
    }
    else {
        remove("temp.txt");
        printf("Account not found!\n");
    }
}

void checkBalance() {
    struct Account a;
    char line[200];
    int number, found = 0;

    printf("Enter Account Number: ");
    scanf("%d", &number);

    FILE *fp = fopen("accounts.txt", "r");

    if (fp == NULL) {
        printf("No account records found.\n");
        return;
    }

    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%d|%49[^|]|%f",
                   &a.accountNumber,
                   a.name,
                   &a.balance) == 3) {

            if (a.accountNumber == number) {
                printf("Account Holder: %s\n", a.name);
                printf("Current Balance: Rs. %.2f\n", a.balance);
                found = 1;
                break;
            }
        }
    }

    fclose(fp);

    if (!found) {
        printf("Account not found!\n");
    }
}

int main() {
    int choice;

    do {
        printf("\n===== BANK MANAGEMENT SYSTEM =====\n");
        printf("1. Create Account\n");
        printf("2. Display Accounts\n");
        printf("3. Deposit Money\n");
        printf("4. Withdraw Money\n");
        printf("5. Check Balance\n");
        printf("6. Exit\n");

        printf("Enter Choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addAccount();
                break;

            case 2:
                displayAccounts();
                break;

            case 3:
                transaction(1);
                break;

            case 4:
                transaction(2);
                break;

            case 5:
                checkBalance();
                break;

            case 6:
                printf("Thank you for using the system!\n");
                break;

            default:
                printf("Invalid choice!\n");
        }

    } while (choice != 6);

    return 0;
}
