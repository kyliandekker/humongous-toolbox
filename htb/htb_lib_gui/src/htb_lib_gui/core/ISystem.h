#pragma once

#include <atomic>
#include <condition_variable>
#include <future>
#include <mutex>
#include <string>
#include <thread>

namespace htb::core
{
	//======================================================================================
	// System
	//======================================================================================
	/// <summary>
	/// Base class for all systems within the engine, providing a common interface
	/// for initialization, destruction, and readiness checking.
	/// </summary>
	class ISystem
	{
	public:
		/// <summary>
		/// Constructs a system with a given name.
		/// </summary>
		/// <param name="a_sName">The name of the system.</param>
		ISystem(const std::string& a_sName);
		virtual ~ISystem();

		/// <summary>
		/// Initializes the system, setting up necessary resources.
		/// </summary>
		/// <returns>True if the initialization was successful, otherwise false.</returns>
		bool Initialize();

		/// <summary>
		/// Destroys the system, releasing resources and performing necessary cleanup.
		/// </summary>
		/// <returns>True if the destruction was successful, otherwise false.</returns>
		bool Destroy(bool a_bLog = true);

		/// <summary>
		/// Checks whether the system is ready for use.
		/// </summary>
		/// <returns>True if the system is ready, otherwise false.</returns>
		bool Running() const;
	protected:
		/// <summary>
		/// Called once on the thread to perform initialization steps.
		/// Must be implemented by subclasses.
		/// </summary>
		/// <returns>True if the initialization was successful, otherwise false.</returns>
		virtual bool OnInitialized();

		/// <summary>
		/// Called once the system is stopping, to release resources.
		/// Must be implemented by subclasses.
		/// </summary>
		virtual bool OnDestroyed();

		std::string m_sName = "";
		std::atomic<bool> m_bRunning{ false }; /// Flag indicating whether the system is running.
		std::atomic<bool> m_bInitialized{ false }; 
		std::atomic<bool> m_bDestroyed{ false };
	};

	//======================================================================================
	// IThreadedSystem
	//======================================================================================
	/// <summary>
	/// Base class for all threaded systems within the engine, providing a common interface
	/// for initialization, destruction, and readiness checking.
	/// </summary>
	class IThreadedSystem
	{
	public:
		IThreadedSystem(const std::string& a_sName);
		~IThreadedSystem();

		/// <summary>
		/// Initializes the system and its thread.
		/// </summary>
		/// <param name="a_Wait">If true, calling-thread waits for initialization to complete.</param>
		/// <returns>True if initialization was successful, false otherwise.</returns>
		bool Initialize(bool a_Wait);

		/// <summary>
		/// Signals the thread to stop and joins it.
		/// </summary>
		/// <returns>True if the destruction was successful, otherwise false.</returns>
		bool Destroy();

		/// <summary>
		/// Wakes up the thread if it is sleeping.
		/// </summary>
		void WakeUp();
	protected:
		/// <summary>
		/// Called once on the thread to perform initialization steps.
		/// Must be implemented by subclasses.
		/// </summary>
		/// <returns>True if the initialization was successful, otherwise false.</returns>
		virtual bool OnInitialized();

		/// <summary>
		/// Called on the thread when shutdown has been requested, before the thread exits.
		/// Can be implemented by subclasses to finish or drain pending work.
		/// </summary>
		/// <returns>True if shutdown was successful, otherwise false.</returns>
		virtual bool OnShutdown();

		/// <summary>
		/// Called once the system is stopping, to release resources.
		/// Must be implemented by subclasses.
		/// </summary>
		virtual bool OnDestroyed();

		/// <summary>
		/// Indicates whether the thread should sleep or wake to run work.
		/// Must be implemented by subclasses.
		/// </summary>
		/// <returns>True if thread should sleep, false to proceed immediately.</returns>
		virtual bool Sleep() const = 0;

		/// <summary>
		/// The main loop method run by the thread; must be implemented by subclasses.
		/// </summary>
		virtual void Loop() = 0;

		/// <summary>
		/// Thread entry point. Internal, runs init and loop.
		/// </summary>
		void ThreadEntry(std::promise<bool> a_Promise);

		std::thread m_Thread; /// The thread.

		std::mutex m_RunningMutex; /// Mutex for controlling running condition.
		std::condition_variable m_RunningCondVar; /// Condition variable for thread wakeup.

		std::string m_sName = "";
		std::atomic<bool> m_bRunning{ false }; /// Flag indicating whether the system is running.
	};
}