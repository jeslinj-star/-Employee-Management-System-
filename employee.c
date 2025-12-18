#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct record {
    int id;
    char name[20];
    float salary;
};

void displaymenu();
void addrecord();
void search_record();
void remove_record();
void display();

int main() {
    int choice;
    do {
        displaymenu();
        scanf("%d", &choice);
        getchar(); // Clear newline from buffer
        
        switch(choice) {
            case 1:
                addrecord();
                break;
            case 2:
                search_record();
                break;
            case 3:
                remove_record();
                break;
            case 4:
                display();
                break;
            case 5:
                printf("Exiting program. Goodbye!\n");
                break;
            default:
                printf("Invalid choice! Please try again.\n");
        }
        printf("\n");
    } while(choice != 5);
    
    return 0;
}

void displaymenu() {
    printf("\n===== Record Management System =====\n");
    printf("1. Add Record\n");
    printf("2. Search Record\n");
    printf("3. Remove Record\n");
    printf("4. Display All Records\n");
    printf("5. Exit\n");
    printf("Enter your choice: ");
}

void addrecord() {
    struct record rec;
    FILE *fp = fopen("record.dat", "ab");
    
    if(fp == NULL) {
        printf("Error in opening file!\n");
        return;
    }
    
    printf("\nEnter ID: ");
    scanf("%d", &rec.id);
    getchar(); // Clear newline
    
    printf("Enter Name: ");
    fgets(rec.name, sizeof(rec.name), stdin);
    rec.name[strcspn(rec.name, "\n")] = '\0'; // Remove newline
    
    printf("Enter Salary: ");
    scanf("%f", &rec.salary);
    
    fwrite(&rec, sizeof(struct record), 1, fp);
    fclose(fp);
    
    printf("Record added successfully!\n");
}

void search_record() {
    struct record rec;
    int id, found = 0;
    FILE *fp = fopen("record.dat", "rb");
    
    if(fp == NULL) {
        printf("Error in opening file or no records exist!\n");
        return;
    }
    
    printf("\nEnter ID to search: ");
    scanf("%d", &id);
    
    while(fread(&rec, sizeof(struct record), 1, fp)) {
        if(rec.id == id) {
            printf("\n----- Record Found -----\n");
            printf("ID: %d\n", rec.id);
            printf("Name: %s\n", rec.name);
            printf("Salary: %.2f\n", rec.salary);
            found = 1;
            break;
        }
    }
    
    if(!found) {
        printf("Record with ID %d not found!\n", id);
    }
    
    fclose(fp);
}

void remove_record() {
    struct record rec;
    int id, found = 0;
    FILE *fp = fopen("record.dat", "rb");
    FILE *temp = fopen("temp.dat", "wb");
    
    if(fp == NULL) {
        printf("Error in opening file or no records exist!\n");
        return;
    }
    
    if(temp == NULL) {
        printf("Error creating temporary file!\n");
        fclose(fp);
        return;
    }
    
    printf("\nEnter ID to remove: ");
    scanf("%d", &id);
    
    while(fread(&rec, sizeof(struct record), 1, fp)) {
        if(rec.id == id) {
            found = 1;
            printf("Record with ID %d removed successfully!\n", id);
        } else {
            fwrite(&rec, sizeof(struct record), 1, temp);
        }
    }
    
    fclose(fp);
    fclose(temp);
    
    remove("record.dat");
    rename("temp.dat", "record.dat");
    
    if(!found) {
        printf("Record with ID %d not found!\n", id);
    }
}

void display() {
    struct record rec;
    FILE *fp = fopen("record.dat", "rb");
    
    if(fp == NULL) {
        printf("Error in opening file or no records exist!\n");
        return;
    }
    
    printf("\n===== All Records =====\n");
    printf("%-10s %-20s %-10s\n", "ID", "Name", "Salary");
    printf("--------------------------------------------\n");
    
    int count = 0;
    while(fread(&rec, sizeof(struct record), 1, fp)) {
        printf("%-10d %-20s %-10.2f\n", rec.id, rec.name, rec.salary);
        count++;
    }
    
    if(count == 0) {
        printf("No records to display!\n");
    } else {
        printf("--------------------------------------------\n");
        printf("Total records: %d\n", count);
    }
    
    fclose(fp);
}
