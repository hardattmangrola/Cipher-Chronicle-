#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ENTRIES 100

#define KEY 0xAA // Example key for XOR encryption

#define PASSWORD "admin"

struct DiaryEntry
{
    char date[11];       
    char content[1000];  
};

struct Diary
{
    struct DiaryEntry entries[MAX_ENTRIES];
    int count;
};
int authenticate()
{
    char password[50];
    printf("Enter the password: ");
    scanf("%s", password);

    if (strcmp(password, PASSWORD) == 0)
    {
        return 1; // Authentication successful
    }
    else
    {
        printf("Authentication failed. Exiting the program.\n");
        return 0; // Authentication failed
    }
}
// Function to add an entry to the diary
void addEntry(struct Diary *diary)
{
    if (diary->count < MAX_ENTRIES)
    {
        printf("Enter the date (format: yyyy-mm-dd): ");
        scanf("%10s", diary->entries[diary->count].date);

        printf("Enter the diary content:\n");
        scanf(" %[^\n]s", diary->entries[diary->count].content);

        // Increment the count after adding an entry
        printf("Entry added successfully!\n");
        diary->count++;
    }
    else
    {
        printf("Diary is full. Cannot add more entries.\n");
    }

}

// Function to save the diary entries to a file
void saveDiary(struct Diary *diary)
{
    FILE *file = fopen("diary.txt", "w");

    if (file != NULL)
    {
        for (int i = 0; i < diary->count; i++)
        {
            fprintf(file, "Entry %d\n", i + 1);
            fprintf(file, "Date: %s\n", diary->entries[i].date);
            fprintf(file, "Content: %s\n", diary->entries[i].content);
            fprintf(file, "\n");
        }

        fclose(file);

        printf("Diary entries saved to diary.txt\n");
    }
    else
    {
        printf("Error opening file for writing.\n");
    }
}

// Function to view the diary entries from a file sorted by date
void viewEntries(struct Diary *diary)
{
    // Sorting entries by date (bubble sort)
    for (int i = 0; i < diary->count - 1; i++)
    {
        for (int j = 0; j < diary->count - i - 1; j++)
        {
            if (strcmp(diary->entries[j].date, diary->entries[j + 1].date) > 0)
            {
                // Swap entries
                struct DiaryEntry temp = diary->entries[j];
                diary->entries[j] = diary->entries[j + 1];
                diary->entries[j + 1] = temp;
            }
        }
    }

    printf("Diary Entries (Sorted by Date):\n");
    for (int i = 0; i < diary->count; i++)
    {
        printf("Entry %d\n", i + 1);
        printf("Date: %s\n", diary->entries[i].date);
        printf("Content: %s\n", diary->entries[i].content);
        printf("\n");
    }
}

// Function to delete an entry from the diary
void deleteEntry(struct Diary *diary)
{
    int entryNumber;
    printf("Enter the entry number to delete: ");
    scanf("%d", &entryNumber);

    if (entryNumber >= 1 && entryNumber <= diary->count)
    {
        for (int i = entryNumber - 1; i < diary->count - 1; i++)
        {
            diary->entries[i] = diary->entries[i + 1];
        }

        // Clear the last entry after shifting
        strcpy(diary->entries[diary->count - 1].date, "");
        strcpy(diary->entries[diary->count - 1].content, "");

        // Decrement the count after deleting an entry
        printf("Entry deleted successfully!\n");
        diary->count--;
    }
    else
    {
        printf("Invalid entry number.\n");
    }
}


// Structure to represent an encrypted file
struct EncryptedFile {
    char filename[100];
    size_t size;
    unsigned char* data;
};

// Function prototypes
void encryptFile(const char* filename);
void decryptFile(const char* filename);
void saveEncryptedFile(const char* filename, struct EncryptedFile* encryptedFile);
void loadEncryptedFile(const char* filename, struct EncryptedFile* encryptedFile);
void cleanup(struct EncryptedFile* encryptedFile);


void cleanup(struct EncryptedFile* encryptedFile) {
    free(encryptedFile->data);
}

void encryptFile(const char* filename) {
    // Load the plaintext file
    FILE* file = fopen(filename, "rb");
    if (!file) {
        perror("Error opening file");
        return;
    }

    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    rewind(file);

    // Allocate memory to store plaintext data
    unsigned char* plaintext = (unsigned char*)malloc(file_size);
    if (!plaintext) {
        perror("Error allocating memory");
        fclose(file);
        return;
    }

    fread(plaintext, 1, file_size, file);
    fclose(file);

    // XOR-based encryption
    for (size_t i = 0; i < file_size; i++) {
        plaintext[i] ^= KEY;
    }

    // Create an EncryptedFile structure
    struct EncryptedFile encryptedFile;
    snprintf(encryptedFile.filename, sizeof(encryptedFile.filename), "encrypted_%s", filename);
    encryptedFile.size = file_size;
    encryptedFile.data = plaintext;

    // Save the encrypted file
    saveEncryptedFile(encryptedFile.filename, &encryptedFile);

    // Clean up
    cleanup(&encryptedFile);

    printf("File encrypted successfully: %s\n", encryptedFile.filename);
}

void decryptFile(const char* filename) {
    // Load the encrypted file
    struct EncryptedFile encryptedFile;
    loadEncryptedFile(filename, &encryptedFile);

    // XOR-based decryption
    for (size_t i = 0; i < encryptedFile.size; i++) {
        encryptedFile.data[i] ^= KEY;
    }

    // Save the decrypted data to a new file
    FILE* decrypted_file = fopen("decrypted_diary.txt", "wb");
    if (!decrypted_file) {
        perror("Error creating decrypted file");
        cleanup(&encryptedFile);
        return;
    }

    fwrite(encryptedFile.data, 1, encryptedFile.size, decrypted_file);
    fclose(decrypted_file);

    // Clean up
    cleanup(&encryptedFile);

    printf("File decrypted successfully: decrypted_diary.txt\n");
}

void saveEncryptedFile(const char* filename, struct EncryptedFile* encryptedFile) {
    // Save the EncryptedFile structure to a binary file
    FILE* file = fopen(filename, "wb");
    if (!file) {
        perror("Error creating encrypted file");
        return;
    }

    // Save the size of the data
    fwrite(&encryptedFile->size, sizeof(size_t), 1, file);

    // Save the encrypted data
    fwrite(encryptedFile->data, 1, encryptedFile->size, file);

    fclose(file);
}

void loadEncryptedFile(const char* filename, struct EncryptedFile* encryptedFile) {
    // Load the EncryptedFile structure from a binary file
    FILE* file = fopen(filename, "rb");
    if (!file) {
        perror("Error opening encrypted file");
        exit(EXIT_FAILURE);
    }

    // Load the size of the data
    fread(&encryptedFile->size, sizeof(size_t), 1, file);

    // Allocate memory for the encrypted data
    encryptedFile->data = (unsigned char*)malloc(encryptedFile->size);
    if (!encryptedFile->data) {
        perror("Error allocating memory");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    // Load the encrypted data
    fread(encryptedFile->data, 1, encryptedFile->size, file);

    fclose(file);
}

int main()
{
    if (!authenticate())
    {
        return 1; // Exit the program if authentication fails
    }
    struct Diary myDiary;
    myDiary.count = 0;
    int choice;

    do
    {
        printf("Personal Diary Menu:\n");
        printf("1. Add Entry\n");
        printf("2. Delete Entry\n");
        printf("3. View Entries (sorted by date)\n");
        printf("4. Save Diary\n");
        printf("5. Encrypt Diary\n");
        printf("6. Decrypt Diary\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            addEntry(&myDiary);
            break;
        case 2:
            deleteEntry(&myDiary);
            break;
        case 3:
            viewEntries(&myDiary);
            break;
        case 4:
            saveDiary(&myDiary);
            break;
        case 5:
            // Encrypt a file
            encryptFile("diary.txt");
            break;
        case 6:
            // Decrypt the encrypted file
            decryptFile("encrypted_diary.txt");
            break;
        case 7:
            printf("Exiting the program.\n");
            break;
        default:
            printf("Invalid choice. Please enter a number between 1 and 7.\n");
        }
    } while (choice != 7);

    return 0;
}
