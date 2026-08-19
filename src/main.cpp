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


std::vector<std::string> split_string(std::string &value ){
  std::istringstream cmd_stream = string_to_stream(value);
  std::vector<std::string>result;
  std::string word;

  while(cmd_stream >> word){
    result.push_back(word);
  }
  return result;
}


std::vector<std::string> split_string_special_char(std::string &value){
  std::istringstream cmd_stream = string_to_stream(value);
  std::vector<std::string>result;
  std::string temp;
  cmd_stream >> temp;
  result.push_back(temp);
  int count = 0;
  if(value.find('\"') != -1 && value.find('\'') != -1){
    while(std::getline(cmd_stream,temp,'"')){
    if(count > 0 && temp!=" "){
      result.push_back(temp);
    }
    count++;
   }
  }
  else if(value.find('\"') != -1){
    while(std::getline(cmd_stream,temp,'"')){
    if(count > 0 && temp!=" "){
      result.push_back(temp);
    }
    count++;
   }
  }
  else if(value.find('\'') != -1){
    while(std::getline(cmd_stream,temp,'\'')){
    if(count > 0 && temp!=" "){
      result.push_back(temp);
    }
    count++;
   }
  }else{
    split_string(value);
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

      if(command.find('\'') != -1 || command.find('"') != -1){
      std::vector<std::string>first = split_string_special_char(command);
      for(int i=0;i<first.size();i++){
        second.push_back(first[i].data());
      }
      second.push_back(nullptr);
      }
      else{
        std::string cmd = command.substr(0);
        std::istringstream cmd_stream(cmd);
        std::vector<std::string>first;
        std::string temp;
        while(std::getline(cmd_stream,temp,' ')){
         first.push_back(temp);
        }
        for(int i=0;i<first.size();i++){
          second.push_back(first[i].data());
        }
      second.push_back(nullptr);
      }
      
      execvp(path_value,second.data());
      perror("execvp");

      }else if(process>0){
        waitpid(process,nullptr,0);
      }else{
        std::cerr << "Fork failed" << std::endl;
      }
    }

  }


  void Echo_Command(){

  if(command.find('"')!= -1){
  std::istringstream cmd_stream = string_to_stream(command);
    std::string temp;
    int count=0;
    while(std::getline(cmd_stream,temp,'"')){
      // std::cout<<temp.find_first_not_of(' ')<<std::endl;
    if(count>=1 && !temp.find_first_not_of(' ')){
      auto pos = temp.find_first_not_of(' ');
  std::cout << ' ';

  if (pos != std::string::npos) {
      std::cout << temp.substr(pos);
  }
    }else if(count>=1 && temp!=""){
      std::cout<<" ";
    }else{
      std::cout<<"";
    }
    count++;
  }
  std::cout<<std::endl;
}
else if(command.find('\'')!= -1){
    std::istringstream cmd_stream = string_to_stream(command);
    std::string temp;
    int count=0;
    while(std::getline(cmd_stream,temp,'\'')){
    if(count>=1){
      std::cout<<temp;
    }
    count++;
  }
  std::cout<<std::endl;
}
else{
    std::vector<std::string>parsed_string = split_string(command);
    for(int i=1;i<parsed_string.size();i++){
      std::cout<<parsed_string[i];
      if(i<parsed_string.size()-1){
        std::cout<<" ";
      }
    }
    std::cout<<std::endl;
 }
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
