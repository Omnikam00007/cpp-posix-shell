#include <iostream>
#include <algorithm>
#include <unistd.h>
#include <string>
#include <cstdlib>
#include <vector>
#include <sstream>
#include <sys/wait.h>
class Shell{
public:
  std::string command;

std::string find_path(){
  std::string system_path = std::getenv("PATH");
    std::istringstream path_stream(system_path);
    std::string temp;
    std::string filepath;
    std::string cmd;
    std::string cmd_stream(command);
    std::getline(cmd_stream,cmd,' ');
    bool found=false;
    while(std::getline(path_stream,temp,':')){
       filepath = temp + "/" + cmd;
      if(access(filepath.c_str(), X_OK) == 0){
        found = true;
        break;
      }
    }
    return filepath;
}


bool executable(){
  std::string system_path = std::getenv("PATH");
    std::istringstream path_stream(system_path);
    std::istringstream cmd_stream(command);
    std::string temp;
    std::string cmd;
    std::getline(cmd_stream,cmd,' ');
    bool found=false;
    while(std::getline(path_stream,temp,':')){
      std::string filepath = temp + "/" + cmd;
      if(access(filepath.c_str(), X_OK) == 0){
        found = true;
        break;
      }
    }
    return found;
}



  void external_cmd(){
    if(executable()){
      pid_t process = fork();

      if(process==0){
        char* path_value = find_path().data();
      std::string cmd = command.substr(0);
      std::istringstream cmd_stream(cmd);
      std::vector<std::string>first;
      std::vector<char*>second;
      std::string temp;
      while(std::getline(cmd_stream,temp,' ')){
         first.push_back(temp);
      }
      for(int i=0;i<first.size();i++){
        second.push_back(first[i].data());
      }
      second.push_back(nullptr);
      // std::cout<<"Program was passed "<<second.size()-1<<" args (including program name)."<<std::endl;
      execvp(path_value,second.data());

      }else if(process>0){
        waitpid(process,nullptr,0);
      }else{
        std::cerr << "Fork failed" << std::endl;
      }
    }

  }


  void Echo_Command(){
    std::cout<<command.substr(5)<<std::endl;
  }

  void not_builtin(){
    std::string system_path = std::getenv("PATH");
    std::istringstream path_stream(system_path);
      std::string temp;

      bool found = false;
      while(std::getline(path_stream, temp, ':')){
  std::string filepath = temp + "/" + command.substr(5);
  if(access(filepath.c_str(), X_OK) == 0){
    std::cout << command.substr(5)<<" is "<<filepath<<std::endl;        
    found = true;
    break;
  }
}
  if(!found){
    std::cout<<command.substr(5)<<": not found"<<std::endl;
}
  }

  void Type_command(){
    if(command.substr(5)=="exit" || command.substr(5)=="type" || command.substr(5)=="echo"){
        std::cout<<command.substr(5)<<" is a shell builtin"<<std::endl;
      }
    else{
      not_builtin();
    }
  }

  void myCommand(){
    while(true){
      std::cout<<"$ ";
      getline(std::cin,command);
      if(command=="exit"){
        break;
      }
      else if(command.substr(0,5)=="echo "){
        Echo_Command();
      }
      else if(command.substr(0,5)=="type "){
        Type_command();
      }else if(executable()){
          external_cmd();
      }
      else{
        std::cout<<command<<": not found\n";
          continue;
      }
    }
  }

};

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  // TODO: Uncomment the code below to pass the first stage
  Shell* myObject = new Shell();
  myObject->myCommand();
  delete myObject;

  return 0;
}
