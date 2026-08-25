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

std::istringstream string_to_stream(std::string &value){
  std::istringstream stream(value);

  return stream;
}

enum States{
  NORMAL_STATE,
  DOUBLE_QUOTED,
  SINGLE_QUOTED
};




std::vector<std::string> split_string(std::string &value){
  std::vector<std::string>result;
  std::string token;
  States state = NORMAL_STATE;
  for(int i =0;i<command.size();i++){
    switch(state){
        case NORMAL_STATE:
          if(command[i]==' '){
            if (!token.empty()) {
                    result.push_back(token);
                    token.clear();
                }
          }else if(command[i]=='\\'){
            if(command[i+1]){
              token.push_back(command[i+1]);
              i++;
            }
          }
          else if(command[i]=='"'){
            state=DOUBLE_QUOTED;
          }else if(command[i]=='\''){
            state=SINGLE_QUOTED;
          }
          else{
            token.push_back(command[i]);
          }
          break;
        case DOUBLE_QUOTED:
          if(command[i]=='"'){
            state=NORMAL_STATE;
          }
          else if(command[i]=='\\' && command[i+1] && command[i+1]=='\\' || command[i+1]=='"'){
              token.push_back(command[i+1]);
              i++;
          }else{
            token.push_back(command[i]);          
          }
          break;
        case SINGLE_QUOTED:
          if(command[i]=='\''){
            state=NORMAL_STATE;
          }else{
            token.push_back(command[i]);          
          }
          break;
        default:
          continue;
        } 
  }
  if (!token.empty()){
    result.push_back(token);
    token.clear();
  }

  return result;
}

std::string find_path(){
  std::string system_path = std::getenv("PATH");
    std::istringstream path_stream(system_path);
    std::string temp;
    std::string filepath;
    std::string cmd;
    std::istringstream cmd_stream(command);
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
        std::string temp2 = find_path();
        char* path_value = temp2.data();
        std::vector<char*>second;

     std::vector<std::string>first = split_string(command);
     for(int i=0;i<first.size();i++){
        second.push_back(first[i].data());
      }
      second.push_back(nullptr);
      execvp(path_value,second.data());
      perror("execvp");
    }
      else if(process>0){
        waitpid(process,nullptr,0);
      }else{
        std::cerr << "Fork failed" << std::endl;
      }
    }else{
      std::cout<<"command is not executable"<<std::endl;
    }
}

  void Echo_Command(){
      
      std::vector<std::string>result = split_string(command);

      for(int i=1;i<result.size();i++){
        std::cout<<result[i]<<" ";
      }
      std::cout<<std::endl;
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
