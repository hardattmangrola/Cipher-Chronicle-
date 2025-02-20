# Cipher Chronicle 📒

## Overview
Cipher Chronicle is a secure personal diary application written in C. It allows users to create, view, delete, and save diary entries with an additional layer of security through XOR-based encryption. The application ensures access control with password authentication and provides options for encrypted storage of diary entries.

## Features
- **Password Authentication**: Ensures only authorized users can access the diary.
- **Add Entries**: Users can add diary entries with a date and content.
- **View Entries**: Displays diary entries sorted by date.
- **Delete Entries**: Removes specific diary entries.
- **Save Entries**: Saves diary entries to a text file.
- **Encrypt Diary**: Uses XOR-based encryption to protect diary contents.
- **Decrypt Diary**: Recovers encrypted entries for viewing.

## How It Works
1. **Authentication**: The user must enter the correct password (`admin` by default) to access the diary.
2. **Menu Options**:
   - Add an entry with a date and content.
   - View all entries sorted by date.
   - Delete an entry by specifying its number.
   - Save the diary to a file (`diary.txt`).
   - Encrypt the saved diary file (`diary.txt` → `encrypted_diary.txt`).
   - Decrypt the file (`encrypted_diary.txt` → `decrypted_diary.txt`).
3. **Encryption & Decryption**: Uses XOR encryption with a predefined key (`0xAA`).
4. **Exit**: Users can exit the program safely.

## Usage
### Compilation
Compile the program using GCC:
```sh
gcc cipher_chronicle.c -o cipher_chronicle
```

### Running the Program
```sh
./cipher_chronicle
```

### Encrypting the Diary
The program provides an option to encrypt the saved `diary.txt` file using XOR encryption.

### Decrypting the Diary
To view encrypted entries, the program decrypts `encrypted_diary.txt` back to `decrypted_diary.txt`.

## File Structure
- `cipher_chronicle.c` - Main source code.
- `diary.txt` - Stores saved diary entries (before encryption).
- `encrypted_diary.txt` - Stores encrypted diary entries.
- `decrypted_diary.txt` - Stores decrypted diary entries.

## Future Enhancements
- Implement a stronger encryption algorithm.
- Provide a user-defined encryption key.
- Implement a graphical user interface (GUI).
- Add cloud storage integration for backups.

## License
This project is open-source and available under the MIT License.

