#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define cmax  50
#define max_teams  20
#define max_players  460
#define max_games  38
#define max_points  114


typedef struct team {
    char name[cmax];
    int points;
    int goals_conceded;
    int goals_scored;
} team;

typedef struct player{
    char name[cmax];
    char current_team[cmax];
    int goals;
} player;

team teams[max_teams];
player players[max_players];

void main_menu();
int second_menu(int a);
int manage_teams(int a);
int manage_players(int a);
int add_goal(team t, char p[cmax]);
int add_winner(team t);
int add_tie(team t1, team t2);
bool check_team(char team_name[cmax]);
bool check_player(char player_name[cmax]);
int compareTeams(const void* a, const void* b);
int comparePlayers(const void* a, const void* b);
int compareStandings(const void* a, const void* b);
int comparebyGoals(const void* a, const void* b);
int modify_goals(team t1, team t2, int g1, int g2);
int team_count();
int player_count();
int add_game();

int main()
{
    main_menu();
    return 0;
}
void main_menu()
{
    int choice;
    printf("\nWelcome to the soccer league management system!\n");
    printf("1- Manage Teams\n");
    printf("2- Manage Players\n");
    printf("3- Add Game Details\n");
    printf("4- Exit\n");
    scanf("%i", &choice);
    printf("\n______________________________\n");
    second_menu(choice);
}

int second_menu(int a)
{
    if (a == 1)
    {
        int choice;
        printf("1- Add Team\n");
        printf("2- Delete Team\n");
        printf("3- Display Team Info\n");
        printf("4- Display Standing\n");
        printf("5- Previous Page\n");
        scanf("%i", &choice);
        manage_teams(choice);
    }
    else if(a == 2)
    {
        int choice;
        printf("1- Add Player\n");
        printf("2- Delete Player\n");
        printf("3- Display Goal Scorers Standings\n");
        printf("4- Display Player Stats\n");
        printf("5- previous Page\n");
        scanf("%i", &choice);
        manage_players(choice);
    }
    else if(a == 3)
    {
       add_game();
    }
    else if(a == 4)
    {
        return 0;
    }
    else
    {
        printf("ERROR 1: Invalid Key ! \n");
        return 1;
    }
    return 0;
}

int manage_teams(int a)
{
    if (a == 1)
    {
        struct team new_team;
        printf("\nTeam name: ");
        scanf("%s", new_team.name);
        new_team.points = 0;
        new_team.goals_conceded = 0;
        new_team.goals_scored = 0;
        FILE *file;
        file = fopen("teams.txt", "a");
        fwrite(&new_team, sizeof(new_team), 1, file);
        printf("\n Team added successfully!\n");
        fclose(file);
    }

    else if (a == 2)
    {
        struct team deleted_team;
        struct team original_team;
        printf("\nTeam name: ");
        scanf("%s", deleted_team.name);
        if (!check_team(deleted_team.name))
        {
            printf("ERROR1: Team doesn't exist!");
            return 1;
        }
        FILE *file;
        file = fopen("teams.txt", "r");
        int count = 0;
        while (fread(&original_team, sizeof(struct team), 1, file))
        {
            teams[count] = original_team;
            count ++;
        }
        fclose(file);
        remove("teams.txt");
        FILE *new_file;
        new_file = fopen("teams.txt", "w");
        for (int i = 0; i < count; i++)
        {
            if (strcmp(teams[i].name,deleted_team.name) != 0)
            {
                fwrite(&teams[i], sizeof(struct team), 1, new_file);
            }
        }
        fclose(new_file);
        printf("\nTeam deleted successfully!\n");
    }
    else if (a == 3)
    {
        struct team temp;
        struct team displayed_team;
        printf("\nTeams name: ");
        scanf("%s", displayed_team.name);
        FILE *file = fopen("teams.txt", "r");
        if (!file)
        {
            printf("\nERROR: Unable to open teams file!\n");
            return false;
        }
        while (fread(&temp, sizeof(struct team), 1, file))
        {
            if (strcmp(temp.name, displayed_team.name) == 0)
            {
                printf("\nTeam name: %s\nPoints: %i\nGoals scored: %i\nGoals conceded: %i\n", temp.name, temp.points, temp.goals_scored, temp.goals_conceded);
                fclose(file);
                return 0;
            }
        }

        printf("\nTeam not found !\n");
        fclose(file);
        return 1;
    }
    else if (a == 4)
    {
        FILE *file;
        struct team temp;
        file = fopen("teams.txt", "r");
        int count = 0;
        while (fread(&temp, sizeof(struct team), 1, file))
        {
            teams[count] = temp;
            count++;
        }
        qsort(teams, count, sizeof(team), compareStandings);
        for (int i = 0; i < count; i++)
        {
            printf("\n%i- %s points: %i\n", i + 1, teams[i].name, teams[i].points);
        }

    }
    else if (a == 5)
    {
        main_menu();
        return 0;
    }
    else
    {
        printf("Key not valid !");
        return 1;
    }
    printf("\n______________________________\n");
    main_menu();
    return 0;
}
int manage_players(int a)
{
    if (a == 1)
    {
        struct player new_player;
        printf("\nPlayer name: ");
        scanf("%s", new_player.name);
        printf("\nPlayer's team: ");
        scanf("%s", new_player.current_team);
        if (check_team(new_player.current_team))
        {
            new_player.goals = 0;
            FILE *playerfile;
            playerfile = fopen("players.txt", "a");
            fwrite(&new_player, sizeof(new_player), 1, playerfile);
            printf("\n Player added successfully!\n");
            fclose(playerfile);
        }
        else
        {
            printf("\nTeam not found!\n");
            return 2;
        }

    }
    else if (a == 2)
    {
        struct player deleted_player;
        struct player original_player;
        printf("\nPlayer name: ");
        scanf("%s", deleted_player.name);
        if (!check_player(deleted_player.name))
        {
            printf("ERROR1: Player doesn't exist!");
            return 1;
        }
        FILE *file;
        file = fopen("players.txt", "r");
        int count = 0;
        while (fread(&original_player, sizeof(struct player), 1, file))
        {
            players[count] = original_player;
            count ++;
        }
        fclose(file);
        remove("players.txt");
        FILE *new_file;
        new_file = fopen("players.txt", "w");
        for (int i = 0; i < count; i++)
        {
            if (players[i].name != deleted_player.name)
            {
                fwrite(&players[i], sizeof(struct player), 1, new_file);
            }
        }
        fclose(new_file);
        printf("\nPlayer deleted successfully!");
    }
    else if(a == 3)
    {
        struct player original_player;
        FILE *file;
        file = fopen("players.txt", "r");
        int count = 0;
        while (fread(&original_player, sizeof(struct player), 1, file))
        {
            players[count] = original_player;
            count ++;
        }
        qsort(players, count, sizeof(player), comparebyGoals);
        for (int i = 0; i < count; i++)
        {
            printf("\n%i- %s, Goals: %i", i + 1, players[i].name, players[i].goals);
        }
        printf("\n");
    }
    else if (a == 4)
    {
        struct player displayed_player;
        struct player original_player;
        printf("\nPlayer name: ");
        scanf("%s", displayed_player.name);
        FILE *file;
        file = fopen("players.txt ", "r");
        int count = 0;
        while (fread(&original_player , sizeof(struct player), 1, file))
        {
            players[count] = original_player;
            count ++;
        }
        player key;
        strcpy(key.name, displayed_player.name);
        qsort(players, count, sizeof(player), comparePlayers);
        player* result = (player*) bsearch(&key, players, count, sizeof(player), comparePlayers);
        if (result != NULL)
        {
            printf("\nPlayer name: %s", result -> name);
            printf("\nPlayer's team: %s", result -> current_team);
            printf("\nNummber of goals\n: %i", result -> goals);
        }
        else
        {
            printf("\nERROR: Player not found!\n");
            return 1;
        }
        fclose(file);
    }
    else if (a == 5)
    {
        main_menu();
        return 0;
    }
    else
    {
        printf("Key not valid !");
        return 1;
    }

    printf("\n______________________________\n");
    main_menu();
    return 0;
}

int add_game()
{
    team team1, team2;
    int goals_team1 = -1, goals_team2 = -1;
    while(!check_team(team1.name))
    {
        printf("\nPlease enter the name of the home team: ");
        scanf("%s", team1.name);
    }
    while(!check_team(team2.name))
    {
        printf("\nPlease enter the name of the away team: ");
        scanf("%s", team2.name);
    }
    if(strcmp(team1.name, team2.name) == 0)
    {
        printf("ERROR: Team added twice!\n");
        return 1;
    }
    while(goals_team1 < 0)
    {
        printf("\nGoals scored by the home team: ");
        scanf("%i", &goals_team1);
    }
    while(goals_team2 < 0)
    {
        printf("\nGoals scored by the away team: ");
        scanf("%i", &goals_team2);
    }

    modify_goals(team1, team2, goals_team1, goals_team2);

    if(goals_team1 < goals_team2)
    {
        add_winner(team2);
    }
    else if(goals_team1 > goals_team2)
    {
        add_winner(team1);
    }
    else
    {
        add_tie(team1, team2);
    }

    while(goals_team1 != 0)
    {
        char goal_scorer[cmax];
        printf("\nEnter goal scorer name: ");
        scanf("%s", goal_scorer);
        add_goal(team1, goal_scorer);
        goals_team1 --;
    }
    while(goals_team2 != 0)
    {
        char goal_scorer[cmax];
        printf("\nEnter goal scorer name: ");
        scanf("%s", goal_scorer);
        add_goal(team2, goal_scorer);
        goals_team2 --;
    }

    printf("\n______________________________\n");
    main_menu();
    return 0;
}
int compareTeams(const void* a, const void* b)
{
    const team *t1 = a;
    const team *t2 = b;
    return strcmp(t1 -> name, t2 -> name);
}
int comparePlayers(const void *a, const void* b)
{
    const player *p1 = a;
    const player *p2 = b;
    return strcmp(p1 -> name, p2 -> name);
}
int compareStandings(const void* a, const void* b) {
    const team* t1 = a;
    const team* t2 = b;

    if (t1->points != t2->points)
        return t2->points - t1->points;

    int goal_difference1 = t1->goals_scored - t1->goals_conceded;
    int goal_difference2 = t2->goals_scored - t2->goals_conceded;
    if (goal_difference1 != goal_difference2)
        return goal_difference2 - goal_difference1;

    if (t1->goals_scored != t2->goals_scored)
        return t2->goals_scored - t1->goals_scored;

    return strcmp(t1->name, t2->name);
}

bool check_player(char player_name[cmax]) {
    FILE *file = fopen("players.txt", "r");
    if (!file) {
        printf("ERROR: Could not open players file!\n");
        return false;
    }

    struct player temp_player;
    while (fread(&temp_player, sizeof(struct player), 1, file)) {
        if (strcmp(temp_player.name, player_name) == 0) {
            fclose(file);
            return true;
        }
    }

    fclose(file);
    return false;
}

int comparebyGoals(const void* a, const void* b)
{
    const player *p1 = a;
    const player *p2 = b;

    if (p1->goals != p2->goals)
        return p2->goals - p1->goals;
    return strcmp(p1->name, p2->name);
}
bool check_team(char team_name[cmax]) {
    FILE *file = fopen("teams.txt", "r");
    if (!file) {
        printf("ERROR: Unable to open teams file!\n");
        return false;
    }

    struct team temp;
    while (fread(&temp, sizeof(struct team), 1, file)) {
        if (strcmp(temp.name, team_name) == 0) {
            fclose(file);
            return true;
        }
    }

    fclose(file);
    return false;
}

int add_winner(team t)
{
        struct team original_team;
        FILE *file;
        file = fopen("teams.txt", "r");
        int count = 0;
        while (fread(&original_team, sizeof(struct team), 1, file))
        {
            teams[count] = original_team;
            count ++;
        }
        fclose(file);
        remove("teams.txt");
        FILE *new_file;
        new_file = fopen("teams.txt", "w");
        for (int i = 0; i < count; i++)
        {
            if (strcmp(teams[i].name, t.name) == 0)
            {
                teams[i].points += 3;
            }
            fwrite(&teams[i], sizeof(struct team), 1, new_file);
        }
        fclose(new_file);
        return 0;
}
int add_tie(team t1, team t2)
{
        struct team original_team;
        FILE *file;
        file = fopen("teams.txt", "r");
        int count = 0;
        while (fread(&original_team, sizeof(struct team), 1, file))
        {
            teams[count] = original_team;
            count ++;
        }
        fclose(file);
        remove("teams.txt");
        FILE *new_file;
        new_file = fopen("teams.txt", "w");
        for (int i = 0; i < count; i++)
        {
            if (strcmp(teams[i].name, t1.name) == 0 || strcmp(teams[i].name, t2.name) == 0)
            {
                teams[i].points ++;
            }
            fwrite(&teams[i], sizeof(struct team), 1, new_file);
        }
        fclose(new_file);
        return 0;
}

int add_goal(team t, char p[cmax])
{
        struct player original_player;
        FILE *file;
        file = fopen("players.txt", "r");
        int count = 0;
        while (fread(&original_player, sizeof(struct player), 1, file))
        {
            players[count] = original_player;
            count ++;
        }
        fclose(file);
        remove("players.txt");
        FILE *new_file;
        new_file = fopen("players.txt", "w");
        for (int i = 0; i < count; i++)
        {
            if (strcmp(players[i].name, p) == 0 && strcmp(players[i].current_team, t.name) == 0)
            {
                players[i].goals ++;
            }
            fwrite(&players[i], sizeof(struct player), 1, new_file);
        }
        fclose(new_file);
        return 0;
}

int modify_goals(team t1, team t2, int g1, int g2)
{
        struct team original_team;
        FILE *file;
        file = fopen("teams.txt", "r");
        int count = 0;
        while (fread(&original_team, sizeof(struct team), 1, file))
        {
            teams[count] = original_team;
            count ++;
        }
        fclose(file);
        remove("teams.txt");
        FILE *new_file;
        new_file = fopen("teams.txt", "w");
        for (int i = 0; i < count; i++)
        {
            if (strcmp(teams[i].name, t1.name) == 0)
            {
                teams[i].goals_scored += g1;
                teams[i].goals_conceded += g2;
            }
            if (strcmp(teams[i].name, t2.name) == 0)
            {
                teams[i].goals_scored += g2;
                teams[i].goals_conceded += g1;
            }
            fwrite(&teams[i], sizeof(struct team), 1, new_file);
        }
        fclose(new_file);
        return 0;
}

int team_count()
{
    struct team t;
    FILE *file;
    file = fopen("teams.txt", "r");
    int ans = 0;
    while(fread(&t, sizeof(struct team), 1, file))
    {
        ans++;
    }
    return ans;
}

int player_count()
{
    struct player p;
    FILE *file;
    file = fopen("players.txt", "r");
    int ans = 0;
    while(fread(&p, sizeof(struct player), 1, file))
    {
        ans++;
    }
    return ans;
}
