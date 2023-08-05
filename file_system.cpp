#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <sstream>
#include <stack>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include <ctime>
#include <cstdio>
#include <filesystem>
#include <unistd.h>
using namespace std;
class File
{
public:
    // char name[string_size];
    string name;

    int admin_read;
    int admin_write;
    int admin_execute;
    int other_read;
    int other_write;
    int other_execute;

    File *make_file(string *name, int admin_read, int admin_write, int admin_execute, int other_read, int other_write, int other_execute);
    // File* make_file(const char name);
};

class Directory
{
public:
    // char name[string_size];
    string name;
    Directory *parent;
    // Directory ** subdirct;
    vector<Directory *> subdirct;
    // File **files;
    vector<File *> files;
    int num_sub;
    int num_files;
    int admin_read;
    int admin_write;
    int admin_execute;
    int other_read;
    int other_write;
    int other_execute;

    Directory *make_directory(string *name, Directory *parent, int admin_read, int admin_write, int admin_execute, int other_read, int other_write, int other_execute);
    // Directory* make_directory(const char name,Directory*parent);

    void insert_directory(Directory *parent, Directory *child);

    void insert_file(Directory *parent, File *new_file);
    void display_tree(Directory *directory, int depth);
    Directory *search_dir(Directory *directory, const std::string &name);
    int search_tree(Directory *directory, string name);
    int search_tree1(Directory *directory, string name);
    // Directory* search_file(Directory* directory,const std::string& name);
};

Directory *Directory::make_directory(string *name, Directory *parent, int admin_read, int admin_write, int admin_execute, int other_read, int other_write, int other_execute)
{
    Directory *direct = new Directory();
    direct->name = *name;
    direct->num_sub = 0;
    direct->num_files = 0;
    direct->parent = parent;
    direct->name = *name;
    direct->admin_read = admin_read;
    direct->admin_write = admin_write;
    direct->admin_execute = admin_execute;
    direct->other_read = other_read;
    direct->other_write = other_write;
    direct->other_execute = other_execute;

    return direct;
}

void Directory::insert_directory(Directory *parent, Directory *child)
{
    // parent->subdirct[parent->num_sub]=child;
    parent->subdirct.push_back(child);
    parent->num_sub = parent->num_sub + 1;
}

File *File::make_file(string *name, int admin_read, int admin_write, int admin_execute, int other_read, int other_write, int other_execute)
{
    File *new_file = new File();

    new_file->name = *name;
    new_file->admin_read = admin_read;
    new_file->admin_write = admin_write;
    new_file->admin_execute = admin_execute;
    new_file->other_read = other_read;
    new_file->other_write = other_write;
    new_file->other_execute = other_execute;
    return new_file;
}

void Directory::insert_file(Directory *parent, File *new_file)
{
    parent->files.push_back(new_file);
    parent->num_files = parent->num_files + 1;
}

int Directory::search_tree(Directory *directory, string name)
{
    int file_vector_size = directory->files.size();
    // cout<<file_vector_size<<"\n";
    for (int i = 0; i < file_vector_size; i++)
    {
        //  cout<<directory->files[i]->name<<"\n";
        if (directory->files[i]->name == name)
        {

            return 1;
        }
    }

    return 0;
}

int Directory::search_tree1(Directory *directory, string name)
{
    int file_vector_size = directory->subdirct.size();
    // cout<<file_vector_size<<"\n";
    for (int i = 0; i < file_vector_size; i++)
    {
        // cout<<directory->subdirct[i]->name<<"\n";
        if (directory->subdirct[i]->name == name)
        {

            return 1;
        }
    }

    return 0;
}

Directory *Directory::search_dir(Directory *directory, const std::string &name)
{
    std::stack<Directory *> stack;
    stack.push(directory);

    while (!stack.empty())
    {
        Directory *currentDir = stack.top();
        stack.pop();

        if (currentDir->name == name)
        {
            return currentDir;
        }

        for (Directory *subdirectory : currentDir->subdirct)
        {
            stack.push(subdirectory);
        }
    }

    return nullptr;
}

void Directory::display_tree(Directory *directory, int depth)
{
    for (int i = 0; i < depth; i++)
    {
        cout << "  ";
    }

    cout << "|-- DIRECTORY ---> " << directory->name << "   PERMISSIONS: " << directory->admin_read << " " << directory->admin_write << " " << directory->admin_execute << " " << directory->other_read << " " << directory->other_write << " " << directory->other_execute << "\n"
         << "\n";

    for (File *file : directory->files)
    {
        for (int i = 0; i < depth + 1; i++)
        {
            cout << "  ";
        }
        cout << "|-- FILE ---> " << file->name << "   PERMISSIONS: " << file->admin_read << " " << file->admin_write << " " << file->admin_execute << " " << file->other_read << " " << file->other_write << " " << file->other_execute << "\n"
             << "\n";
    }

    for (Directory *subdirectory : directory->subdirct)
    {
        display_tree(subdirectory, depth + 1);
    }
}

class files
{
public:
    string name;
    string file_content;
    string timestamp;
    size_t length;
    int admin_read;
    int admin_write;

    files(string file_name) : name(file_name){};
    friend void read_content(files &obj);
    friend void timeStamp(files &obj);
    friend void reader(files &obj);
    friend void read_fragmented_content(files &obj, size_t size, size_t offset_file);
};

struct MemoryBlock
{
    size_t size;
    bool allocated;
    files *file_ptr[5];

    MemoryBlock *next; // Pointer to the next memory block in the linked list.
    MemoryBlock() : size(0), allocated(false), next(nullptr)
    {
        // Set all elements of the 'file_ptr' array to nullptr
        for (int i = 0; i < 5; ++i)
        {
            file_ptr[i] = nullptr;
        }
    }
};
size_t file_size(string filename)
{
    size_t fileSize_int = 0;
    ifstream file(filename, ios::binary | ios::ate);
    if (file.is_open())
    {
        // Get the file size in bytes.
        streampos fileSize = file.tellg();
        fileSize_int = static_cast<int>(fileSize);
        // cout << "File size: " << fileSize_int << " bytes" << endl;
    }
    else
    {
        cout << "Error: Unable to open the file." << endl;
    }
    return fileSize_int;
}
void read_fragmented_content(files &obj1, size_t size, size_t offset_file)
{
    std::string file_name = obj1.name;
    std::ifstream file(file_name, std::ios::binary);
    string fileContent;
    // Specify the number of bytes to read.
    const int bytes_to_read = size;

    // Create a buffer to store the read data.
    char buffer[bytes_to_read];
    const int offset = offset_file;
    file.seekg(offset, std::ios::beg);
    // Read the specified number of bytes from the file into the buffer.
    file.read(buffer, bytes_to_read);
    fileContent = std::string(buffer, file.gcount());
    obj1.file_content = fileContent;

    // file.close();
}
struct file_info
{
    int first;
    int second;
    string name;
    string timestamp;
};
class MemoryManager
{
private:
    MemoryBlock *head; // Pointer to the head of the linked list.
    MemoryBlock *tail;

public:
    MemoryManager() : head(NULL), tail(NULL) {}
    // Function to create a new memory block with a specified size and add it to the linked list.
    void createBlock(size_t size)
    {
        // MemoryBlock* newBlock = new MemoryBlock;
        MemoryBlock *newBlock = (MemoryBlock *)malloc(size);
        cout << "--" << newBlock << " with size " << size << "--" << endl;
        newBlock->size = size;

        if (!head)
        {
            // If the list is empty, make the new node both the head and tail
            head = tail = newBlock;
        }
        else
        {
            // If the list is not empty, append the new node at the end and update the tail
            tail->next = newBlock;
            tail = tail->next;
        }
    }
    void file_fragment(size_t size, string file_name, int read, int write)
    {
        MemoryBlock *currentBlock = head;
        size_t balance_size;
        size_t offset = 0;
        while (currentBlock != nullptr && size != 0)
        {
            if (currentBlock->size < size)
            {

                balance_size = size - currentBlock->size;
                size = balance_size;
                // currentBlock->size = 0;
                currentBlock->allocated = true;
                int j = 0;
                for (int i = 0; i < 5; i++)
                {
                    if (currentBlock->file_ptr[i] == NULL)
                    {
                        j = i;
                        break;
                    }
                }
                files *obj1 = new files(file_name);
                obj1->length = currentBlock->size;
                read_fragmented_content(*obj1, currentBlock->size, offset);
                obj1->admin_read = read;
                obj1->admin_write = write;
                offset = offset + currentBlock->size;
                currentBlock->size = 0;

                timeStamp(*obj1);
                currentBlock->file_ptr[j] = obj1;
                currentBlock = currentBlock->next;
            }

            else
            {

                balance_size = currentBlock->size - size;
                // size = 0;
                currentBlock->size = balance_size;
                currentBlock->allocated = false;
                int j = 0;
                for (int i = 0; i < 5; i++)
                {
                    if (currentBlock->file_ptr[i] == NULL)
                    {
                        j = i;
                        break;
                    }
                }

                files *obj1 = new files(file_name);
                obj1->length = size;
                obj1->admin_read = read;
                obj1->admin_write = write;
                read_fragmented_content(*obj1, size, offset);

                timeStamp(*obj1);

                currentBlock->file_ptr[j] = obj1;
                size = 0;
            }
        }
    }
    // Function to allocate memory of a specified size and return a pointer to the allocated memory.
    void allocate(size_t size, string file_name, int read, int write)
    {

        size_t totalSize = size;

        MemoryBlock *bestFitBlock = nullptr;
        MemoryBlock *currentBlock = head;
        int temp = 0;

        while (currentBlock != nullptr)
        {

            if (currentBlock->size >= totalSize && !currentBlock->allocated &&
                (bestFitBlock == nullptr || currentBlock->size < bestFitBlock->size))
            {
                bestFitBlock = currentBlock;
                temp = 1;
            }
            currentBlock = currentBlock->next;
        }

        if (!temp)
        {
            file_fragment(size, file_name, read, write);
            return;
        }
        size_t available_size = bestFitBlock->size - size;

        if (bestFitBlock != nullptr && available_size == 0)
        {
            bestFitBlock->allocated = true;

            int j = 0;
            for (int i = 0; i < 5; i++)
            {
                if (bestFitBlock->file_ptr[i] == NULL)
                {
                    j = i;
                    break;
                }
            }
            cout << "Block at address: " << bestFitBlock << " with size " << bestFitBlock->size << " will point to the new file" << endl;
            cout << "Available size of the block: " << available_size << endl
                 << endl;
            bestFitBlock->size = bestFitBlock->size - size;
            files *obj1 = new files(file_name);
            obj1->length = size;
            obj1->admin_read = read;
            obj1->admin_write = write;
            read_content(*obj1);
            timeStamp(*obj1);
            bestFitBlock->file_ptr[j] = obj1;
        }
        else if (available_size > 0 && available_size != bestFitBlock->size)
        {
            bestFitBlock->allocated = false;
            int j = 0;
            for (int i = 0; i < 5; i++)
            {
                if (bestFitBlock->file_ptr[i] == NULL)
                {
                    j = i;
                    break;
                }
            }
            cout << "Block at address: " << bestFitBlock << " with size " << bestFitBlock->size << " will point to the new file" << endl;
            cout << "Available size of the block: " << available_size << endl;
            bestFitBlock->size = bestFitBlock->size - size;
            files *obj1 = new files(file_name);
            obj1->length = size;
            obj1->admin_read = read;
            obj1->admin_write = write;
            read_content(*obj1);
            timeStamp(*obj1);
            bestFitBlock->file_ptr[j] = obj1;
        }
        else
            cout << "no best block available";
        // return;
        // return 0; // No best fit block found, return nullptr.
    }
    void display()
    {
        MemoryBlock *ptr = head;
        int temp = 0;

        while (ptr)
        {
            cout << "----------------------------------------------------------------------" << endl;
            for (int i = 0; i < 5; i++)
            {

                if (ptr->file_ptr[i] != NULL)
                {
                    temp = 1;
                    cout << endl
                         << "Name of file in the block: " << ptr->file_ptr[i]->name << "  at BLOCK ADDRESS:  " << ptr << endl;
                    cout << endl
                         << "file content: " << endl
                         << ptr->file_ptr[i]->file_content << endl;
                }
            }

            if (!temp)
            {
                cout << endl
                     << "No file in the block" << endl;
            }

            cout << " sizeof available block:  " << ptr->size << endl; // Print the data of the current node
            ptr = ptr->next;                                           // Move to the next node
            cout << "----------------------------------------------------------------------" << endl;
        }
        cout << endl;
    }
    void read_operation(string filename)
    {
        MemoryBlock *ptr = head;

        while (ptr)
        {

            for (int i = 0; i < 5; i++)
            {

                if (ptr->file_ptr[i] != NULL)
                {
                    if (ptr->file_ptr[i]->name == filename && ptr->file_ptr[i]->admin_read == 1)
                    {

                        cout << endl
                             << "file content: " << endl
                             << ptr->file_ptr[i]->file_content << endl;
                    }
                }
            }
            ptr = ptr->next;
        }
        cout << endl;
    }
    void write_operation(string filename, string content)
    {
        MemoryBlock *ptr = head;
        int temp = 0;

        while (ptr)
        {

            for (int i = 0; i < 5; i++)
            {

                if (ptr->file_ptr[i] != NULL)
                {
                    if (ptr->file_ptr[i]->name == filename)
                    {
                        cout << "the file exists in the memory" << endl;
                        temp = 1;

                        delete_file(filename);
                    }
                }
            }
            ptr = ptr->next;
        }

        cout << endl;
    }
    file_info get_file_details(string filename)
    {
        MemoryBlock *ptr = head;
        file_info result;
        while (ptr)
        {

            for (int i = 0; i < 5; i++)
            {

                if (ptr->file_ptr[i] != NULL)
                {
                    if (ptr->file_ptr[i]->name == filename)
                    {
                        result.first = ptr->file_ptr[i]->admin_read;
                        result.second = ptr->file_ptr[i]->admin_write;
                        result.name = ptr->file_ptr[i]->name;
                        result.timestamp = ptr->file_ptr[i]->timestamp;
                    }
                }
            }
            ptr = ptr->next;
        }

        cout << endl;
        return result;
    }

    void delete_file(string filename)
    {
        MemoryBlock *ptr = head;

        while (ptr)
        {

            for (int i = 0; i < 5; i++)
            {

                if (ptr->file_ptr[i] != NULL && ptr->file_ptr[i]->name == filename)
                {
                    cout << endl
                         << "file name : " << ptr->file_ptr[i]->name << " will be deleted" << endl;
                    // ptr->file_ptr[i]=NULL;
                    // delete ptr->file_ptr[i];
                    ptr->size = ptr->size + ptr->file_ptr[i]->length;
                    ptr->file_ptr[i] = NULL;
                }
            }
            ptr = ptr->next;
        }
        cout << endl;
    }
};

void read_content(files &obj)
{
    string fileContent;

    ifstream inputFile(obj.name);
    if (inputFile.is_open())
    {
        // Read the file content and store it in the string variable.
        fileContent = std::string(std::istreambuf_iterator<char>(inputFile), std::istreambuf_iterator<char>());

        // Close the file after reading.
        inputFile.close();
    }
    else
    {
        std::cerr << "Error: Unable to open the file." << std::endl;
    }
    obj.file_content = fileContent;
}

void timeStamp(files &obj)
{
    struct stat fileInfo;
    string timeString;
    if (stat(obj.name.c_str(), &fileInfo) == 0)
    {
        time_t timestamp = fileInfo.st_mtime;
        timeString = ctime(&timestamp);
    }
    obj.timestamp = timeString;
}

void reader(files &obj)
{
    cout << obj.timestamp << endl;
}

bool fileExists(const std::string &filename)
{
    std::ifstream file(filename);
    return file.good();
}

int main()
{
    size_t var;

    MemoryManager memoryManager;

    // Create memory blocks of different sizes.
    memoryManager.createBlock(600);
    memoryManager.createBlock(1000);
    memoryManager.createBlock(700);
    memoryManager.createBlock(800);
    memoryManager.createBlock(500);
    memoryManager.display();

    int ch;
    Directory d1;
    File f1;
    string user_name;
    cout << "ENTER YOUR NAME"
         << "\n"
         << "\n";
    cin >> user_name;

    Directory *admin = d1.make_directory(new string(user_name), NULL, 1, 1, 1, 1, 0, 0);
    cout << "YOUR IN DIRECTORY ---> " << admin->name << "\n"
         << "\n";
    do
    {
        cout << "CHOOSE FROM THE FOLLOWING"
             << "\n"
             << "\n";
        cout << "->ENTER 1.CREATE FILE" << endl
             << endl
             << "->ENTER 2.CREATE DIRECTORY" << endl
             << endl
             << "->ENTER 3.DISPLAY" << endl
             << endl
             << "->ENTER 4.DISPLAY_MEMORY" << endl
             << endl
             << "->ENTER 5.EXIT" << endl
             << endl
             << "\n";
        cin >> ch;
        switch (ch)
        {
        case 1:
        {
            string fpath;
            cout << "ENTER THE FILE PATH"
                 << "\n"
                 << "\n";
            cin >> fpath;
            vector<string> actual_path;
            std::size_t start = 0, end;
            while ((end = fpath.find('\\', start)) != std::string::npos)
            {
                if (end > start)
                {
                    actual_path.push_back(fpath.substr(start, end - start));
                }
                start = end + 1;
            }

            if (start < fpath.length())
            {
                actual_path.push_back(fpath.substr(start));
            }
            int n = actual_path.size();
            std::ofstream outputFile(actual_path[n - 1], std::ios::binary);

            if (outputFile.is_open())
            {
                // Step 2: File opened successfully

                // Step 3: Write a one-byte char into the file
                char byteToWrite = ' ';
                outputFile.write(&byteToWrite, 1);

                // Step 4: Close the file
                outputFile.close();
            }
            outputFile.close();
            size_t var;
            var = file_size(actual_path[n - 1]);

            // memoryManager.allocate(var, actual_path[n-1]);

            Directory *a = d1.search_dir(admin, actual_path[n - 2]);
            if (a != nullptr)
            {
                int b = d1.search_tree(a, actual_path[n - 1]);
                if (b == 0)
                {
                    cout << "GIVE PERMISSIONS FOR THE FILE"
                         << "\n"
                         << "\n";
                    int admin_read;
                    int admin_write;
                    int admin_execute;
                    int other_read;
                    int other_write;
                    int other_execute;
                    cout << "admin_read"
                         << "\n";
                    cin >> admin_read;
                    cout << "admin_write"
                         << "\n";
                    cin >> admin_write;
                    cout << "admin_execute"
                         << "\n";
                    cin >> admin_execute;
                    cout << "other_read"
                         << "\n";
                    cin >> other_read;
                    cout << "other_write"
                         << "\n";
                    cin >> other_write;
                    cout << "other_execute"
                         << "\n";
                    cin >> other_execute;
                    memoryManager.allocate(var, actual_path[n - 1], admin_read, admin_write);
                    File *new_file = f1.make_file(new string(actual_path[n - 1]), admin_read, admin_write, admin_execute, other_read, other_write, other_execute);

                    d1.insert_file(a, new_file);
                }
                else
                {
                    cout << "******FILE ALREADY EXITS******"
                         << "\n"
                         << "\n";
                    cout << "******PLEASE CHANGE THE FILE NAME******"
                         << "\n"
                         << "\n";
                }
            }
            else
            {
                cout << "******DIRECTORY"
                     << " " << actual_path[n - 2] << " "
                     << "DOES NOT EXITS******"
                     << "\n"
                     << "\n";
            }

            break;
        }
        case 2:
        {
            string dpath;
            cout << "ENTER THE DIRECTORY PATH"
                 << "\n"
                 << "\n";
            cin >> dpath;
            vector<string> actual_path;
            std::size_t start = 0, end;
            while ((end = dpath.find('\\', start)) != std::string::npos)
            {
                if (end > start)
                {
                    actual_path.push_back(dpath.substr(start, end - start));
                }
                start = end + 1;
            }

            if (start < dpath.length())
            {
                actual_path.push_back(dpath.substr(start));
            }
            int n = actual_path.size();
            Directory *a = d1.search_dir(admin, actual_path[n - 2]);
            if (a != nullptr)
            {
                int b1 = d1.search_tree1(a, actual_path[n - 1]);
                if (b1 == 0)
                {
                    cout << "GIVE PERMISSIONS FOR THE DIRECTORY"
                         << "\n"
                         << "\n";
                    int admin_read;
                    int admin_write;
                    int admin_execute;
                    int other_read;
                    int other_write;
                    int other_execute;
                    cout << "admin_read"
                         << "\n";
                    cin >> admin_read;
                    cout << "admin_write"
                         << "\n";
                    cin >> admin_write;
                    cout << "admin_execute"
                         << "\n";
                    cin >> admin_execute;
                    cout << "other_read"
                         << "\n";
                    cin >> other_read;
                    cout << "other_write"
                         << "\n";
                    cin >> other_write;
                    cout << "other_execute"
                         << "\n";
                    cin >> other_execute;

                    Directory *new_directory = d1.make_directory(new string(actual_path[n - 1]), a, admin_read, admin_write, admin_execute, other_read, other_write, other_execute);
                    d1.insert_directory(a, new_directory);
                }
                else
                {
                    cout << "******DIRECTORY ALREADY EXITS******"
                         << "\n"
                         << "\n";
                    cout << "******PLEASE CHANGE THE DIRECTORY NAME******"
                         << "\n"
                         << "\n";
                }
            }
            else
            {
                cout << "******DIRECTORY"
                     << " " << actual_path[n - 2] << " "
                     << "DOES NOT EXITS******"
                     << "\n"
                     << "\n";
            }

            break;
        }

        case 3:
        {
            d1.display_tree(admin, 0);
            cout << "\n";
            break;
        }
        case 4:
        {
            string file_name;
            int choice = 0;
            string s;
            while (choice != 6)
            {
                cout << "->ENTER 1 FOR READING A FILE" << endl
                     << endl;
                cout << "->ENTER 2 FOR WRITING A FILE" << endl
                     << endl;
                cout << "->ENTER 3 FOR APPENDING  A FILE" << endl
                     << endl;
                cout << "->ENTER 4 FOR DELETING A FILE" << endl
                     << endl;
                cout << "->ENTER 5 FOR VIEWING MEMORY BLOCK A FILE" << endl
                     << endl;
                cout << "->ENTER 6 TO EXIT" << endl
                     << endl;
                cin >> choice;
                if (choice == 1)
                {
                    cout << "Enter a file name to read: " << endl;
                    cin >> file_name;
                    file_info obj = memoryManager.get_file_details(file_name);
                    if (!obj.first)
                        cout << "YOU DONT HAVE READ PERMISSION\n"
                             << endl;

                    else if (fileExists(file_name))
                    {
                        memoryManager.read_operation(file_name);
                        // memoryManager.display();
                    }
                    else
                    {
                        cout << "File doesnot exist" << endl;
                    }
                }
                else if (choice == 2)
                {
                    cout << "Enter a file name to write: " << endl;
                    cin >> file_name;
                    file_info obj = memoryManager.get_file_details(file_name);
                    if (!obj.second)
                        cout << "YOU DONT HAVE WRITE PERMISSION" << endl
                             << endl;
                    else
                    {
                        cout << "Write the content of the file {Press x to end the content of the file}" << endl;
                        getline(std::cin, s, 'x');
                        memoryManager.write_operation(file_name, s);
                        ofstream MyFile(file_name);
                        MyFile << s;
                        MyFile.close();

                        var = file_size(file_name);
                        memoryManager.allocate(var, file_name, obj.first, obj.second);
                        // memoryManager.display();
                    }
                }
                else if (choice == 3)
                {

                    cout << "Enter a file name to append: " << endl;
                    cin >> file_name;
                    file_info obj = memoryManager.get_file_details(file_name);
                    if (!obj.second)
                        cout << "YOU DONT HAVE WRITE PERMISSION" << endl
                             << endl;
                    else
                    {
                        cout << "Write the content of the file {Press x to end the content of the file}" << endl;
                        getline(std::cin, s, 'x');
                        memoryManager.write_operation(file_name, s);
                        ofstream outputFile(file_name, std::ios::app);
                        outputFile << s;
                        outputFile.close();
                        var = file_size(file_name);
                        memoryManager.allocate(var, file_name, obj.first, obj.second);
                        // memoryManager.display();
                    }
                }
                else if (choice == 4)
                {
                    cout << "Enter a file name to delete: " << endl;
                    cin >> file_name;
                    memoryManager.delete_file(file_name);
                    unlink(file_name.c_str());
                    // memoryManager.display();
                }
                else if (choice == 5)
                    memoryManager.display();
                else
                {

                    cout << "Choice other than 1,2,3,4";
                }
            }

            break;
        }
        }
    } while (ch != 5);
}
