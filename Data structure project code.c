
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#define MAX_NAME_LENGTH 100
#define SCREEN_WIDTH 80
#define FILENAME "playlist_data.txt"

typedef struct Song
{
    char name[MAX_NAME_LENGTH];
    int playCount;
    struct Song* next;
    struct Song* prev;
} Song;

typedef struct Playlist
{
    char name[MAX_NAME_LENGTH];
    Song* head;
    struct Playlist* next;
} Playlist;

Playlist* playlists = NULL;
Playlist* currentPlaylist = NULL;

void printCenter(const char *text)
{
    int len = strlen(text);
    int pad = (SCREEN_WIDTH - len) / 2;
    for (int i = 0; i < pad; i++) printf(" ");
    printf("%s\n", text);
}

void printLine()
{
    for (int i = 0; i < SCREEN_WIDTH; i++) printf("=");
    printf("\n");
}

Song* createSong(char name[])
{
    Song* newSong = (Song*)malloc(sizeof(Song));
    strcpy(newSong->name, name);
    newSong->playCount = 0;
    newSong->next = newSong->prev = NULL;
    return newSong;
}


void createPlaylist(char name[])
{
    Playlist* newPlaylist = (Playlist*)malloc(sizeof(Playlist));
    strcpy(newPlaylist->name, name);
    newPlaylist->head = NULL;
    newPlaylist->next = playlists;
    playlists = newPlaylist;
    currentPlaylist = newPlaylist;
    printCenter("Playlist created successfully!");
}


void switchPlaylist(char name[])
{
    Playlist* temp = playlists;
    while (temp)
    {
        if (strcmp(temp->name, name) == 0)
        {
            currentPlaylist = temp;
            printCenter("Switched to the playlist!");
            return;
        }
        temp = temp->next;
    }
    printCenter("Playlist not found!");
}


void addSong(char name[], int position)
{
    if (!currentPlaylist)
    {
        printCenter("No playlist selected!");
        return;
    }

    Song* newSong = createSong(name);
    if (!currentPlaylist->head || position == 1)
    {
        newSong->next = currentPlaylist->head;
        if (currentPlaylist->head) currentPlaylist->head->prev = newSong;
        currentPlaylist->head = newSong;
    }
    else if (position == -1)
    {
        Song* temp = currentPlaylist->head;
        while (temp->next) temp = temp->next;
        temp->next = newSong;
        newSong->prev = temp;
    }
    else
    {
        Song* temp = currentPlaylist->head;
        int i = 1;
        while (temp->next && i < position - 1)
        {
            temp = temp->next;
            i++;
        }
        newSong->next = temp->next;
        if (temp->next) temp->next->prev = newSong;
        temp->next = newSong;
        newSong->prev = temp;
    }
    printCenter("Song added successfully!");
}


void removeSong(char name[])
{
    if (!currentPlaylist)
    {
        printCenter("No playlist selected!");
        return;
    }
    if (!currentPlaylist->head)
    {
        printCenter("Playlist is empty!");
        return;
    }

    Song* temp = currentPlaylist->head;
    while (temp)
    {
        if (strcmp(temp->name, name) == 0)
        {
            if (temp->prev) temp->prev->next = temp->next;
            if (temp->next) temp->next->prev = temp->prev;
            if (temp == currentPlaylist->head) currentPlaylist->head = temp->next;
            free(temp);
            printCenter("Song removed successfully!");
            return;
        }
        temp = temp->next;
    }
    printCenter("Song not found!");
}


void playNext(Song** current)
{
     if (currentPlaylist == NULL || currentPlaylist->head == NULL)
    {
        printCenter("No songs available to play.\n");
        return;
    }

    if (currentPlaylist->currentlyPlaying == NULL)
    {
        currentPlaylist->currentlyPlaying = currentPlaylist->head;
    }
    else if (currentPlaylist->currentlyPlaying->next != NULL)
    {
        currentPlaylist->currentlyPlaying = currentPlaylist->currentlyPlaying->next;
    }
    else
    {
        printCenter("You're at the last song.\n");
        return;
}

  currentPlaylist->currentlyPlaying->playCount++;
    char buffer[150];
    sprintf(buffer, " Now Playing: %s | Play Count: %d 🎶", currentPlaylist->currentlyPlaying->name, currentPlaylist->currentlyPlaying->playCount);
    printCenter(buffer);


void playPrevious(Song** current)
{

    if (currentPlaylist == NULL || currentPlaylist->head == NULL)
    {
        printCenter("No songs available to play.\n");
        return;
    }

    if (currentPlaylist->currentlyPlaying == NULL)
    {
        currentPlaylist->currentlyPlaying = currentPlaylist->head;
        printCenter("Starting from the first song.");
    }
    else if (currentPlaylist->currentlyPlaying->prev != NULL)
    {
        currentPlaylist->currentlyPlaying = currentPlaylist->currentlyPlaying->prev;
    }
    else
    {
        printCenter("You're at the first song.\n");
        return;
    }

   currentPlaylist->currentlyPlaying->playCount++;
    char buffer[150];
    sprintf(buffer, " Now Playing: %s | Play Count: %d 🎶", currentPlaylist->currentlyPlaying->name, currentPlaylist->currentlyPlaying->playCount);
    printCenter(buffer);

}

void switchPlaylist(char name[])
{
    Playlist* temp = playlists;
    while (temp != NULL)
    {
        if (strcmp(temp->name, name) == 0)
        {
            currentPlaylist = temp;
            if (currentPlaylist->head != NULL)
                currentPlaylist->currentlyPlaying = currentPlaylist->head; // Reset on switch
            printCenter("Switched playlist successfully.");
            return;
        }
        temp = temp->next;
    }
    printCenter("Playlist not found.");
}


void displayPlaylist()
{
    if (!currentPlaylist)
    {
        printCenter("No playlist selected!");
        return;
    }
    if (!currentPlaylist->head)
    {
        printCenter("Playlist is empty!");
        return;
    }

    printLine();
    printCenter(currentPlaylist->name);
    printLine();

    Song* temp = currentPlaylist->head;
    while (temp)
    {
        int pad = (SCREEN_WIDTH - 30) / 2;
        for (int i = 0; i < pad; i++) printf(" ");
        printf("Song: %-20s | Played: %d\n", temp->name, temp->playCount);
        temp = temp->next;
    }
    printLine();
}


void sortPlaylist()
{
    if (!currentPlaylist || !currentPlaylist->head || !currentPlaylist->head->next) return;
    Song* i = currentPlaylist->head;
    while (i)
    {
        Song* j = i->next;
        while (j)
        {
            if (strcmp(i->name, j->name) > 0)
            {
                char tempName[MAX_NAME_LENGTH];
                int tempCount;
                strcpy(tempName, i->name);
                strcpy(i->name, j->name);
                strcpy(j->name, tempName);

                tempCount = i->playCount;
                i->playCount = j->playCount;
                j->playCount = tempCount;
            }
            j = j->next;
        }
        i = i->next;
    }
    printCenter("Playlist sorted by song name!");
}


void shufflePlaylist()
{
    if (!currentPlaylist || !currentPlaylist->head || !currentPlaylist->head->next)
    {
        printCenter("Not enough songs to shuffle!");
        return;
    }

    srand(time(0));
    int n = 0;
    Song* temp = currentPlaylist->head;
    while (temp)
    {
        n++;
        temp = temp->next;
    }

    for (int i = 0; i < n; i++)
    {
        Song* p1 = currentPlaylist->head;
        Song* p2 = currentPlaylist->head;
        int j = rand() % n;
        int k = rand() % n;
        for (int x = 0; x < j; x++) if (p1->next) p1 = p1->next;
        for (int x = 0; x < k; x++) if (p2->next) p2 = p2->next;

        char tempName[MAX_NAME_LENGTH];
        strcpy(tempName, p1->name);
        strcpy(p1->name, p2->name);
        strcpy(p2->name, tempName);

        int tempCount = p1->playCount;
        p1->playCount = p2->playCount;
        p2->playCount = tempCount;
    }
    printCenter("Playlist shuffled!");
}

void searchSong(char name[])
{
    if (!currentPlaylist)
    {
        printCenter("No playlist selected!");
        return;
    }
    if (!currentPlaylist->head)
    {
        printCenter("Playlist is empty!");
        return;
    }

    Song* temp = currentPlaylist->head;
    while (temp)
    {
        if (strcmp(temp->name, name) == 0)
        {
            printCenter("Song found!");
            printf("\nSong: %-20s | Played: %d\n", temp->name, temp->playCount);
            return;
        }
        temp = temp->next;
    }
    printCenter("Song not found!");
}


void getTotalInfo()
{
    if (!currentPlaylist)
    {
        printCenter("No playlist selected!");
        return;
    }
    if (!currentPlaylist->head)
    {
        printCenter("Playlist is empty!");
        return;
    }

    int totalSongs = 0;
    int totalPlayCount = 0;
    Song* temp = currentPlaylist->head;
    while (temp)
    {
        totalSongs++;
        totalPlayCount += temp->playCount;
        temp = temp->next;
    }
    printf("\nTotal Songs: %d | Total Play Count: %d\n", totalSongs, totalPlayCount);
}

void clearPlaylist()
{
    if (!currentPlaylist)
    {
        printCenter("No playlist selected!");
        return;
    }
    Song* temp = currentPlaylist->head;
    while (temp)
    {
        Song* nextSong = temp->next;
        free(temp);
        temp = nextSong;
    }
    currentPlaylist->head = NULL;
    printCenter("Playlist cleared!");
}


void savePlaylist()
{
    FILE* file = fopen(FILENAME, "w");
    if (!file)
    {
        printCenter("Unable to save playlist!");
        return;
    }

    Playlist* tempPlaylist = playlists;
    while (tempPlaylist)
    {
        fprintf(file, "Playlist: %s\n", tempPlaylist->name);
        Song* tempSong = tempPlaylist->head;
        while (tempSong)
        {
            fprintf(file, "Song: %s | Played: %d\n", tempSong->name, tempSong->playCount);
            tempSong = tempSong->next;
        }
        tempPlaylist = tempPlaylist->next;
    }

    fclose(file);
    printCenter("Playlist saved successfully!");
}

void loadPlaylists()
{
    FILE* file = fopen("playlists.txt", "r");
    if (file == NULL)
    {
        printCenter("No saved playlists found.");
        return;
    }

    char line[MAX_NAME_LENGTH * 4];
    Playlist* lastPlaylist = NULL;
    currentPlaylist = NULL;

    while (fgets(line, sizeof(line), file))
    {
        line[strcspn(line, "\n")] = '\0';

        if (strncmp(line, "Playlist:", 9) == 0)
        {
            char playlistName[MAX_NAME_LENGTH];
            sscanf(line, "Playlist: %[^\n]", playlistName);

            Playlist* newPlaylist = (Playlist*)malloc(sizeof(Playlist));
            strcpy(newPlaylist->name, playlistName);
            newPlaylist->head = NULL;
            newPlaylist->currentlyPlaying = NULL;
            newPlaylist->next = NULL;

            if (playlists == NULL)
            {
                playlists = newPlaylist;
            }
            else
            {
                lastPlaylist->next = newPlaylist;
            }

            lastPlaylist = newPlaylist;
            currentPlaylist = newPlaylist;
        }
        else if (strlen(line) > 0)
        {

            char name[MAX_NAME_LENGTH], artist[MAX_NAME_LENGTH], duration[MAX_NAME_LENGTH], genre[MAX_NAME_LENGTH];
            int playCount;
            sscanf(line, "%[^|]|%[^|]|%[^|]|%[^|]|%d", name, artist, duration, genre, &playCount);

            Song* newSong = (Song*)malloc(sizeof(Song));
            strcpy(newSong->name, name);
            strcpy(newSong->artist, artist);
            strcpy(newSong->duration, duration);
            strcpy(newSong->genre, genre);
            newSong->playCount = playCount;
            newSong->next = newSong->prev = NULL;

            if (currentPlaylist->head == NULL)
            {
                currentPlaylist->head = newSong;
                currentPlaylist->currentlyPlaying = newSong;
            }
            else
            {
                Song* temp = currentPlaylist->head;
                while (temp->next != NULL)
                    temp = temp->next;
                temp->next = newSong;
                newSong->prev = temp;
            }
        }
    }

    fclose(file);
    printCenter(" Playlists loaded successfully.");
}


void menu()
{
    int choice, pos;
    char name[MAX_NAME_LENGTH];

    while (1)
    {
        printf("\n");
        printLine();
        printCenter("╔════════════════════════════════════════════════════════════════════════════╗");
        printCenter("║                                                                            ║");
        printCenter("║                   WELCOME TO MUSIC PLAYLIST ORGANIZER                  ║");
        printCenter("║                                                                            ║");
        printCenter("╠════════════════════════════════════════════════════════════════════════════╣");
        printCenter("║ 1️.Create Playlist                                                          ║");
        printCenter("║ 2️. Switch Playlist                                                         ║");
        printCenter("║ 3️. Add Song                                                                ║");
        printCenter("║ 4️. Remove Song                                                             ║");
        printCenter("║ 5️. Display Playlist                                                        ║");
        printCenter("║ 6️. Sort Playlist                                                           ║");
        printCenter("║ 7️. Shuffle Playlist                                                        ║");
        printCenter("║ 8️. Play Previous Song                                                      ║");
        printCenter("║ 9️. Play Next Song                                                          ║");
        printCenter("║ 10. Search Song                                                            ║");
        printCenter("║ 1️1. Get Total Info                                                         ║");
        printCenter("║ 1️2.Clear Playlist                                                          ║");
        printCenter("║ 1️3.Save Playlist                                                           ║");
        printCenter("║ 1️4.Load Playlist                                                           ║");
        printCenter("║ 1️5.Exit                                                                    ║");
        printCenter("╚════════════════════════════════════════════════════════════════════════════╝");
        printLine();

       int boxWidth = 36;
        int boxPadding = (SCREEN_WIDTH - boxWidth) / 2;
        for (int i = 0; i < boxPadding; i++) printf(" ");
        printf("╔══════════════════════════════════╗\n");

        for (int i = 0; i < boxPadding; i++) printf(" ");
        printf("║  Enter your choice: ");
        scanf("%d", &choice);
        getchar();
        printf("                                                               ║\n");

        for (int i = 0; i < boxPadding; i++) printf(" ");
        printf("╚══════════════════════════════════╝\n");

        printf("\n");
        switch (choice)
        {
        case 1:
            printf("Enter playlist name: ");
            fgets(name, MAX_NAME_LENGTH, stdin);
            name[strcspn(name, "\n")] = 0;
            createPlaylist(name);
            break;
        case 2:
            printf("Enter playlist name to switch to: ");
            fgets(name, MAX_NAME_LENGTH, stdin);
            name[strcspn(name, "\n")] = 0;
            switchPlaylist(name);
            break;
        case 3:
            printf("Enter song name: ");
            fgets(name, MAX_NAME_LENGTH, stdin);
            name[strcspn(name, "\n")] = 0;
            printf("Enter position (1=beginning, -1=end, or position number): ");
            scanf("%d", &pos);
            getchar();
            addSong(name, pos);
            break;
        case 4:
            printf("Enter song name to remove: ");
            fgets(name, MAX_NAME_LENGTH, stdin);
            name[strcspn(name, "\n")] = 0;
            removeSong(name);
            break;
        case 5:
            displayPlaylist();
            break;
        case 6:
            sortPlaylist();
            break;
        case 7:
            shufflePlaylist();
            break;
        case 8:
            playPrevious(&currentPlaylist->head);
            break;
        case 9:
            playNext(&currentPlaylist->head);
            break;
        case 10:
            printf("Enter song name to search: ");
            fgets(name, MAX_NAME_LENGTH, stdin);
            name[strcspn(name, "\n")] = 0;
            searchSong(name);
            break;
        case 11:
            getTotalInfo();
            break;
        case 12:
            clearPlaylist();
            break;
        case 13:
            savePlaylist();
            break;
        case 14:
            loadPlaylist();
            break;
        case 15:
            return;
        default:
            printCenter("Invalid choice!");
        }
    }
}

int main()
{
    loadPlaylist();
    menu();

    return 0;
}
