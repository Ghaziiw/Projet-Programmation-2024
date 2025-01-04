#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;

class Fichier {
    char* name;
    char* mode;
    FILE* file;
    int size_len;

public:
    Fichier(){
        name = nullptr;
        mode = nullptr;
        file = nullptr;
        size_len = 0;
    }

    Fichier(char* _name, char* _mode, int _size_len) {
        name = new char[strlen(_name) + 1];
        mode = new char[strlen(_mode) + 1];

        strcpy(name, _name);
        strcpy(mode, _mode);
        size_len = _size_len;

        file = fopen(name, mode);

        if (!file) {
            perror("Error opening file");
            return;
        }
    }

    ~Fichier(){
        delete[] name;
        delete[] mode;
        fclose(file);
    }

    // Getter for name
    char* getName() const {
        return name;
    }

    // Getter for mode
    char* getMode() const {
        return mode;
    }

    // Getter for size_len
    int getSizeLen() const {
        return size_len;
    }

    // Setter for name
    void setName(const char* _name) {
        delete[] name;
        name = new char[strlen(_name) + 1];
        strcpy(name, _name);

        if (file) {
            fclose(file);
        }

        file = fopen(name, mode);
        if (!file) {
            perror("Error opening file");
        }
    }

    // Setter for mode
    void setMode(const char* _mode) {
        delete[] name;
        name = new char[strlen(_mode) + 1];
        strcpy(mode, _mode);

        if (file) {
            fclose(file);
        }

        file = fopen(name, mode);
        if (!file) {
            perror("Error opening file");
        }
    }

    // Setter for size_len
    void setSizeLen(int _size_len) {
        size_len = _size_len;
    }

    void writeFile(char* str) {
        // Check mode validity
        if (strcmp(mode, "w") != 0 && strcmp(mode, "w+") != 0) {
            perror("The mode must be 'w' or 'w+'");
            return;
        }

        // Ensure file is open
        if (!file) {
            perror("File is not open");
            return;
        }

        // Write to file
        if (fprintf(file, "%-*.*s", size_len, size_len, str) < 0) {
            perror("Error writing to file");
        }
        fprintf(file,"%-*.*s",size_len,size_len,str);
    }

    void readFile(char* buffer, int bufferSize) {
        // Check mode validity
        if (strcmp(mode, "r") != 0 && strcmp(mode, "r+") != 0 && strcmp(mode, "w+") != 0) {
            perror("The mode must allow reading (r, r+, or w+)");
            return;
        }

        // Ensure file is open
        if (!file) {
            perror("File is not open for reading");
            return;
        }

        // Read from file
        if (fscanf(file, "%s", buffer) == EOF) {
            if (ferror(file)) {
                perror("Error reading from file");
            } else {
                cout << "End of file reached." << endl;
            }
        }
    }

    void createInFile() {
        char* str = new char[size_len + 1];
        while (gets(str) && *str) {
            writeFile(str);
        }
    }

    void viewFile() {
        if (file) {
            fclose(file);
        }

        file = fopen(name, "r");
        if (!file) {
            perror("File is not open");
        }
        char buffer[size_len];
        while (fgets(buffer, size_len, file) != nullptr) {
            cout << buffer << endl;
        }
        fclose(file);

        file = fopen(name, mode);
        if (!file) {
            perror("File is not open");
        }
    }

    Fichier operator+ (Fichier& f1) {
        int max = (this->size_len > f1.size_len) ? this->size_len : f1.size_len;
        char *new_name= new char[strlen(this->name)+strlen(f1.name)+1];
        cin>>new_name;
        Fichier f0(new_name, "a+", max);
        char buffer[max];
        fclose(file);
        file = fopen(name, "r");
        if (!file) {
            perror("File is not open");
        }
        f1.file= fopen(f1.name,"r");
        if (!f1.file) {
            perror("File is not open");
        }
        //copying the content of the first file to the new file
        while (fgets(buffer, max, file) != nullptr) {
            f0.writeFile(buffer);
        }
        //copying the content of the second file to the new file
        while (fgets(buffer, max, f1.file) != nullptr) {
            f0.writeFile(buffer);
        }
        //returning first and second file to their original state
        fclose(file);
        fclose(f1.file);
        file= fopen(name, mode);
        f1.file= fopen(f1.name, f1.mode);
        //setting the mode of the new file to write
        f0.setMode("w");
        return f0;
    }
};

int Main()
{
    Fichier ville1 ("Ville.txt","w",15);
    Fichier ville2("new.txt","w",25);
    Fichier f3;
    ville1.createInFile();
    ville2.createInFile();
    f3=ville1+ville2;
    f3.viewFile();
}
