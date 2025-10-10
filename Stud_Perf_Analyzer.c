#include <stdio.h>
#include <string.h>

struct Student {
    int roll_no;
    char name[50];
    float marks[3];
    float total;
    float average;
    char grade;
};

float total_Marks(float marks[]);
float _Average(float total);
char _Grade(float average);
void student_Performance(char grade);
void studentRollNo(struct Student students[], int index, int studentCount);

float total_Marks(float marks[]) {
    float total = 0;
    for (int i = 0; i < 3; i++) {
        total += marks[i];
    }
    return total;
}

float _Average(float total) {
    return total / 3;
}

char _Grade(float avg) {
    if (avg >= 85) return 'A';
    else if (avg >= 70) return 'B';
    else if (avg >= 50) return 'C';
    else if (avg >= 35) return 'D';
    else return 'F';
}

void student_Performance(char grade) {
    int stars = 0;
    switch (grade) {
        case 'A': stars = 5; break;
        case 'B': stars = 4; break;
        case 'C': stars = 3; break;
        case 'D': stars = 2; break;
        default: stars = 0; break;
    }
    for (int i = 0; i < stars; i++) {
        printf("* ");
    }
    printf("\n");
}

void studentRollNo(struct Student students[], int index, int studentCount) {
    if (index == studentCount)
    return;
    printf("%d ", students[index].roll_no);
    studentRollNo(students, index + 1, studentCount);
}

int main() {
    int studentCount;
    printf("Enter the Number of Students: ");
    scanf("%d", &studentCount);

    if (studentCount < 1 || studentCount > 100) {
        printf("Invalid Number! Please enter between (1 - 100)\n");
        return 1;
    }

    struct Student students[studentCount];
    getchar();

    for (int studentIndex = 0; studentIndex < studentCount; studentIndex++) {
    printf("\nEnter details for student %d (RollNo Name Marks1 Marks2 Marks3):\n", studentIndex + 1);

    if (scanf("%d %49s %f %f %f", 
              &students[studentIndex].roll_no, 
              students[studentIndex].name, 
              &students[studentIndex].marks[0], 
              &students[studentIndex].marks[1], 
              &students[studentIndex].marks[2]) != 5) {
        printf("Invalid input! please enter RollNo, Name, and 3 marks.\n");
        return 1;
    }

    if (students[studentIndex].roll_no < 1 || students[studentIndex].roll_no > 100) {
        printf("Invalid Roll Number! please enter between 1 and 100.\n");
        return 1;
    }

    for (int existingRoll = 0; existingRoll < studentIndex; existingRoll++) {
        if (students[existingRoll].roll_no == students[studentIndex].roll_no) {
            printf("Duplicate roll number found: %d\n", students[studentIndex].roll_no);
            return 1;
        }
    }

    for (int subjectIndex = 0; subjectIndex < 3; subjectIndex++) {
        if (students[studentIndex].marks[subjectIndex] < 0 || students[studentIndex].marks[subjectIndex] > 100) {
            printf("Invalid Marks! please enter marks between 0 and 100.\n");
            return 1;
        }
    }
    students[studentIndex].total = total_Marks(students[studentIndex].marks);
    students[studentIndex].average = _Average(students[studentIndex].total);
    students[studentIndex].grade = _Grade(students[studentIndex].average);
}

    printf("Student Performance Analysis\n");

    for (int studentIndex = 0; studentIndex < studentCount; studentIndex++) {
        printf("\nRoll No: %d\n", students[studentIndex].roll_no);
        printf("Name: %s\n", students[studentIndex].name);
        printf("Total: %.2f\n", students[studentIndex].total);
        printf("Average: %.2f\n", students[studentIndex].average);
        printf("Grade: %c\n", students[studentIndex].grade);

        if (students[studentIndex].grade != 'F') {
            printf("Performance: ");
            student_Performance(students[studentIndex].grade);
        } else {
            printf("Performance: fail...\n");
        }
    }

    printf("\nList of Roll Numbers: ");
    studentRollNo(students, 0, studentCount);
    printf("\n");

    return 0;
}

