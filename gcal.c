#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>

#define MAX_REMINDERS 100
#define MAX_DAYS 31
#define MAX_LENGTH 256

typedef struct {
    char reminders[MAX_REMINDERS][MAX_LENGTH];
    int count;
} Day;

Day december[MAX_DAYS];

// Print the December calendar
void print_calendar() {
    const char *days[] = {"Sun ", "Mon ", "Tue ", "Wed ", "Thu ", "Fri ", "Sat "};
    int days_in_month = 31;
    int first_day = 0; // December starts on Sunday

    for (int i = 0; i < 7; i++) printf("%s ", days[i]);
    printf("\n");

    for (int i = 0; i < first_day; i++) printf("    ");

    for (int day = 1; day <= days_in_month; day++) {
        if (december[day - 1].count > 0) {
            printf("(%2d) ", day);
        } else {
            printf(" %2d  ", day);
        }
        if ((first_day + day) % 7 == 0) printf("\n");
    }
    printf("\n\nDecember reminders:\n");

    for (int i = 0; i < MAX_DAYS; i++) {
        if (december[i].count > 0) {
	    int weekday = (first_day + i) % 7;
            printf("%s %d::\n", days[weekday], i + 1);
            for (int j = 0; j < december[i].count; j++) {
                printf("  [%d] %s\n", j + 1, december[i].reminders[j]);
            }
        }
    }
}


void view_today() {
    const char *days[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

    // Get today's date
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    int today = tm_info->tm_mday;
    int weekday = tm_info->tm_wday;

    // Print today's date
    printf("\nToday: %s %d\n", days[weekday], today);

    // Check for reminders
    if (december[today - 1].count == 0) {
        printf("No reminders for today.\n");
    } else {
        printf("Reminders:\n");
        for (int i = 0; i < december[today - 1].count; i++) {
            printf("  [%d] %s\n", i + 1, december[today - 1].reminders[i]);
        }
    }
}



// Add a reminder to a day
void add_reminder(int day, const char *reminder) {
    if (day < 1 || day > MAX_DAYS) {
        printf("Invalid day. Please enter a valid day (1-31).\n");
        return;
    }
    if (december[day - 1].count >= MAX_REMINDERS) {
        printf("Maximum reminders reached for Day %d.\n", day);
        return;
    }
    strcpy(december[day - 1].reminders[december[day - 1].count], reminder);
    december[day - 1].count++;
    printf("Reminder added for Day %d.\n", day);
    print_calendar();
}

// Remove a reminder from a day
void remove_reminder(int day, int index) {
    if (day < 1 || day > MAX_DAYS || index < 1 || index > december[day - 1].count) {
        printf("Invalid day or index. Please check and try again.\n");
        return;
    }
    for (int i = index - 1; i < december[day - 1].count - 1; i++) {
        strcpy(december[day - 1].reminders[i], december[day - 1].reminders[i + 1]);
    }
    december[day - 1].count--;
    printf("Reminder removed from Day %d.\n", day);
    print_calendar();
}

// Save reminders to a file
void save_reminders() {
    FILE *file = fopen("reminders.txt", "w");
    if (!file) {
        perror("Failed to save reminders");
        return;
    }
    for (int i = 0; i < MAX_DAYS; i++) {
        if (december[i].count > 0) {
            fprintf(file, "%d %d\n", i + 1, december[i].count);
            for (int j = 0; j < december[i].count; j++) {
                fprintf(file, "%s\n", december[i].reminders[j]);
            }
        }
    }
    fclose(file);
}

// Load reminders from a file
void load_reminders() {
    FILE *file = fopen("reminders.txt", "r");
    if (!file) return;

    int day, count;
    while (fscanf(file, "%d %d\n", &day, &count) == 2) {
        december[day - 1].count = count;
        for (int i = 0; i < count; i++) {
            fgets(december[day - 1].reminders[i], MAX_LENGTH, file);
            december[day - 1].reminders[i][strcspn(december[day - 1].reminders[i], "\n")] = 0;
        }
    }
    fclose(file);
}

// Main function
int main(int argc, char *argv[]) {
    signal(SIGINT, (void (*)(int))save_reminders);
    load_reminders();

    if (argc < 2) {
        printf("Usage: gcal <command> [arguments]\n");
        return 1;
    }

    if (strcmp(argv[1], "add") == 0 && argc >= 4) {
        int day = atoi(argv[2]);
        char reminder[MAX_LENGTH];
        strcpy(reminder, argv[3]);
        for (int i = 4; i < argc; i++) {
            strcat(reminder, " ");
            strcat(reminder, argv[i]);
        }
        add_reminder(day, reminder);
    } else if (strcmp(argv[1], "remove") == 0 && argc == 4) {
        int day = atoi(argv[2]);
        int index = atoi(argv[3]);
        remove_reminder(day, index);
    } else if (strcmp(argv[1], "view") == 0) {
        print_calendar();
    } else if (strcmp(argv[1], "today") == 0) {
        view_today();
    } else {
        printf("Invalid command. Use 'add', 'remove', 'view', or 'today'.\n");
    }
    save_reminders();
    return 0;
}

