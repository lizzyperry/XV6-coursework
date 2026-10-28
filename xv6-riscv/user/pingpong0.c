#include "kernel/types.h"
#include "user/user.h"

int main() {
  // Declare two pipes
  // p2c - a parent to a child communication
  int p2c[2];  
// c2p - a child to a parent communication
  int c2p[2];

  // Call the pipe() system call to create p2c and c2p
  if (pipe(p2c) < 0 || pipe(c2p) < 0) {
    fprintf(2, "pingpong0: pipe failed\n");
    exit(1);
  }

  // Declare one byte for sending to the child process
  char our_byte = 'P';

  // Fork a new process
  int pid = fork();
  if (pid > 0) {
    // 1. PLACE YOUR CODE HERE TO SEND A BYTE TO THE OTHER PROCESS
    close(p2c[0]);
    close(c2p[1]);
    write(p2c[1], &our_byte, 1);
// 5. PLACE YOUR CODE HERE TO RECEIVE THE MODIFIED BYTE
    read(c2p[0], &our_byte, 1);
    fprintf(1, "%d:Received pong, %c\n", getpid(), our_byte);

    close(p2c[1]);
    close(c2p[0]);
    wait(0);     
  }
  else if (pid == 0) {
    // 2. PLACE YOUR CODE HERE TO READ A BYTE IN THE PIPE p2c
    close(p2c[1]);
    close(c2p[0]);
    read(p2c[0], &our_byte, 1);
    fprintf(1, "%d:Received ping, %c\n", getpid(), our_byte);
    // 3. PLACE YOUR CODE HERE TO MODIFY THE BYTE TO LETTER 'R'
    our_byte = 'R';
    // 4. PLACE YOUR CODE HERE TO SEND THE BYTE BACK OVER c2p PIPE
    write(c2p[1], &our_byte, 1);
    close(p2c[0]);
    close(c2p[1]);
  }
  else {
    fprintf(2, "pingpong0: fork failed\n");
    exit(1);
  }
  exit(0); // Exit parent process
}
