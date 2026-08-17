#include <iostream>
#include <algorithm>
#include <unistd.h>
#include <string>
#include <cstdlib>
#include <vector>
#include <sstream>
#include <filesystem>
class Shell{
public:
  std::string command;

string find_path(){
  std::string system_path = std::getenv("PATH");
    std::istringstream path_stream(system_path);
    std::string temp;
    std::string filepath;

    bool found=false;
    while(getline(path_stream,temp,":")){
       filepath = temp + "/" + command.substr(5);
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
    std::string temp;

    bool found=false;
    while(getline(path_stream,temp,":")){
      std::string filepath = temp + "/" + command.substr(5);
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
      string path_value = find_path();
      string cmd = command.substr(0);
      istringstream cmd_stream(cmd);
      vector<string>first;
      while(getline(cmd_stream,temp," ")){
         first.push_back(temp);
      }

      int s_first = first.size();
      char* second[s_first];
      for(int i =0;i<s_first;i++){
        char* temp1 = s_first.c_str();
        second[i] temp1;
      }

      execvp(path_value,second);

      waitpid(pid_t);
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
