#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;
    char name[50];
    float marks;
} Student;

void addStudent();
void displayStudents();
void updateStudent();
void deleteStudent();

const char *filename = "students.dat";

int main() {
    int choice;

    while (1) {
        printf("\nStudent Management System\n");
        printf("1. Add Student\n");
        printf("2. Display Students\n");
        printf("3. Update Student\n");
        printf("4. Delete Student\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addStudent();
                break;
            case 2:
                displayStudents();
                break;
            case 3:
                updateStudent();
                break;
            case 4:
                deleteStudent();
                break;
            case 5:
                exit(0);
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
    return 0;
}

void addStudent() {
    FILE *file = fopen(filename, "ab");
    if (!file) {
        printf("Error opening file.\n");
        return;
    }

    Student student;
    printf("Enter ID: ");
    scanf("%d", &student.id);
    printf("Enter Name: ");
    scanf(" %[^\n]", student.name);
    printf("Enter Marks: ");
    scanf("%f", &student.marks);

    fwrite(&student, sizeof(Student), 1, file);
    fclose(file);

    printf("Student added successfully.\n");
}

void displayStudents() {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        printf("No records found.\n");
        return;
    }

    Student student;
    printf("\nStudent Records:\n");
    printf("ID\tName\t\tMarks\n");
    printf("--------------------------------\n");
    while (fread(&student, sizeof(Student), 1, file)) {
        printf("%d\t%-10s\t%.2f\n", student.id, student.name, student.marks);
    }

    fclose(file);
}

void updateStudent() {
    FILE *file = fopen(filename, "rb+");
    if (!file) {
        printf("No records found.\n");
        return;
    }

    int id, found = 0;
    printf("Enter ID to update: ");
    scanf("%d", &id);

    Student student;
    while (fread(&student, sizeof(Student), 1, file)) {
        if (student.id == id) {
            found = 1;
            printf("Enter new Name: ");
            scanf(" %[^\n]", student.name);
            printf("Enter new Marks: ");
            scanf("%f", &student.marks);

            fseek(file, -sizeof(Student), SEEK_CUR);
            fwrite(&student, sizeof(Student), 1, file);
            printf("Student updated successfully.\n");
            break;
        }
    }

    if (!found) {
        printf("Student with ID %d not found.\n", id);
    }

    fclose(file);
}

void deleteStudent() {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        printf("No records found.\n");
        return;
    }

    FILE *temp = fopen("temp.dat", "wb");
    if (!temp) {
        printf("Error creating temporary file.\n");
        fclose(file);
        return;
    }

    int id, found = 0;
    printf("Enter ID to delete: ");
    scanf("%d", &id);

    Student student;
    while (fread(&student, sizeof(Student), 1, file)) {
        if (student.id == id) {
            found = 1;
            continue;
        }
        fwrite(&student, sizeof(Student), 1, temp);
    }

    fclose(file);
    fclose(temp);

    remove(filename);
    rename("temp.dat", filename);

    if (found) {
        printf("Student deleted successfully.\n");
    } else {
        printf("Student with ID %d not found.\n", id);
    }
}
