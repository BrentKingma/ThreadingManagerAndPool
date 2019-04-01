// ThreadingManager.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <thread>
#include "ThreadPool.h"
#include "ThreadManager.h"
#include <string>

int runThread = true;
class ThreadFunctions
{
public:

	void Thread1()
	{
		while (runThread)
		{
			std::cout << "Thread 1 Running" << std::endl;
		}
	}

	void Thread2()
	{
		while (runThread)
		{
			std::cout << "Thread 2 Running" << std::endl;
		}
	}

	

	float Thread4()
	{
		return 10.0f;
	}
};
int Thread3()
{
	return 5;
}
void ThreadManagerExample()
{
	//Creates the thread manager
	MyThreading::ThreadManager threadManager = MyThreading::ThreadManager();
	//Creates an instance of the class that has the functions
	ThreadFunctions threadFunctions;
	//Creates the 2 threads, using the function name and the refernce to the instance that contains that function
	std::thread thread1 = std::thread(&ThreadFunctions::Thread1, threadFunctions);
	std::thread thread2 = std::thread(&ThreadFunctions::Thread2, threadFunctions);
	//Passes them on to the ThreadManager
	threadManager.AddThread("Thread 1", std::move(thread1));
	threadManager.AddThread("Thread 2", std::move(thread2));
	//Gets the number of threads currently in the manager
	std::cout << threadManager.GetNumThreads() << std::endl;

	system("pause");

	//Creates temp thread memory address
	std::thread returnedThread;
	//Gets the thread back from the manager
	threadManager.ReturnThread("Thread 2", returnedThread);

	system("pause");

	//Calls the closer of all threads currently running
	runThread = false;
	//Tells the thread manager to join the thread
	threadManager.KillThread(std::string("Thread 1"));
	//Joins the returned thread
	returnedThread.join();

	system("pause");
}
void ThreadPoolExample()
{
	//Initializes the pool with 5 worker threads
	MyThreading::ThreadPool::init(5);

	ThreadFunctions threadFunctions;

	system("pause");
	//Sends the global function to the threading pool
	auto t1 = MyThreading::ThreadPool::enqueue(Thread3);
	//Creates lamda function and gives it to the threading pool
	auto t2 = MyThreading::ThreadPool::enqueue([] { return 3; });
	//Using bind it creates a call to the referenced function that is within the provided argument
	auto t3 = MyThreading::ThreadPool::enqueue(std::bind(&ThreadFunctions::Thread4, threadFunctions));

	system("pause");

	std::cout << (t1.get() + t2.get()) << std::endl;

	system("pause");

	MyThreading::ThreadPool::kill();
		
}

int main()
{
    std::cout << "Hello World!\n"; 
	//ThreadManagerExample();
	//ThreadPoolExample();

}