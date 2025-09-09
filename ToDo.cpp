#include <stdio.h>
#include <vector>
#include <string>

struct Task{
	std::string taskName;
	std::string deadLine;
	bool finished;

};

std::vector<Task> tasks;

void addTask(std::string taskName, std::string deadLine){
	struct Task newTask;
	newTask.taskName = taskName;
	newTask.finished = false;
	newTask.deadLine = deadLine;
	tasks.push_back(newTask);	
}

void viewTasks(){
	
	for(auto t : tasks){
		printf("Task name: %s, the deadline: %s , finished status: %d ", t.taskName.c_str(), t.deadLine.c_str(), t.finished);
	}
}

bool chnage_deadline_task(std::string task_name, std::string dead_line){
	for (auto t : tasks){
		if (t.taskName == task_name){
			t.deadLine = dead_line;
			return true;
		}
	}
	return false;
}

bool mark_task_as_completed(std::string task_name){
	for (auto t : tasks){
		if(t.taskName == task_name){
			t.finished = true;
			return true;
		}
	}
	return false;
}

bool deleteTask(std::string task_name){
	for(int i = 0; i < tasks.size(); ++i){
		if(tasks[i].taskName == task_name){
			tasks.erase(tasks.begin() + i);
			return true;
		}
	}
	return false;
}

void clearAllTasks(){
	tasks.clear();
}

struct Task findTask(std::string task_name){
	for(auto t : tasks){
		if(t.taskName == task_name){
			return t;
		}
	}

	struct Task t3;
	return t3;
}

int main(){

	printf("Hello from vim !");
	addTask("Play fotball", "23 May 2025");
	addTask("Play basketball", "23 May 2025");
	addTask("read a bokk", "today");
	viewTasks();

	return 0;

}
