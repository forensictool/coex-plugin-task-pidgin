# coex-plugin-task-pidgin
Collect a list of user messenger contacts.

Requirements: Qt5

## Build plugin for coex:
	
	$ ./build_lib.sh

## Build single app:

	$ ./build_app.sh
	
### run single app:

	$ ./bin/coex-pidgin --help

	Task is search logs of Pidgin

	  Usage: ./coex-pidgin [OPTIONS] 

	  OPTIONS:

	    --input-folder <fullpath>       - required parameter
	    --output-folder <fullpath>      - required parameter
	    --help                          - this help

	  Version: 0.1.0
	  Author: Igor Polyakov <fox.user.3@gmail.com>


	$ ./bin/coex-pidgin --input-folder ../ --output-folder ../output


	==========TaskPidgin::execute==========

	 * Debug mode: On
	 * InputFolder:  "../"
	 * OutputFolder:  "../output" 

	==========TaskPidgin::finish==========


