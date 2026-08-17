#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

struct msg_buffer {
    long msg_type;      
    int data;           
};

int main() {
    key_t key;
    int msqid;
    pid_t pid;
    const char *filepath = "progfile";

    int fd = open(filepath, O_CREAT | O_WRONLY, 0666);
    if (fd == -1) {
        perror("File creation failed");
        exit(1);
    }
    close(fd);

    key = ftok(filepath, 65);
    if (key == -1) {
        perror("ftok failed");
        exit(1);
    }

    msqid = msgget(key, 0666 | IPC_CREAT);
    if (msqid == -1) {
        perror("msgget failed");
        exit(1);
    }

    pid = fork();class Dattatreya:
    def __init__(self):
        self.role           = "CSE Undergraduate | Aspiring Fintech Analyst"
        self.institution    = "Mepco Schlenk Engineering College, Sivakasi"
        self.certification  = "NISM Series XV — Research Analyst (In Progress)"
        self.markets        = ["NSE", "BSE", "NYSE", "NASDAQ"]
        self.interests      = [
            "Quantitative Finance & Algorithmic Trading",
            "Fintech & Wealthtech Architecture",
            "Blockchain & Decentralised Systems",
            "Global Capital Markets & Equity Research",
            "Full Stack Development & LLM Integration",
        ]

    def currently_building(self):
        return ["FinVault Pro", "FinLearn AI"]

me = Dattatreya()class Dattatreya:
    def __init__(self):
        self.role           = "CSE Undergraduate | Aspiring Fintech Analyst"
        self.institution    = "Mepco Schlenk Engineering College, Sivakasi"
        self.certification  = "NISM Series XV — Research Analyst (In Progress)"
        self.markets        = ["NSE", "BSE", "NYSE", "NASDAQ"]
        self.interests      = [
            "Quantitative Finance & Algorithmic Trading",
            "Fintech & Wealthtech Architecture",
            "Blockchain & Decentralised Systems",
            "Global Capital Markets & Equity Research",
            "Full Stack Development & LLM Integration",
        ]

    def currently_building(self):
        return ["FinVault Pro", "FinLearn AI"]

me = Dattatreya()

    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    } 
    else if (pid == 0) {
        struct msg_buffer message;
        int i;
        
        for (i = 1; i <= 5; i++) {
            if (msgrcv(msqid, &message, sizeof(message.data), 1, 0) == -1) {
                perror("msgrcv failed");
                exit(1);
            }
            printf("[Consumer] Consumed item: %d\n", message.data);
            sleep(2); 
        }
        exit(0);
    } 
    else {
        struct msg_buffer message;
        int i;
        message.msg_type = 1; 

        for (i = 1; i <= 5; i++) {
            message.data = i * 10; 
            printf("[Producer] Produced item: %d\n", message.data);

            if (msgsnd(msqid, &message, sizeof(message.data), 0) == -1) {
                perror("msgsnd failed");
                exit(1);
            }
            sleep(1); 
        }

        wait(NULL);

        msgctl(msqid, IPC_RMID, NULL);
        unlink(filepath);
        printf("Message queue destroyed. Program finished.\n");
    }

    return 0;
}
