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
  void Echo_Command(){
    std::cout<<command.substr(5)<<std::endl;
  }

  void not_builtin(){
    std::string system_path = std::getenv("PATH");
    // if (!system_path) {
    // std::cout << "PATH not set\n";
    // return;
    // }
    std::istringstream path_stream(system_path);
      std::vector<std::string>paths_split;
      std::string temp;

      bool found = false;
      while(std::getline(path_stream, temp, ':')){
	std::string filepath = temp + "/" + command.substr(5);
  // std::cout<<filepath<<std::endl;
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
      }else{
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
