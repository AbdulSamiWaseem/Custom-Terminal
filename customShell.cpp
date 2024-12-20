#include <iostream>
#include <sys/wait.h>
#include <unistd.h> 
#include <fcntl.h> 
#include <cerrno>  
#include <cstring> 
#include <vector>
#include <sys/stat.h>

using namespace std;

const int MAX_HISTORY_SIZE = 10;
vector<string> history;

int StringLength(char* chararr) 
{   
    int count = 0;
    for (int i = 0; chararr[i] != '\0'; i++) 
    {
        count++;
    }
    return count;
}

int countingTokens(char* chararr) 
{
    int len = StringLength(chararr);
    int tokenCount = 0;
    bool flag = false;

    for (int i = 0; i < len; i++) 
    {
        if (chararr[i] == ' ') 
        {
            flag = false;
        }
        else 
        {
            if (!flag) 
            {
                tokenCount++;
                flag = true;
            }
        }
    }
    return tokenCount;
}

vector<string> StringTokensForExec(char* chararr) 
{

    int len = StringLength(chararr);
    vector<string> tokens;

    string currentToken;

    for (int i = 0; i < len; i++) {
        if (chararr[i] == ' ' || chararr[i] == '<' || chararr[i] == '>' || chararr[i] == '|' || chararr[i] == '&') 
        {
            if (!currentToken.empty()) 
            {
                tokens.push_back(currentToken);
                currentToken.clear();
            }
            if (chararr[i] != ' ') 
            {
                tokens.push_back(string(1, chararr[i]));
            }
        } 
        else 
        {
            currentToken += chararr[i];
        }
    }

    if (!currentToken.empty()) 
    {
        tokens.push_back(currentToken);
    }

    return tokens;
}


void StringTokensForDup(char* chararr, char*& in, char*& out)
{

    int len = StringLength(chararr);
    bool foundInput = false;
    bool foundOutput = false;

    for (int i = 0; i < len; i++)
    {
        if (chararr[i] == '<' && !foundInput)
        {
            foundInput = true;
            i = i+2; 
            int cap = 1;
            in = new char[cap];

            int j = 0;
            while (chararr[i] != ' ' && chararr[i] != '\0' && chararr[i] != '&')
            {
                in[j] = chararr[i];
                i++;
                j++;

                if (j >= cap)
                {
                    cap *= 2;
                    char* newarr = new char[cap];
                    for (int x = 0; x < j; x++)
                    {
                        newarr[x] = in[x];
                    }
                    delete[] in;
                    in = newarr;
                }
            }
            
            in[j] = '\0';
        }

        if (chararr[i] == '>' && !foundOutput)
        {
            foundOutput = true;
            i = i+2;
            int cap = 1;
            out = new char[cap];

            int j = 0;
            while (chararr[i] != ' ' && chararr[i] != '\0' && chararr[i] != '&')
            {
                out[j] = chararr[i];
                i++;               
                j++;

                if (j >= cap)
                {
                    cap *= 2;
                    char* newarr = new char[cap];
                    for (int x = 0; x < j; x++)
                    {
                        newarr[x] = out[x];
                    }
                    delete[] out;
                    out = newarr;
                }
            }
            out[j] = '\0';
        }
    }
}

void addToHistory(const string& command) 
{
    if (history.size() == MAX_HISTORY_SIZE) 
    {
        history.erase(history.begin());
    }
    history.push_back(command);
}

void displayHistory() 
{
    int count = 1;
    for (auto itr = history.end() - 1; itr >= history.begin(); --itr) 
    {
        cout << count++ << ": " << *itr << endl;
    }
}

string getNthCommand(int n) 
{
    if (n <= 0 || n > history.size()) 
    {
        return "";
    }
    return history[history.size() - n];
}

void executeCommand(const string& command) 
{
    addToHistory(command);
    system(command.c_str());
}


void ChangeDirectory(const char* path) {
    char* pathCopy = strdup(path);
    if (chdir(pathCopy) != 0) {
        cout << "Error changing directory to " << path << endl;
    }
    free(pathCopy); // Remember to free the allocated memory
}


int main() {
    int NumOfChildren = 0;
    char* in = NULL;
    char* out = NULL;

    while (true) {
        bool andFlag = false;
        
        
        char* chararr = new char[1024];
        cout << "Enter: ";
        cin.getline(chararr, 1024);
        string command(chararr);
        
        vector<string> tokens;
        tokens = StringTokensForExec(chararr);

        if (command == "history") {
            displayHistory();
            continue;
        }

        if (command == "exit") {
            break;
        }
        
        if (command.length() > 0 && command != "!!" && command[0] != '!')
	{
            addToHistory(command);
        }
        
         if (command == "!!") 
         {
         	int n = 1;
         	string nthCommand = getNthCommand(n);
           	if (nthCommand.empty()) 
           	{
                	cout << "No such command in history." << endl;
                	continue;
            	}
            	
            	executeCommand(nthCommand);
            	continue;
	}

        if (command[0] == '!') 
        {
            int n = stoi(command.substr(1));
            string nthCommand = getNthCommand(n);
            if (nthCommand.empty()) 
            {
                cout << "No such command in history." << endl;
                continue;
            }
            executeCommand(nthCommand);
            continue;
        }

        if (strcmp(tokens[0].c_str(), "cd") == 0) 
        {

            if (tokens.size() > 1) 
            {
                ChangeDirectory(const_cast<char*>(tokens[1].c_str()));
            } 
            else 
            {
                cout << "Usage: cd <directory>" << endl;
            }

            continue;

        }

        

        int id;
        NumOfChildren++;
        id = fork();

        if (id < 0) 
        {
            cout << "ERROR! Could not fork" << endl;
        } 
        else if (id == 0) 
        { //child
           if (in != NULL) 
           {
                int fdin = open(in, O_RDONLY);
                if (fdin < 0) 
                {
                    cout << "Error opening input file. " << endl;
                    exit(1);
                }
                dup2(fdin, STDIN_FILENO);
                close(fdin);
            }

            if (out != NULL) 
            {
                int fdout = open(out, O_WRONLY | O_CREAT | O_TRUNC, 0666);
                if (fdout < 0) 
                {
                    cout << "Error opening output file. " << endl;
                    exit(1);
                }
                dup2(fdout, STDOUT_FILENO);
                close(fdout);
            }

            execlp("sh", "sh", "-c", chararr, nullptr);
            cout << "Error executing command" << endl;
            exit(1);
        } 
        else 
        { //parent
            while (NumOfChildren > 0) 
            {
                if (!andFlag) 
                {
                    wait(NULL);
                }
                NumOfChildren--;
            }
        }
    }

    return 0;
}

