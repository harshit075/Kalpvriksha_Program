#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Players_data.h"



typedef struct PlayerNode {
    int playerId;
    char name[50];
    char teamName[50];
    char role[20];
    int totalRuns;
    float battingAverage;
    float strikeRate;
    int wickets;
    float economyRate;
    float performanceIndex;
    struct PlayerNode* next;
} PlayerNode;

typedef struct {
    int teamId;
    char teamName[50];
    int totalPlayers;
    float avgBattingStrikeRate;
    PlayerNode* playerList;
} Team;



Team teamsList[10];
int totalTeams = 10;


int isPlayerIdExists(int id) {
    for (int index = 0; index < totalTeams; index++) {
        PlayerNode* temp = teamsList[index].playerList;
        while (temp) {
            if (temp->playerId == id) return 1;
            temp = temp->next;
        }
    }
    return 0;
}


float computePerformanceIndex(PlayerNode* p) {
    if (strcmp(p->role, "Batsman") == 0) {
        return (p->battingAverage * p->strikeRate) / 100.0;
    } else if (strcmp(p->role, "Bowler") == 0) {
        return (p->wickets * 2.0) + (100.0 - p->economyRate);
    } else if (strcmp(p->role, "All-rounder") == 0) {
        return ((p->battingAverage * p->strikeRate) / 100.0) + (p->wickets * 2.0);
    }
    return 0.0;
}


PlayerNode* createPlayer(const Player data) {
    PlayerNode* p = malloc(sizeof(PlayerNode));

    if (!p) {
        printf("Memory allocation failed! Player skipped.\n");
        return NULL;
    }

    p->playerId = data.id;
    strcpy(p->name, data.name);
    strcpy(p->teamName, data.team);
    strcpy(p->role, data.role);
    p->totalRuns = data.totalRuns;
    p->battingAverage = data.battingAverage;
    p->strikeRate = data.strikeRate;
    p->wickets = data.wickets;
    p->economyRate = data.economyRate;
    p->performanceIndex = computePerformanceIndex(p);
    p->next = NULL;

    return p;
}

void insertPlayer(Team* team, PlayerNode* newPlayer) {
    newPlayer->next = team->playerList;
    team->playerList = newPlayer;
    team->totalPlayers++;
}


void updateTeamStrikeRate(Team* team) {
    float totalSR = 0;
    int count = 0;

    PlayerNode* temp = team->playerList;
    while (temp) {
        if (strcmp(temp->role, "Batsman") == 0 || strcmp(temp->role, "All-rounder") == 0) {
            totalSR += temp->strikeRate;
            count++;
        }
        temp = temp->next;
    }

    team->avgBattingStrikeRate = (count > 0) ? totalSR / count : 0;
}


void initializeTeamsAndPlayers() {
    for (int index = 0; index < totalTeams; index++) {
        teamsList[index].teamId = index + 1;
        strcpy(teamsList[i].teamName, teams[index]);
        teamsList[index].totalPlayers = 0;
        teamsList[index].avgBattingStrikeRate = 0;
        teamsList[index].playerList = NULL;
    }

    for (int index = 0; index < playerCount; index++) {
        if (isPlayerIdExists(players[index].id)) continue; 
        PlayerNode* p = createPlayer(players[index]);
        if (!p) continue;

        for (int t = 0; t < totalTeams; t++) {
            if (strcmp(players[i].team, teamsList[t].teamName) == 0) {
                insertPlayer(&teamsList[t], p);
                break;
            }
        }
    }

    for (int index = 0; index < totalTeams; index++) {
        updateTeamStrikeRate(&teamsList[index]);
    }
}


Team* searchTeamById(int id) {
    int low = 0, high = totalTeams - 1;
    while (low <= high) {
        int mid = (low + high) / 2;
        if (teamsList[mid].teamId == id) return &teamsList[mid];
        else if (teamsList[mid].teamId < id) low = mid + 1;
        else high = mid - 1;
    }
    return NULL;
}


void addPlayer() {
    int teamId;
    printf("Enter Team ID (1-10): ");
    scanf("%d", &teamId);
    getchar();

    Team* team = searchTeamById(teamId);
    if (!team) {
        printf("Invalid team!\n");
        return;
    }

    PlayerNode* p = malloc(sizeof(PlayerNode));
    if (!p) {
        printf("Memory allocation failed!\n");
        return;
    }

    printf("Enter Player ID: ");
    scanf("%d", &p->playerId);
    getchar();

    if (isPlayerIdExists(p->playerId)) {
        printf("Player ID already exists!\n");
        free(p);
        return;
    }

    printf("Enter Player Name: ");
    fgets(p->name, 50, stdin);
    p->name[strcspn(p->name, "\n")] = 0;

    strcpy(p->teamName, team->teamName);

    int choice;
    printf("Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
    scanf("%d", &choice);

    switch (choice) {
        case 1: strcpy(p->role, "Batsman"); break;
        case 2: strcpy(p->role, "Bowler"); break;
        case 3: strcpy(p->role, "All-rounder"); break;
        default:
            printf("Invalid role.\n");
            free(p);
            return;
    }

    printf("Total Runs: "); scanf("%d", &p->totalRuns);
    printf("Batting Average: "); scanf("%f", &p->battingAverage);
    printf("Strike Rate: "); scanf("%f", &p->strikeRate);
    printf("Wickets: "); scanf("%d", &p->wickets);
    printf("Economy Rate: "); scanf("%f", &p->economyRate);

    p->performanceIndex = computePerformanceIndex(p);
    p->next = NULL;

    insertPlayer(team, p);
    updateTeamStrikeRate(team);

    printf("Player added successfully!\n");
}


void displayPlayersOfTeam() {
    int id;
    printf("Enter Team ID: ");
    scanf("%d", &id);

    Team* team = searchTeamById(id);
    if (!team) { printf("Invalid team.\n"); return; }

    printf("\nPlayers of Team %s:\n", team->teamName);
    printf("====================================================================================\n");
    printf("ID\tName\t\tRole\tRuns\tAvg\tSR\tWkts\tER\tPerfIndex\n");
    printf("====================================================================================\n");

    PlayerNode* t = team->playerList;
    while (t) {
        printf("%d\t%-12s\t%-12s\t%d\t%.1f\t%.1f\t%d\t%.1f\t%.2f\n",
               t->playerId, t->name, t->role, t->totalRuns, t->battingAverage,
               t->strikeRate, t->wickets, t->economyRate, t->performanceIndex);
        t = t->next;
    }

    printf("====================================================================================\n");
    printf("Total Players: %d\nAvg Batting SR: %.2f\n", team->totalPlayers, team->avgBattingStrikeRate);
}


int compareTeams(const void* a, const void* b) {
    Team* x = (Team*)a;
    Team* y = (Team*)b;
    return (y->avgBattingStrikeRate > x->avgBattingStrikeRate) - (y->avgBattingStrikeRate < x->avgBattingStrikeRate);
}

int comparePlayers(const void* a, const void* b) {
    PlayerNode* p1 = *(PlayerNode**)a;
    PlayerNode* p2 = *(PlayerNode**)b;
    return (p2->performanceIndex > p1->performanceIndex) - (p2->performanceIndex < p1->performanceIndex);
}

// Display teams sorted
void displayTeamsByStrikeRate() {
    qsort(teamsList, totalTeams, sizeof(Team), compareTeams);

    printf("\nTeams Sorted by Strike Rate:\n");
    printf("======================================================\n");
    printf("ID\tTeam\t\tAvg SR\tPlayers\n");
    printf("======================================================\n");

    for (int index = 0; index < totalTeams; index++) {
        printf("%d\t%-15s\t%.2f\t%d\n",
               teamsList[index].teamId,
               teamsList[index].teamName,
               teamsList[index].avgBattingStrikeRate,
               teamsList[index].totalPlayers);
    }
}


void displayTopKPlayers() {
    int teamId, roleChoice, k;

    printf("Enter Team ID: ");
    scanf("%d", &teamId);
    printf("Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
    scanf("%d", &roleChoice);
    printf("Enter K: ");
    scanf("%d", &k);

    Team* team = searchTeamById(teamId);
    if (!team) { printf("Invalid team.\n"); return; }

    char role[20];
    if (roleChoice == 1) strcpy(role, "Batsman");
    else if (roleChoice == 2) strcpy(role, "Bowler");
    else if (roleChoice == 3) strcpy(role, "All-rounder");
    else { printf("Invalid role.\n"); return; }

    PlayerNode* arr[200];
    int count = 0;

    PlayerNode* t = team->playerList;
    while (t) {
        if (strcmp(t->role, role) == 0)
            arr[count++] = t;
        t = t->next;
    }

    qsort(arr, count, sizeof(PlayerNode*), comparePlayers);

    if (k > count) k = count;

    printf("\nTop %d %s of %s:\n", k, role, team->teamName);
    printf("===============================================================\n");
    for (int i = 0; i < k; i++) {
        printf("%d\t%-12s\t%.2f\n", arr[i]->playerId, arr[i]->name, arr[i]->performanceIndex);
    }
}


void displayAllPlayersByRole() {
    int roleChoice;
    printf("Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
    scanf("%d", &roleChoice);

    char role[20];
    if (roleChoice == 1) strcpy(role, "Batsman");
    else if (roleChoice == 2) strcpy(role, "Bowler");
    else if (roleChoice == 3) strcpy(role, "All-rounder");
    else { printf("Invalid role.\n"); return; }

    PlayerNode* arr[500];
    int count = 0;

    for (int index = 0; index < totalTeams; index++) {
        PlayerNode* t = teamsList[index].playerList;
        while (t) {
            if (strcmp(t->role, role) == 0)
                arr[count++] = t;
            t = t->next;
        }
    }

    qsort(arr, count, sizeof(PlayerNode*), comparePlayers);

    printf("\nAll %s Players Across Teams:\n", role);
    printf("===============================================================\n");

    for (int index = 0; index < count; index++) {
        printf("%d\t%-12s\t%-12s\t%.2f\n",
               arr[index]->playerId, arr[index]->name, arr[index]->teamName, arr[index]->performanceIndex);
    }
}

void freeTeamPlayers(Team* t) {
    PlayerNode* curr = t->playerList;
    while (curr) {
        PlayerNode* next = curr->next;
        free(curr);
        curr = next;
    }
}

void freeAllTeams() {
    for (int index = 0; index < totalTeams; index++) {
        freeTeamPlayers(&teamsList[index]);
    }
}



int main() {
    initializeTeamsAndPlayers();

    int choice;
    do {
        printf("\n=============================================\n");
        printf(" ICC ODI Player Performance Analyzer\n");
        printf("=============================================\n");
        printf("1. Add Player\n");
        printf("2. Display Players of a Team\n");
        printf("3. Display Teams by Strike Rate\n");
        printf("4. Display Top K Players by Role\n");
        printf("5. Display All Players by Role\n");
        printf("6. Exit\n");
        printf("=============================================\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addPlayer(); break;
            case 2: displayPlayersOfTeam(); break;
            case 3: displayTeamsByStrikeRate(); break;
            case 4: displayTopKPlayers(); break;
            case 5: displayAllPlayersByRole(); break;
            case 6: 
                printf("Cleaning memory and exiting...\n");
                freeAllTeams();
                break;
            default:
                printf("Invalid choice!\n");
        }

    } while (choice != 6);

    return 0;
}
