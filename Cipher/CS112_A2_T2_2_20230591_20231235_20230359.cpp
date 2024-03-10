//File: CS112_A2_T2_2_20230591_20231235_20230359.cpp
// Section 1/2
// Yousef Osama Mamdouh Ahmed - 20230591 - Route Cipher
// Sara Shaban Ahmed Ali      - 20231235 - Simple Substitution Cipher
// Mohamed Hisham Gaber Fahmy - 20230359 - Rail-Fence Cipher


#include "bits/stdc++.h"

using namespace std;

struct routeCipher {

    void init() {
        while (true) {
            try {
                int choice;
                cout << "What do you want to do?\n";
                cout << "1. Cipher\n2. Decipher\n";
                if (!(cin >> choice) || choice < 1 || choice > 2) {
                    throw exception();
                }

                string msg;
                cout << "Enter your message:";
                cin.ignore(INT_MAX, '\n');
                getline(cin, msg);
                int key;
                cout << "Enter the key:";
                if (!(cin >> key)) {
                    throw exception();
                }
                if (choice == 1) {
                    cout << cipher(msg, key) << "\n";
                    break;
                } else {
                    if (msg.size() % key) {
                        cout << "the message length should be divisible by the key\n";
                        throw exception();
                    }
                    cout << decipher(msg, key) << "\n";
                    break;
                }
            } catch (...) {
                cin.clear();
                cin.ignore(INT_MAX, '\n');
                cout << "Invalid input.\n";
            }
        }
    }

    string trim(string msg) {
        string result;
        for (char c: msg) {
            if (isalpha(c)) result += toupper(c);
        }
        return result;
    }

    vector<vector<int>> getRoute(int height, int width) {
        // arrays with dimensions[height][width]
        vector<vector<int>> result(height, vector<int>(width));
        vector<vector<bool>> isTaken(height, vector<bool>(width, false));
        // directions in order [down, left, up, right]
        int dx[] = {1, 0, -1, 0};
        int dy[] = {0, -1, 0, 1};
        // pointer on the current direction
        int dir = 0;
        // pointer on the current cell
        int x = -1, y = width - 1;
        // count the number of cells passed
        int cnt = 0;
        while (cnt < height * width) {
            // new cell according to the current direction
            int nx = x + dx[dir];
            int ny = y + dy[dir];
            // if the new cell is not valid or taken, change to the next direction
            if (nx < 0 || ny < 0 || nx >= height || ny >= width || isTaken[nx][ny]) {
                dir = (dir + 1) % 4;
                continue;
            }
            // mark the new cell as taken
            isTaken[nx][ny] = true;
            // mark the order of the new cell as cnt
            result[nx][ny] = cnt++;
            // go to the new cell
            x = nx;
            y = ny;
        }
        return result;
    }

    string cipher(string msg, int key) {
        //remove spaces and symbols and change to uppercase
        msg = trim(msg);
        string result;
        // add special character in case the characters are not divisible by the key
        while (msg.size() % key) msg += 'X';
        int width = key;
        int height = msg.size() / width;
        // get order of each cell in the route
        auto route = getRoute(height, width);
        //store the position of a given order
        pair<int, int> pos[height * width];
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                pos[route[i][j]] = {i, j};
            }
        }
        //add the position of characters with order from 0 to height*width
        for (int i = 0; i < height * width; i++) {
            result += msg[pos[i].first * width + pos[i].second];
        }

        return result;
    }

    string decipher(string msg, int key) {
        string result;
        int width = key;
        int height = msg.size() / width;
        // get order of each cell in the route
        auto route = getRoute(height, width);
        // iterate over the cells row by row and column by column
        // and add the character in the message with the index equal to the order of this cell
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                result += msg[route[i][j]];
            }
        }
        return result;
    }
};

struct simpleSubstitution {
    vector<char> encryption_code;
    char alphabet[26];


    void init() {
        try {
            string unique_letters;
            cout << "Enter the 5 key letters:";
            cin >> unique_letters;
            unique_letters = unique_letters.substr(0, 5); // Take only the first 5 characters

            // Populate the alphabet array
            for (char c = 'a'; c <= 'z'; ++c) {
                alphabet[c - 'a'] = c;
            }

            // Populate the encryption code array
            for (char c: unique_letters) {
                encryption_code.push_back(c);
            }

            // Add the rest of the alphabet to the encryption code, avoiding repetition of the first 5 letters
            for (char c: alphabet) {
                if (unique_letters.find(c) == string::npos) { // Check if the letter is not in the first 5 letters
                    encryption_code.push_back(c);
                }
            }

            int choice;
            cout << "What do you want to do?\n";
            cout << "1. Cipher\n2. Decipher\n";
            if (!(cin >> choice) || choice < 1 || choice > 2) {
                throw exception();
            }

            string message;
            cout << "Enter a message:";
            cin.ignore(); // Ignore previous newline character
            getline(cin, message);
            if (choice == 1) {
                string encrypted_message = encrypt(message);
                cout << "Encrypted Message:" << encrypted_message << endl;
            } else {
                string decrypted_message = decrypt(message);
                cout << "Decrypted Message:" << decrypted_message << endl;
            }
        } catch (...) {
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            cout << "Invalid input.\n";
        }
    }

    // Encrypt function
    string encrypt(const string &message) {
        string encrypted_message;

        for (char c: message) {
            if (isalpha(c)) {
                c = tolower(c); // Convert to lowercase
                char encrypted_char = encryption_code[c - 'a'];
                encrypted_message += encrypted_char;
            } else {
                encrypted_message += c; // Append any non-letter character as it is
            }
        }

        return encrypted_message;
    }

// Decrypt function
    string decrypt(const string &encrypted_message) {
        string decrypted_message;

        // Decrypt each character in the encrypted message
        for (char c: encrypted_message) {
            if (isalpha(c)) {
                char decrypted_char = ' '; // Initialize decrypted character
                for (int i = 0; i < 26; ++i) {
                    if (tolower(encryption_code[i]) == tolower(c)) {
                        decrypted_char = alphabet[i]; // Found corresponding decrypted character
                        break;
                    }
                }
                // Append decrypted character preserving original case
                if (islower(c))
                    decrypted_message += decrypted_char;
                else
                    decrypted_message += toupper(decrypted_char);
            } else {
                decrypted_message += c; // Preserve non-letter characters as they are
            }
        }

        return decrypted_message;
    }
};

struct railfenceCipher {

    void init() {
        try {
            int choice;
            cout << "What do you want to do?\n";
            cout << "1. Cipher\n2. Decipher\n";
            if (!(cin >> choice) || choice < 1 || choice > 2) {
                throw exception();
            }
            string text, tmp;
            int depth;

            cout << "Please enter the text:";
            cin.ignore();
            getline(cin, tmp);
            for (char c: tmp) {
                if (isalpha(c))
                    text += tolower(c);
            }

            cout << "Please enter the depth or rows. Notice: depth should be 2 or 3 only:";
            cin >> depth;
            if (choice == 1) {
                cout << cipher(text, depth) << '\n';
            } else {
                cout << decipher(text, depth) << '\n';
            }
        } catch (...) {
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            cout << "Invalid input.\n";
        }
    }

    string cipher(string text, int depth) {
        if (depth == 2) {
            string result;
            for (int i = 0; i < text.length(); i += 2)
                result += text[i];
            for (int i = 1; i < text.length(); i += 2)
                result += text[i];
            return result;
        } else if (depth == 3) {
            string result;
            for (int i = 0; i < text.length(); i += 4)
                result += text[i];
            for (int i = 1; i < text.length(); i += 2)
                result += text[i];
            for (int i = 2; i < text.length(); i += 4)
                result += text[i];
            return result;
        } else {
            return "Number of rails not supported";
        }
    }

    string decipher(string ciphertext, int rails) {
        vector<vector<char>> fence(rails, vector<char>(ciphertext.length(), '\n'));
        int direction = -1;
        int row = 0, col = 0;

        for (char &ch: ciphertext) {
            if (row == 0 || row == rails - 1) {
                direction *= -1;
            }

            fence[row][col] = '*';
            col += 1;
            row += direction;
        }

        int index = 0;
        string plaintext;

        for (int r = 0; r < rails; ++r) {
            for (int j = 0; j < ciphertext.length(); ++j) {
                if (fence[r][j] == '*' && index < ciphertext.length()) {
                    fence[r][j] = ciphertext[index];
                    index += 1;
                }
            }
        }

        row = 0;
        col = 0;
        direction = -1;

        for (char &ch: ciphertext) {
            if (row == 0 || row == rails - 1) {
                direction *= -1;
            }

            plaintext += fence[row][col];
            col += 1;
            row += direction;
        }

        return plaintext;
    }
};

int main() {
    while (true) {
        try {
            int choice;
            cout << "What cipher do you want?\n";
            cout << "1. Route Cipher\n2. Simple Substitution\n3. Rail-fence Cipher\n4. Exit\n";
            if (!(cin >> choice) || choice < 1 || choice > 4) {
                throw exception();
            }

            if (choice == 1) {
                routeCipher cipher;
                cipher.init();
            } else if (choice == 2) {
                simpleSubstitution cipher;
                cipher.init();
            } else if (choice == 3) {
                railfenceCipher cipher;
                cipher.init();
            } else {
                break;
            }
        } catch (...) {
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            cout << "Invalid input.\n";
        }
    }
}
