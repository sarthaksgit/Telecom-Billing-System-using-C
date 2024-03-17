#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "telecom_records.txt"
#define MAX_NAME_LENGTH 50
#define MAX_PHONE_LENGTH 15

// Structure to represent a telecom record
struct Record {
    char name[MAX_NAME_LENGTH];
    char phone[MAX_PHONE_LENGTH];
    float amount;
};

// Function prototypes
void addRecord();
void listRecords();
void editRecord();
void searchRecord();
void deleteRecord();

int main() {
    int choice;
    printf("\n**************************************************************\n");
    printf("****          Welcome to Telecom Billing System          ****\n");
    printf("**************************************************************\n");

    do {
        printf("\nMain Menu:\n");
        printf("1. Add Record\n");
        printf("2. List Records\n");
        printf("3. Edit Record\n");
        printf("4. Search Record\n");
        printf("5. Delete Record\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1:
                addRecord();
                break;
            case 2:
                listRecords();
                break;
            case 3:
                editRecord();
                break;
            case 4:
                searchRecord();
                break;
            case 5:
                deleteRecord();
                break;
            case 6:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    } while(choice != 6);

    return 0;
}

// Function to add a record
void addRecord() {
    struct Record record;
    FILE *file = fopen(FILENAME, "a");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    printf("\nAdding a New Record\n");

    printf("Enter name: ");
    getchar(); // Clear input buffer
    fgets(record.name, MAX_NAME_LENGTH, stdin);
    record.name[strcspn(record.name, "\n")] = '\0'; // Remove newline character

    printf("Enter phone number: ");
    scanf("%s", record.phone);
    getchar(); // Clear input buffer

    printf("Enter amount: ");
    scanf("%f", &record.amount);
    getchar(); // Clear input buffer

    fprintf(file, "%s %s %.2f\n", record.name, record.phone, record.amount);
    fclose(file);
    printf("Record added successfully.\n");
}

// Function to list all records
void listRecords() {
    struct Record record;
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) {
        printf("No records found.\n");
        return;
    }

    printf("\nListing All Records\n");
    printf("Name\tPhone\tAmount\n");
    while (fscanf(file, "%s %s %f", record.name, record.phone, &record.amount) == 3) {
        printf("%s\t%s\t%.2f\n", record.name, record.phone, record.amount);
    }
    fclose(file);
}

// Function to edit a record
void editRecord() {
    char old_phone[MAX_PHONE_LENGTH];
    char new_phone[MAX_PHONE_LENGTH];
    char new_name[MAX_NAME_LENGTH];

    printf("\nEditing a Record\n");
    printf("Enter old phone number to edit: ");
    scanf("%s", old_phone);
    getchar(); // Clear input buffer

    printf("Enter new phone number: ");
    scanf("%s", new_phone);
    getchar(); // Clear input buffer

    printf("Enter new name: ");
    fgets(new_name, MAX_NAME_LENGTH, stdin);
    new_name[strcspn(new_name, "\n")] = '\0'; // Remove newline character

    struct Record record;
    FILE *file = fopen(FILENAME, "r+");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    int found = 0;
    while (fscanf(file, "%s %s %f", record.name, record.phone, &record.amount) != EOF) {
        if (strcmp(record.phone, old_phone) == 0) {
            found = 1;
            fseek(file, -sizeof(record), SEEK_CUR); // Move file pointer back to the beginning of the record
            fprintf(file, "%s %s %.2f\n", new_name, new_phone, record.amount); // Overwrite the record with new data
            break;
        }
    }

    fclose(file);

    if (found) {
        printf("Record updated successfully.\n");
    } else {
        printf("Record not found.\n");
    }
}

// Function to search for a record
void searchRecord() {
    char phone[MAX_PHONE_LENGTH];
    printf("\nSearching for a Record\n");
    printf("Enter phone number to search: ");
    scanf("%s", phone);
    getchar(); // Clear input buffer

    struct Record record;
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) {
        printf("No records found.\n");
        return;
    }

    int found = 0;
    while(fscanf(file, "%s %s %f", record.name, record.phone, &record.amount) != EOF) {
        if (strcmp(record.phone, phone) == 0) {
            printf("Name: %s\nPhone: %s\nAmount: %.2f\n", record.name, record.phone, record.amount);
            found = 1;
            break;
        }
    }
    fclose(file);

    if (!found)
        printf("Record not found.\n");
}

// Function to delete a record
void deleteRecord() {
    char phone[MAX_PHONE_LENGTH];
    printf("\nDeleting a Record\n");
    printf("Enter phone number to delete: ");
    scanf("%s", phone);
    getchar(); // Clear input buffer

    struct Record record;
    FILE *file = fopen(FILENAME, "r");
    FILE *temp = fopen("temp.txt", "w");
    if (file == NULL || temp == NULL) {
        printf("Error opening file.\n");
        return;
    }

    int found = 0;
    while(fscanf(file, "%s %s %f", record.name, record.phone, &record.amount) != EOF) {
        if (strcmp(record.phone, phone) == 0) {
            found = 1;
        } else {
            fprintf(temp, "%s %s %.2f\n", record.name, record.phone, record.amount);
        }
    }
    fclose(file);
    fclose(temp);

    remove(FILENAME);
    rename("temp.txt", FILENAME);

    if (found)
        printf("Record deleted successfully.\n");
    else
        printf("Record not found.\n");
}
