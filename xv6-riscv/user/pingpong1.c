#include "kernel/types.h"
#include "user/user.h"

int main() {
  //single pipe for both directions
  int p[2];

  //create pipe before forking so child shares it
  if (pipe(p) < 0) {
    fprintf(2, "pingpong1: pipe failed\n");
    exit(1);
  }

  //declare one byte to send to child process
  char our_byte = 'P';

  //fork new process
  int pid = fork();
  if (pid > 0) {
    // parent sends 'P' to child
    write(p[1], &our_byte, 1);

    //wait for child to read 'P', write 'R' and exit.
    //reminder to myself without this, the parent could read its own 'P' back.
    wait(0);

    // only child's 'R' can be in the pipe now
    read(p[0], &our_byte, 1);
    fprintf(1, "%d:Received pong, %c\n", getpid(), our_byte);

    close(p[0]);
    close(p[1]);
  }
  else if (pid == 0) {
    // child reads 'P' (blocks until the parent has written it)
    read(p[0], &our_byte, 1);
    fprintf(1, "%d:Received ping, %c\n", getpid(), our_byte);

    //change byte to 'R' and sends it back over the same pipe
    our_byte = 'R';
    write(p[1], &our_byte, 1);

    close(p[0]);
    close(p[1]);
  }
  else {
    fprintf(2, "pingpong1: fork failed\n");
    exit(1);
  }
  exit(0);
}
