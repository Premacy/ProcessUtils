#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <csignal>
#include <set>

std::atomic_bool _run = true;

char* terminal_commands[] = { 
"-c create new child process",
"-k kill child process by PID",
"-s show child process statys by PID",
"-exit exit program"
};

std::set<int> pids_of_processes;

struct pair_param{
  std::string key;
  std::string value;
};

void print_menu(){
  for(int i = 0; i < 4; ++i){
    std::cout << terminal_commands[i] << std::endl;
  }
}

pair_param parse_command(const std::string& command){
  pair_param p;
  std::istringstream istr(command);

  std::getline(istr, p.key, ' ');
  std::getline(istr, p.value, ' ');

  return p;
}

void child_loop(){
  while (true) {}
}

void create_child_process(){
  int pid = fork();

  if (pid == 0) { // child
    child_loop();
  } else if( pid > 0 ) {
    pids_of_processes.insert(pid);
    std::cout << "Child process sucessfully created with PID = " << pid << std::endl;
  } else {
    std::cout << "Error. Can not create child process" << std::endl;
  }
}

void print_pids(){
  for (int pid : pids_of_processes) {
    std::cout << "PID : " << pid << std::endl;
  }
}

void kill_process(int pid){
  std::cout << "Try to kill process" << std::endl;

  auto it = pids_of_processes.find(pid);
  if (it == pids_of_processes.end()) {
    std::cout << "Erorr delete PID " << pid << std::endl;
    return;
  }

  kill(pid, SIGKILL);
  pids_of_processes.erase(it);
}

void process_command(const std::string& command){
  auto p = parse_command(command);
  /*if(!p_opt.has_value()){
    std::cout << "Parsing error" << std::endl;
    return;
  }*/

  //auto p = p_opt.value();
  std::cout << p.key << std::endl;

  if (p.key == "-c") {
    create_child_process();
  } else if (p.key == "-k") {
    kill_process(std::stoi(p.value));
  } else if (p.key == "-s") {
    print_pids();
  } else if (p.key == "exit") {
    exit(0);
  }
}

void terminal_loop(){
  while (_run) {
    print_menu();
    
    std::string command;
    std::getline(std::cin, command);

    process_command(command);
  }
}

void signal_handler(int sig_num){
  std::cout << "End program" << std::endl;
  exit(0);
}

int main(int argc, char* argv[]){
  std::signal(SIGINT, signal_handler);
  terminal_loop();

  return 0;
}
