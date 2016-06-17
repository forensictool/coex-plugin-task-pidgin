#include <iostream>
#include "task.h"
#include "coex/coex.h"
#include "coex/config.h"
#include "coex/typeos.h"

void printHelp(QStringList &args){
    coex::ITask* pTask = (coex::ITask*)(new TaskPidgin());
	std::cout << "\n" << pTask->description().toStdString() << "\n\n";
	std::cout << "  Usage: " << args[0].toStdString() << " [OPTIONS] \n\n";
	std::cout << "  OPTIONS:\n\n";
	std::cout << "    --input-folder <fullpath>       - required parameter\n";
	std::cout << "    --output-folder <fullpath>      - required parameter\n";
	std::cout << "    --help                          - this help\n";
	std::cout << "\n";
	std::cout << "  Version: " << VERSION_MAJOR << "." << VERSION_MINOR << "." << VERSION_BUILD << "\n";
	std::cout << "  Author: " << pTask->author().toStdString() << "\n\n";
	// std::cout << "  LICENSE: " << pTask->license().toStdString() << "\n";
}

int main(int argc, char* argv[])
{
	QStringList args;
	for(int i = 0; i < argc; i++){
		args << QString(argv[i]);
	}
	
	if(!args.contains("--input-folder") || !args.contains("--output-folder")){
		printHelp(args);
		return -1;
	}

	if(args.contains("--help")){
		printHelp(args);
		return 0;
	}
	
	QString inputFolder = "";
	if(args.contains("--input-folder") && args.indexOf("--input-folder") + 1 > args.size()){
		std::cout << "\nERROR: Could not found parameter for '--input-folder'\n";
		printHelp(args);
		return -2;
	}else{
		inputFolder = args[args.indexOf("--input-folder") + 1];
	}
	
	QString outputFolder = "";
	if(args.contains("--output-folder") && args.indexOf("--output-folder") + 1 > args.size()){
		std::cout << "\nERROR: Could not found parameter for '--output-folder'\n";
		printHelp(args);
		return -3;
	}else{
		outputFolder = args[args.indexOf("--output-folder") + 1];
	}
	
	args << "--debug"; // always debug on
	
	coex::IConfig *pConfig = (coex::IConfig *)(new Config());
	
	pConfig->setInputFolder(inputFolder);
	pConfig->setOutputFolder(outputFolder);
	pConfig->setTypeOS((coex::ITypeOperationSystem *)(new TypeOS()));
	
    coex::ITask* pTask = (coex::ITask*)(new TaskPidgin());
	
	pTask->setOption(args);
	std::cout << "\n\n";
	pTask->execute(pConfig);
	return 0;
};
