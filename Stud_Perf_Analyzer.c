#include <stdio.h>
#include <string.h>

#define NUM_SUBJECTS 3
struct Student {
    int roll_no;
    char name[50];
    float marks[3];
    float total;
    float average;
    char grade;
};

float total_marks(float marks[]);
float caluculate_average(float total);
char grades(float average);
void student_Performance(char grade);
void student_roll_no(struct Student students[], int index, int studentCount);

float total_marks(float marks[]) {
    float total = 0;
    for (int index = 0; index < 3; index++) {
        total += marks[index];
    }
    return total;
}

float caluculate_average(float total) {
    return total / NUM_SUBJECTS;
}

char grades(float avg) {
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
    for (int index = 0; index < stars; index++) {
        printf("* ");
    }
    printf("\n");
}

void student_roll_no(struct Student students[], int index, int studentCount) {
    if (index == studentCount)
    return;
    printf("%d ", students[index].roll_no);
    student_roll_no(students, index + 1, studentCount);
}

int main() {
    int studentCount;
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
    students[studentIndex].total = total_marks(students[studentIndex].marks);
    students[studentIndex].average = caluculate_average(students[studentIndex].total);
    students[studentIndex].grade = grades(students[studentIndex].average);
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
    student_roll_no(students, 0, studentCount);
    printf("\n");

    return 0;
}
