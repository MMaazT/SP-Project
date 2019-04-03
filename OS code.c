#include<stdio.h>
#include <string.h>
#include<Windows.h>
#include<process.h>
#include<winbase.h>
#include <Tlhelp32.h>


void killProcessByName(const char *filename)
{
    HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
    PROCESSENTRY32 pEntry;
    pEntry.dwSize = sizeof (pEntry);
    BOOL hRes = Process32First(hSnapShot, &pEntry);
    while (hRes)
    {
        if (strcmp(pEntry.szExeFile, filename) == 0)
        {
            HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0,
                                          (DWORD) pEntry.th32ProcessID);
            if (hProcess != NULL)
            {
                TerminateProcess(hProcess, 9);
                CloseHandle(hProcess);
            }
        }
        hRes = Process32Next(hSnapShot, &pEntry);
    }
    CloseHandle(hSnapShot);
}

int main () {

	// declare and initialize string

	char str[100];
    while (TRUE) {
    gets(str);
    /* These statements check if the arguments have been added after the given command. If you press enter, it will give an error.*/
    if(str[3]=='\0' && strcmp(str, "run")==0) printf("You need to give an argument to the 'run' command.\n");
    else if(str[3]=='\0' && strcmp(str, "add")==0) printf("You need to give an argument to the 'add' command.\n");
    else if(str[3]=='\0' && strcmp(str, "sub")==0) printf("You need to give an argument to the 'sub' command.\n");
    else if(str[4]=='\0' && strcmp(str, "mult")==0) printf("You need to give an argument to the 'mult' command.\n");
    else if(str[3]=='\0' && strcmp(str, "div")==0) printf("You need to give an argument to the 'div' command.\n");
    else if(str[4]=='\0' && strcmp(str, "help")==0) printf("You need to give an argument to the 'help' command.\n");
    else{
    char *token;
    token= strtok(str, " ");/*gets the first token*/
    int sum, mult, sub;
    float div;
    sum=0;
    sub=0;


        if(strcmp(token, "run")==0){

            char addre[100];
            //strcpy(addre, "C:\\Windows\\System32\\");
            //strcpy(addre, "C:\\Users\\mmaaz\\Documents\\Maaz.txt");
            // strcpy(addre, "C:\\Users\\mmaaz\\");
            token= strtok(NULL, " ");
           // printf("%s\n",token);
            strcpy(addre, token);  //concatenate the filename to address of Win32
            PROCESS_INFORMATION pi;
            STARTUPINFO si;
            ZeroMemory(&pi, sizeof(pi));
            ZeroMemory(&si, sizeof(si)); //Fills a block of memory with zeros. A pointer to the starting address of the block of memory to fill with zeros ,and the size in bytes if the block of memory to fill zeros with. Helps to clear a block of memory.
             si.cb = sizeof (si) ; /*Only compulsory field. CB is the size of the structure in bytes.
       CreateProcess(lpAppName, lpCommandLine,lpProcessAttributes,lpThreadAttributes,bInheritHandles, dwCreationFlags,lpEnvironment, lpCurrentDirectory,lpStartupInfo,lpProcessInfo)
       lpProcessAttributes is Null beacuase of which the child process does not inherit the returned handle to the new process object, and gets a default security descriptor.f
       lpThreadAttributes is Null beacuase of which the child process does not inherit the returned handle to the new thread object, and gets a default security descriptor.
       bINherit handle:If this member is FALSE, the new process DOES NOT inherit the returned handle. We cannot write more commands until the current program closes
       lpCurrentDirectory:If this parameter is NULL, the new process will have the same current drive and directory as the calling process. */
            if(CreateProcess(NULL, addre ,NULL,NULL,FALSE,0,NULL,NULL,&si,&pi))
{
                //WaitForSingleObject(ProcessInfo.hProcess,INFINITE);//does not allow the other commands/processes to run until the created process is closed and the handle is returned.
                CloseHandle(pi.hThread);
                CloseHandle(pi.hProcess);
}
            else
{               const int ques = MessageBox(NULL,"The process could not be started! Either the file name is wrong, or you don't have the rights to access the file!","Error Opening the File!", MB_ICONWARNING);
            switch(ques)
            {
            case IDOK:
               break;
            }
}
    }
        else if(strcmp(token, "add")==0){
            while(token!=NULL){
            token= strtok(NULL, " ");
            sum= sum + atoi(token);
    }
        printf("%i\n", sum);
    }
        else if (strcmp(token, "mult")==0){
            token= strtok(NULL, " "); //store the first number in token
            int tok2=1;
            mult=atoi(token); //assign it to mult
            while(token!=NULL){
                token= strtok(NULL, " "); //store the current number
                mult= mult * (tok2); //the current number gets multiplied by the next number in the string
                tok2=atoi(token); // stores the next number of the string.
    }
        printf("%i\n", mult);
    }
        else if (strcmp(token, "sub")==0){
            token= strtok(NULL, " ");
            sub=atoi(token);
            while(token!=NULL){
                token= strtok(NULL, " ");
                sub= sub - atoi(token);
    }
        printf("%i\n", sub);

    }
        else if (strcmp(token, "div")==0){
            token =strtok(NULL, " ");
            div=atoi(token);
            int tok2=1;
            div=atoi(token); //assign it to div
            while(token!=NULL){
                token= strtok(NULL, " "); //store the subsequent number in token
                div= div / (tok2); //the previous number gets multiplied by the next number in the string
                tok2=atoi(token); // stores the next number of the string.
    }
         printf("%f\n", div);
    }
        else if (strcmp(token, "help")==0){
            token=strtok(NULL, " ");
            if(strcmp(token, "add")==0) printf("The 'add' command sums the string of numbers that follows it.\n");
            else if(strcmp(token, "sub")==0) printf("The 'sub' command subtracts each subsequent number from the previous one.\n");
            else if(strcmp(token, "mult")==0) printf("The 'mult' command multiplies all the numbers.\n");
            else if(strcmp(token, "div")==0) printf("The 'div' command divides the first number by the second number.\n");
            else if(strcmp(token, "run")==0)printf("The 'run' command starts a process.\n");
            else if(strcmp(token, "exit")==0)printf("The 'exit' command terminates this program.\n");
            else MessageBox(NULL,"No information found on this command! Help available for 'add', 'sub', 'mult', 'div', 'run', and 'exit' commands only.","Invalid Help Command", MB_ICONINFORMATION);
           }
        else if(strcmp(token, "exit")==0) {
            const int ques = MessageBox(NULL, "Do you really want to exit?", "Exit", MB_YESNO | MB_ICONQUESTION);
            switch(ques)
            {
            case IDYES:
               exit(0);
            case IDNO:
                break;
            }
    }

        else if (strcmp(token, "kill")==0){
            token= strtok(NULL, " ");
            killProcessByName(token);

    }

        else {
            const int ques = MessageBox(NULL, "No such command exists! Do you want to try again?", "Invalid Command", MB_YESNO | MB_ICONERROR);
            switch(ques)
            {
                case IDYES:
                    break;
                case IDNO:
                    exit(0);
            }
    }
}
}
}






