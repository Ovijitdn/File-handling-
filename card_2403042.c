#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define CARD_NUMBER_LENGTH 20
#define MOBILE_NUMBER_LENGTH 11
#define MAX_FAILED_ATTEMPTS 3
typedef struct
{
    char cardNumber042[CARD_NUMBER_LENGTH + 1];
    int minutes042;
    int price042;
    int isUsed042;
} Card;
typedef struct
{
    char cardNumber042[CARD_NUMBER_LENGTH + 1];
    char mobileNumber042[MOBILE_NUMBER_LENGTH + 1];
    int minutes042;
    char date042[11];
    char time042[9];
} Transaction;
typedef struct
{
    char mobileNumber042[MOBILE_NUMBER_LENGTH + 1];
    int failedAttempts042;
    int isLocked042;
} User;
const char *CARDS_FILE042 = "cards.dat";
const char *TRANSACTIONS_FILE042 = "transactions.dat";
const char *USERS_FILE042 = "users.dat";
void generateRandomCardNumber(char *cardNumber042);
void getCurrentDateTime(char *dateStr042, char *timeStr042);
int getPrice(int minutes042);
int isValidMobileNumber(const char *mobile042);
void clearInputBuffer();
void adminMenu();
void createNewCards();
void deleteCard();
void unlockUser();
void viewHistory();
void viewStatistics();
void searchByMobile();
void userMenu();

int main()
{
    srand(time(NULL));
    char username042[50];
    char password042[50];
    int choice042 = 0;
    while (1)
    {
        printf("\n=============================\n");
        printf("  Mobile Recharge System\n");
        printf("  Created By Ovijit  \n");
        printf("=============================\n");
        printf("1. Admin Login\n");
        printf("2. User Menu\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice042) != 1)
        {
            printf("Invalid input. Please enter a number.\n");
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();
        switch (choice042)
        {
        case 1:
            printf("Enter admin username: ");
            scanf("%49s", username042);
            printf("Enter admin password: ");
            scanf("%49s", password042);
            clearInputBuffer();
            if (strcmp(username042, "Ovijit") == 0 && strcmp(password042, "admin123") == 0)
            {
                printf("Admin login successful!\n");
                adminMenu();
            }
            else
            {
                printf("Invalid credentials.\n");
            }
            break;
        case 2:
            userMenu();
            break;
        case 3:
            printf("Exiting program.\n");
            exit(0);
        default:
            printf("Invalid choice. Please try again.\n");
        }
    }
    return 0;
}
// --- ADMIN MENU ---
void adminMenu()
{
    int option042 = 0;
    do
    {
        printf("\n--- The Admin Menu ---\n");
        printf("1. New Card\n");
        printf("2. Delete Card\n");
        printf("3. Unlock User\n");
        printf("4. History\n");
        printf("5. Statistics\n");
        printf("6. Search by Mobile\n");
        printf("7. Exit to Main Menu\n");
        printf("Enter your option: ");
        if (scanf("%d", &option042) != 1)
        {
            printf("Invalid input. Please enter a number.\n");
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();
        switch (option042)
        {
        case 1:
            createNewCards();
            break;
        case 2:
            deleteCard();
            break;
        case 3:
            unlockUser();
            break;
        case 4:
            viewHistory();
            break;
        case 5:
            viewStatistics();
            break;
        case 6:
            searchByMobile();
            break;
        case 7:
            printf("Returning to main menu...\n");
            break;
        default:
            printf("Invalid option. Please try again.\n");
        }
    } while (option042 != 7);
}
void createNewCards()
{
    int numCards042, minutes042;
    printf("How many cards and minute: ");
    if (scanf("%d %d", &numCards042, &minutes042) != 2)
    {
        printf("Invalid input format.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();
    if (minutes042 != 40 && minutes042 != 60 && minutes042 != 100)
    {
        printf("Invalid minute value. Please choose 40, 60, or 100.\n");
        return;
    }
    FILE *fp042 = fopen(CARDS_FILE042, "ab");
    if (fp042 == NULL)
    {
        printf("Error opening cards file!\n");
        return;
    }
    for (int i = 0; i < numCards042; i++)
    {
        Card newCard042;
        generateRandomCardNumber(newCard042.cardNumber042);
        newCard042.minutes042 = minutes042;
        newCard042.price042 = getPrice(minutes042);
        newCard042.isUsed042 = 0;
        fwrite(&newCard042, sizeof(Card), 1, fp042);
    }
    fclose(fp042);
    printf("%d Cards Creation Successful\n", numCards042);
}
void deleteCard()
{
    char cardToDelete042[CARD_NUMBER_LENGTH + 2];
    printf("Enter Card Number: ");
    scanf("%21s", cardToDelete042);
    clearInputBuffer();
    if (strlen(cardToDelete042) != CARD_NUMBER_LENGTH)
    {
        printf("Invalid card number length. Must be %d digits.\n", CARD_NUMBER_LENGTH);
        return;
    }
    FILE *fp042 = fopen(CARDS_FILE042, "rb");
    if (fp042 == NULL)
    {
        printf("No cards exist to delete.\n");
        return;
    }
    FILE *tempFp042 = fopen("temp.dat", "wb");
    if (tempFp042 == NULL)
    {
        printf("Error creating temporary file.\n");
        fclose(fp042);
        return;
    }
    Card tempCard042;
    int found042 = 0;
    while (fread(&tempCard042, sizeof(Card), 1, fp042) == 1)
    {
        if (strcmp(tempCard042.cardNumber042, cardToDelete042) == 0)
        {
            found042 = 1;
        }
        else
        {
            fwrite(&tempCard042, sizeof(Card), 1, tempFp042);
        }
    }
    fclose(fp042);
    fclose(tempFp042);
    remove(CARDS_FILE042);
    rename("temp.dat", CARDS_FILE042);
    if (found042)
        printf("Card Deletion Successful\n");
    else
        printf("Card not found.\n");
}
void unlockUser()
{
    char mobileToUnlock042[MOBILE_NUMBER_LENGTH + 2];
    printf("Enter Mobile Number: ");
    scanf("%12s", mobileToUnlock042);
    clearInputBuffer();
    if (!isValidMobileNumber(mobileToUnlock042))
    {
        printf("Invalid mobile number format.\n");
        return;
    }
    FILE *fp042 = fopen(USERS_FILE042, "r+b");
    if (fp042 == NULL)
    {
        printf("User data file not found. No users to unlock.\n");
        return;
    }
    User tempUser042;
    int found042 = 0;
    while (fread(&tempUser042, sizeof(User), 1, fp042) == 1)
    {
        if (strcmp(tempUser042.mobileNumber042, mobileToUnlock042) == 0)
        {
            found042 = 1;
            tempUser042.isLocked042 = 0;
            tempUser042.failedAttempts042 = 0;
            fseek(fp042, -sizeof(User), SEEK_CUR);
            fwrite(&tempUser042, sizeof(User), 1, fp042);
            break;
        }
    }
    fclose(fp042);
    if (found042)
        printf("User Unlock Successful\n");
    else
        printf("Mobile number not found.\n");
}
void viewHistory()
{
    FILE *fp042 = fopen(TRANSACTIONS_FILE042, "rb");
    if (fp042 == NULL)
    {
        printf("No transaction history found.\n");
        return;
    }
    printf("\n%-25s %-12s %-10s %-8s %-15s\n", "Card No", "Date", "Time", "Minute", "Mobile No");
    printf("----------------------------------------------------------------------------\n");
    Transaction trans042;
    while (fread(&trans042, sizeof(Transaction), 1, fp042) == 1)
    {
        char formattedCard042[30];
        snprintf(formattedCard042, sizeof(formattedCard042), "%.4s %.4s %.4s %.4s %.4s",
                 trans042.cardNumber042, trans042.cardNumber042 + 4, trans042.cardNumber042 + 8,
                 trans042.cardNumber042 + 12, trans042.cardNumber042 + 16);
        printf("%-25s %-12s %-10s %-8d %-15s\n", formattedCard042, trans042.date042, trans042.time042, trans042.minutes042, trans042.mobileNumber042);
    }
    fclose(fp042);
}
void viewStatistics()
{
    int sold40042 = 0, sold60042 = 0, sold100042 = 0;
    int remain40042 = 0, remain60042 = 0, remain100042 = 0;
    FILE *fp042 = fopen(CARDS_FILE042, "rb");
    if (fp042 == NULL)
    {
        printf("Card data file not found. Create new cards first.\n");
        return;
    }
    Card tempCard042;
    while (fread(&tempCard042, sizeof(Card), 1, fp042) == 1)
    {
        if (tempCard042.isUsed042)
        {
            if (tempCard042.minutes042 == 40)
                sold40042++;
            else if (tempCard042.minutes042 == 60)
                sold60042++;
            else if (tempCard042.minutes042 == 100)
                sold100042++;
        }
        else
        {
            if (tempCard042.minutes042 == 40)
                remain40042++;
            else if (tempCard042.minutes042 == 60)
                remain60042++;
            else if (tempCard042.minutes042 == 100)
                remain100042++;
        }
    }
    fclose(fp042);
    printf("\n--- Statistics ---\n");
    printf("Total Cards Sold:   40 min: %-5d 60 min: %-5d 100 min: %-5d\n", sold40042, sold60042, sold100042);
    printf("Remaining Cards:    40 min: %-5d 60 min: %-5d 100 min: %-5d\n", remain40042, remain60042, remain100042);
    printf("Total Revenue (Tk): 40 min: %-5ld 60 min: %-5ld 100 min: %-5ld\n",
           (long)sold40042 * getPrice(40),
           (long)sold60042 * getPrice(60),
           (long)sold100042 * getPrice(100));
}
void searchByMobile()
{
    char mobileToSearch042[MOBILE_NUMBER_LENGTH + 2];
    printf("Enter Mobile Number: ");
    scanf("%12s", mobileToSearch042);
    clearInputBuffer();
    if (!isValidMobileNumber(mobileToSearch042))
    {
        printf("Invalid mobile number format.\n");
        return;
    }
    FILE *fp042 = fopen(TRANSACTIONS_FILE042, "rb");
    if (fp042 == NULL)
    {
        printf("No transaction history found.\n");
        return;
    }
    printf("\n--- Search Results for %s ---\n", mobileToSearch042);
    printf("%-25s %-12s %-10s %-8s\n", "Card No", "Date", "Time", "Minute");
    printf("---------------------------------------------------------------------\n");
    Transaction trans042;
    int count042 = 0;
    while (fread(&trans042, sizeof(Transaction), 1, fp042) == 1)
    {
        if (strcmp(trans042.mobileNumber042, mobileToSearch042) == 0)
        {
            char formattedCard042[30];
            snprintf(formattedCard042, sizeof(formattedCard042), "%.4s **** **** **** %.4s",
                     trans042.cardNumber042, trans042.cardNumber042 + 16);
            printf("%-25s %-12s %-10s %-8d\n", formattedCard042, trans042.date042, trans042.time042, trans042.minutes042);
            count042++;
        }
    }
    fclose(fp042);
    printf("\nTotal Transactions: %d\n", count042);
}
// --- USER MENU ---
void userMenu()
{
    char mobileNumber042[MOBILE_NUMBER_LENGTH + 2];
    char cardNumber042[CARD_NUMBER_LENGTH + 2];
    int packChoice042;
    printf("Enter Mobile Number: ");
    scanf("%12s", mobileNumber042);
    clearInputBuffer();
    if (!isValidMobileNumber(mobileNumber042))
    {
        printf("Invalid mobile number format. Must be 11 digits starting with 01.\n");
        return;
    }
    FILE *userFp042 = fopen(USERS_FILE042, "r+b");
    if (userFp042 == NULL)
    {
        userFp042 = fopen(USERS_FILE042, "wb+");
        if (userFp042 == NULL)
        {
            printf("Error managing user data.\n");
            return;
        }
    }
    User currentUser042;
    int userFound042 = 0;
    long userPosition042 = -1;

    rewind(userFp042);
    while (fread(&currentUser042, sizeof(User), 1, userFp042) == 1)
    {
        if (strcmp(currentUser042.mobileNumber042, mobileNumber042) == 0)
        {
            userFound042 = 1;
            userPosition042 = ftell(userFp042) - sizeof(User);
            break;
        }
    }
    if (!userFound042)
    {
        strcpy(currentUser042.mobileNumber042, mobileNumber042);
        currentUser042.failedAttempts042 = 0;
        currentUser042.isLocked042 = 0;
    }
    if (currentUser042.isLocked042)
    {
        printf("This number is locked due to multiple failed attempts. Please contact admin.\n");
        fclose(userFp042);
        return;
    }
    printf("\nChoose your option:\n");
    printf("1. Auto-reload with 40 min\n");
    printf("2. Auto-reload with 60 min\n");
    printf("3. Auto-reload with 100 min\n");
    printf("4. Enter card number manually\n");
    printf("Enter choice: ");
    scanf("%d", &packChoice042);
    clearInputBuffer();
    int requiredMinutes042 = 0;
    if (packChoice042 == 1)
        requiredMinutes042 = 40;
    else if (packChoice042 == 2)
        requiredMinutes042 = 60;
    else if (packChoice042 == 3)
        requiredMinutes042 = 100;
    FILE *cardFp042 = fopen(CARDS_FILE042, "r+b");
    if (cardFp042 == NULL)
    {
        printf("System error: Card database not found.\n");
        fclose(userFp042);
        return;
    }
    Card tempCard042;
    long cardPosition042 = -1;
    int cardFound042 = 0;
    if (requiredMinutes042 > 0)
    {
        rewind(cardFp042);
        while (fread(&tempCard042, sizeof(Card), 1, cardFp042) == 1)
        {
            if (tempCard042.minutes042 == requiredMinutes042 && !tempCard042.isUsed042)
            {
                cardPosition042 = ftell(cardFp042) - sizeof(Card);
                cardFound042 = 1;
                break;
            }
        }
        if (!cardFound042)
        {
            printf("No available %d-minute cards. Please try another option.\n", requiredMinutes042);
            fclose(cardFp042);
            fclose(userFp042);
            return;
        }
    }
    else
    {
        printf("Enter Card Number: ");
        scanf("%21s", cardNumber042);
        clearInputBuffer();
        if (strlen(cardNumber042) != CARD_NUMBER_LENGTH)
        {
            printf("Invalid card number. Must be %d digits.\n", CARD_NUMBER_LENGTH);
            currentUser042.failedAttempts042++;
        }
        else
        {
            rewind(cardFp042);
            while (fread(&tempCard042, sizeof(Card), 1, cardFp042) == 1)
            {
                if (strcmp(tempCard042.cardNumber042, cardNumber042) == 0 && !tempCard042.isUsed042)
                {
                    cardPosition042 = ftell(cardFp042) - sizeof(Card);
                    cardFound042 = 1;
                    break;
                }
            }
            if (!cardFound042)
            {
                printf("Card not found or already used.\n");
                currentUser042.failedAttempts042++;
            }
        }
        if (currentUser042.failedAttempts042 >= MAX_FAILED_ATTEMPTS)
        {
            currentUser042.isLocked042 = 1;
            printf("Too many failed attempts! Your number has been locked. Contact Admin.\n");
        }
        fseek(userFp042, userPosition042, SEEK_SET);
        fwrite(&currentUser042, sizeof(User), 1, userFp042);
        if (!cardFound042 || currentUser042.isLocked042)
        {
            fclose(cardFp042);
            fclose(userFp042);
            return;
        }
    }
    if (cardFound042)
    {
        fseek(cardFp042, cardPosition042, SEEK_SET);
        fread(&tempCard042, sizeof(Card), 1, cardFp042);
        tempCard042.isUsed042 = 1;
        fseek(cardFp042, cardPosition042, SEEK_SET);
        fwrite(&tempCard042, sizeof(Card), 1, cardFp042);
        char date042[11], time042[9];
        getCurrentDateTime(date042, time042);
        Transaction newTransaction042;
        strcpy(newTransaction042.cardNumber042, tempCard042.cardNumber042);
        strcpy(newTransaction042.mobileNumber042, currentUser042.mobileNumber042);
        newTransaction042.minutes042 = tempCard042.minutes042;
        strcpy(newTransaction042.date042, date042);
        strcpy(newTransaction042.time042, time042);
        FILE *transFp042 = fopen(TRANSACTIONS_FILE042, "ab");
        if (transFp042 == NULL)
        {
            printf("Error opening transactions file.\n");
            fclose(cardFp042);
            fclose(userFp042);
            return;
        }
        fwrite(&newTransaction042, sizeof(Transaction), 1, transFp042);
        fclose(transFp042);
        printf("Recharge successful! Card Number: %s\n", newTransaction042.cardNumber042);
    }
    currentUser042.failedAttempts042 = 0;
    fseek(userFp042, userPosition042, SEEK_SET);
    fwrite(&currentUser042, sizeof(User), 1, userFp042);
    fclose(cardFp042);
    fclose(userFp042);
    printf("Thank you for using our service!\n");
}
void generateRandomCardNumber(char *cardNumber042)
{
    for (int i = 0; i < CARD_NUMBER_LENGTH; i++)
    {
        cardNumber042[i] = '0' + rand() % 10;
    }
    cardNumber042[CARD_NUMBER_LENGTH] = '\0';
}
void getCurrentDateTime(char *dateStr042, char *timeStr042)
{
    time_t now = time(NULL);
    struct tm *local = localtime(&now);
    strftime(dateStr042, 11, "%d/%m/%Y", local);
    strftime(timeStr042, 9, "%H:%M:%S", local);
}
int getPrice(int minutes042)
{
    switch (minutes042)
    {
    case 40:
        return 100;
    case 60:
        return 150;
    case 100:
        return 200;
    default:
        return -1;
    }
}
int isValidMobileNumber(const char *mobile042)
{
    if (strlen(mobile042) != MOBILE_NUMBER_LENGTH)
        return 0;
    if (mobile042[0] != '0' || mobile042[1] != '1')
        return 0;
    for (int i = 2; i < MOBILE_NUMBER_LENGTH; i++)
    {
        if (mobile042[i] < '0' || mobile042[i] > '9')
            return 0;
    }
    return 1;
}
void clearInputBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
