#include<iostream>
#include<cstdlib>
#include<queue>
using namespace std;
enum Operator
{
	kAdd,
	kSub,
	kFact
};
typedef struct
{
	int op1;
	int op2;
	Operator operation;
}Task; // TODO: typedef this struct

typedef struct
{
	pthread_mutex_t* slock;
	std::queue<Task*> *sTaskList;
}ThreadData;

void CreateTasks(int totalNoOfTasks, std::queue<Task*> *taskList)
{
	for (int i = 0; i < totalNoOfTasks ; ++i)
	{
		cout << "creating task " << i << endl; 
		Task *task = new Task();
		task->op1 = rand() % 100;
		task->op2 = rand() % 100;
		task->operation = static_cast<Operator>(rand() % 3);
		taskList->push(task);
	}
}

void* ExecuteTask(void* p)
{
	ThreadData *threadData = static_cast<ThreadData*>(p);
	pthread_mutex_t *lock = threadData->slock;
	std::queue<Task*> *taskList = threadData->sTaskList;
	Task *newTask = NULL;
	cout << "entering ExecuteTask" << endl;
	cout << "lock obj address" << lock << endl;
	cout << "total tasks " << taskList->size();
	while (true)
	{
		pthread_mutex_lock(lock);
		if (!taskList->empty())
		{
			newTask = taskList->front();
			taskList->pop();
		}
		else
		{
			pthread_mutex_unlock(lock);
			break;
		}
		pthread_mutex_unlock(lock);
		switch (newTask->operation)
		{
			case kAdd: cout << newTask->op1 + newTask->op2 << endl;
				   break;
			case kSub: cout << newTask->op1 - newTask->op2 << endl;
				   break;
			default:
				   cout << "Sorry Chethan!" << endl;
				   break;
		}
	}
}

int main(int argc, char *argv[])
{
	std::queue<Task*> *TaskList = new std::queue<Task*>;
	
	// 1. Create Tasks
	// 2. Pool/Queue the tasks
	CreateTasks(100,TaskList);
	
	// 3. Create Worker Thread Pool
	const int numThreads = 4;
	pthread_t ThreadId[numThreads];
	pthread_mutex_t lock;
	pthread_mutex_init(&lock,NULL);
	ThreadData threadData;
	threadData.lock = &lock;
	threadData.TaskList = TaskList;
	
	// 4. Execute the tasks
	for (int i = 0; i < numThreads ; ++i)
	{
		cout << "creating thread "<< i << endl;
		pthread_create(&ThreadId[i], NULL, ExecuteTask, &threadData);
	}
	
	for (int i = 0 ; i < numThreads ; ++i)
	{
		pthread_join(ThreadId[i], NULL);
	}

	// 5. Release the worker thread pool


	// 6. exit the program
}
