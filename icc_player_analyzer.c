#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
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
    PlayerNode* p = (PlayerNode*)malloc(sizeof(PlayerNode));
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
        strcpy(teamsList[index].teamName, teams[index]);
        teamsList[index].totalPlayers = 0;
        teamsList[index].avgBattingStrikeRate = 0;
        teamsList[index].playerList = NULL;
    }

    for (int index = 0; index < playerCount; index++) {
        PlayerNode* p = createPlayer(players[index]);
        for (int j = 0; j < totalTeams; j++) {
            if (strcmp(players[index].team, teamsList[j].teamName) == 0) {
                insertPlayer(&teamsList[j], p);
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
    printf("Enter Team ID to add player (1-10): ");
    scanf("%d", &teamId);
    getchar();

    Team* team = searchTeamById(teamId);
    if (!team) {
        printf("Invalid Team ID.\n");
        return;
    }

    PlayerNode* p = (PlayerNode*)malloc(sizeof(PlayerNode));

    printf("Enter Player ID: "); scanf("%d", &p->playerId);
    getchar();
    printf("Enter Player Name: "); fgets(p->name, 50, stdin);
    p->name[strcspn(p->name, "\n")] = 0;
    strcpy(p->teamName, team->teamName);

    int roleChoice;
    printf("Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
    scanf("%d", &roleChoice);
    switch (roleChoice) {
        case 1: strcpy(p->role, "Batsman"); break;
        case 2: strcpy(p->role, "Bowler"); break;
        case 3: strcpy(p->role, "All-rounder"); break;
        default: printf("Invalid Role.\n"); free(p); return;
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
    printf("Player added successfully to Team %s!\n", team->teamName);
}


void displayPlayersOfTeam() {
    int teamId;
    printf("Enter Team ID: ");
    scanf("%d", &teamId);

    Team* team = searchTeamById(teamId);
    if (!team) { printf("Invalid Team ID.\n"); return; }

    printf("\nPlayers of Team %s:\n", team->teamName);
    printf("======================================================================================================================\n");
    printf("ID\tName\t\tRole\t\tRuns\tAvg\tSR\tWkts\tER\tPerf.Index\n");
    printf("======================================================================================================================\n");

    PlayerNode* temp = team->playerList;
    while (temp) {
        printf("%d\t%-15s\t%-12s\t%d\t%.1f\t%.1f\t%d\t%.1f\t%.2f\n",
               temp->playerId, temp->name, temp->role, temp->totalRuns,
               temp->battingAverage, temp->strikeRate, temp->wickets,
               temp->economyRate, temp->performanceIndex);
        temp = temp->next;
    }
    printf("======================================================================================================================\n");
    printf("Total Players: %d\nAverage Batting Strike Rate: %.2f\n", team->totalPlayers, team->avgBattingStrikeRate);
}


int compareTeams(const void* a, const void* b) {
    Team* t1 = (Team*)a;
    Team* t2 = (Team*)b;
    return (t2->avgBattingStrikeRate > t1->avgBattingStrikeRate) - (t2->avgBattingStrikeRate < t1->avgBattingStrikeRate);
}


void displayTeamsByStrikeRate() {
    qsort(teamsList, totalTeams, sizeof(Team), compareTeams);
    printf("\nTeams Sorted by Average Batting Strike Rate:\n");
    printf("==============================================================\n");
    printf("ID\tTeam Name\t\tAvg Bat SR\tTotal Players\n");
    printf("==============================================================\n");
    for (int index = 0; index < totalTeams; index++) {
        printf("%d\t%-15s\t%.2f\t\t%d\n", teamsList[index].teamId, teamsList[index].teamName,
               teamsList[index].avgBattingStrikeRate, teamsList[index].totalPlayers);
    }
    printf("==============================================================\n");
}


int comparePlayers(const void* a, const void* b) {
    PlayerNode* p1 = *(PlayerNode**)a;
    PlayerNode* p2 = *(PlayerNode**)b;
    return (p2->performanceIndex > p1->performanceIndex) - (p2->performanceIndex < p1->performanceIndex);
}


void displayTopKPlayers() {
    int teamId, roleChoice, num_player;
    printf("Enter Team ID: "); scanf("%d", &teamId);
    printf("Enter Role (1-Batsman, 2-Bowler, 3-All-rounder): "); scanf("%d", &roleChoice);
    printf("Enter number of players (K): "); scanf("%d", &num_player);

    Team* team = searchTeamById(teamId);
    if (!team) { printf("Invalid Team ID.\n"); return; }

    char role[20];
    switch (roleChoice) {
        case 1: strcpy(role, "Batsman"); break;
        case 2: strcpy(role, "Bowler"); break;
        case 3: strcpy(role, "All-rounder"); break;
        default: printf("Invalid Role.\n"); return;
    }

    PlayerNode* temp = team->playerList;
    PlayerNode* arr[100];
    int count = 0;

    while (temp) {
        if (strcmp(temp->role, role) == 0) arr[count++] = temp;
        temp = temp->next;
    }

    qsort(arr, count, sizeof(PlayerNode*), comparePlayers);

    if (num_player > count) num_player = count;
    printf("\nTop %d %ss of Team %s:\n", num_player, role, team->teamName);
    printf("=====================================================================================================================\n");
    printf("ID\tName\t\tRole\tRuns\tAvg\tSR\tWkts\tER\tPerf.Index\n");
    printf("=====================================================================================================================\n");
    for (int index = 0; index < num_player; index++) {
        printf("%d\t%-15s\t%-12s\t%d\t%.1f\t%.1f\t%d\t%.1f\t%.2f\n",
               arr[index]->playerId, arr[index]->name, arr[index]->role, arr[index]->totalRuns,
               arr[index]->battingAverage, arr[index]->strikeRate, arr[index]->wickets,
               arr[index]->economyRate, arr[index]->performanceIndex);
    }
    printf("=====================================================================================================================\n");
}


void displayAllPlayersByRole() {
    int roleChoice;
    printf("Enter Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
    scanf("%d", &roleChoice);

    char role[20];
    switch (roleChoice) {
        case 1: strcpy(role, "Batsman"); break;
        case 2: strcpy(role, "Bowler"); break;
        case 3: strcpy(role, "All-rounder"); break;
        default: printf("Invalid Role.\n"); return;
    }

    PlayerNode* arr[300];
    int count = 0;

    for (int index = 0; index < totalTeams; index++) {
        PlayerNode* temp = teamsList[index].playerList;
        while (temp) {
            if (strcmp(temp->role, role) == 0) arr[count++] = temp;
            temp = temp->next;
        }
    }

    qsort(arr, count, sizeof(PlayerNode*), comparePlayers);

    printf("\nAll %ss Across All Teams:\n", role);
    printf("=====================================================================================================================\n");
    printf("ID\tName\t\tTeam\t\tRole\tRuns\tAvg\tSR\tWkts\tER\tPerf.Index\n");
    printf("=====================================================================================================================\n");
    for (int index = 0; index < count; index++) {
        printf("%d\t%-15s\t%-12s\t%-12s\t%d\t%.1f\t%.1f\t%d\t%.1f\t%.2f\n",
               arr[index]->playerId, arr[index]->name, arr[index]->teamName, arr[index]->role,
               arr[index]->totalRuns, arr[index]->battingAverage, arr[index]->strikeRate,
               arr[index]->wickets, arr[index]->economyRate, arr[index]->performanceIndex);
    }
    printf("=====================================================================================================================\n");
}


int main() {
    initializeTeamsAndPlayers();

    int choice;
    do {
        printf("\n==============================================================================\n");
        printf("ICC ODI Player Performance Analyzer\n");
        printf("==============================================================================\n");
        printf("1. Add Player to Team\n");
        printf("2. Display Players of a Specific Team\n");
        printf("3. Display Teams by Average Batting Strike Rate\n");
        printf("4. Display Top K Players of a Specific Team by Role\n");
        printf("5. Display all Players of specific Role Across All Teams\n");
        printf("6. Exit\n");
        printf("==============================================================================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addPlayer(); break;
            case 2: displayPlayersOfTeam(); break;
            case 3: displayTeamsByStrikeRate(); break;
            case 4: displayTopKPlayers(); break;
            case 5: displayAllPlayersByRole(); break;
            case 6: printf("Exiting...\n"); break;
            default: printf("Invalid choice! Try again.\n");
        }
    } while (choice != 6);

    return 0;
}
