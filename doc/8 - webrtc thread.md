# Processes and Threads #

An application consists of one or more processes. 

A **process**, in the simplest terms, is an executing program. One or more threads run in the context of the process. 

A **thread** is the basic unit to which the operating system allocates processor time. A thread can execute any part of the process code, including parts currently being executed by another thread.

## Create a thread in MAC / Linux ##

The ```pthread_create()``` function starts a new thread in the calling process. 

The new thread starts execution by invoking ```start_routine()```; ```arg``` is passed as the sole argument of ```start_routine()```.

The ```attr``` argument points to a ```pthread_attr_t``` structure whose contents are used at thread creation time to determine attributes for the new thread.

```c
#include <pthread.h>

int pthread_create(
  pthread_t *thread, 
  const pthread_attr_t *attr,
  void *(*start_routine) (void *), 
  void *arg
);
```

## Create a thread in Windows ##
The ```CreateThread``` function creates a new thread for a process. The creating thread must specify the starting address of the code that the new thread is to execute. 

```c++
#include <windows.h>

HANDLE CreateThread(
  [in, optional]  LPSECURITY_ATTRIBUTES   lpThreadAttributes,
  [in]            SIZE_T                  dwStackSize,
  [in]            LPTHREAD_START_ROUTINE  lpStartAddress,
  [in, optional]  __drv_aliasesMem LPVOID lpParameter,
  [in]            DWORD                   dwCreationFlags,
  [out, optional] LPDWORD                 lpThreadId
);
```


# WebRTC thread #

WebRTC threads designed to handle WebRTC's internal tasks and operations efficiently, which can be categorized into three types:

1. **Network Thread**: handles the transmission and reception of network data, ensuring that packets are sent and received promptly.

2. **Worker Thread**: performs tasks such as encoding, decoding, and other media processing operations, which require significant computational resources.

3. **Signaling Thread**: manages the signaling operations, including session negotiation and ICE (Interactive Connectivity Establishment). It is essential not to block this thread inside callbacks, as doing so will block the entire signaling process.

Key characteristics of a WebRTC thread:

1. **Event-Driven**: WebRTC threads operate based on an event-driven model. Events are tasks or messages that are queued for execution. The thread processes these events sequentially, ensuring operations are handled without race conditions.

2. **Internal Queue**: Each WebRTC thread has an internal queue that stores events. These events can be added from different parts of the program, and the thread processes them in the order they were added (FIFO - First In, First Out).

3. **Concurrency Management**: Using dedicated threads with their own event queues helps manage concurrency effectively, avoiding issues like data races and deadlocks, which are common in multi-threaded environments.

## Internal design ##

A WebRTC thread is an instance of the ```rtc::Thread``` class in the WebRTC library.
```c++
// rtc_base/thread.h
namespace rtc {
class RTC_EXPORT ThreadManager{}
class RTC_LOCKABLE RTC_EXPORT Thread : public webrtc::TaskQueueBase{}
}
```

Important members in ```rtc::Thread```

```c++
class Thread{
 public:
  // Starts the execution of the thread.
  bool Start();

  // Tells the thread to stop
  // Never call Stop on the current thread.
  virtual void Stop();

  // By default, Thread::Run() calls ProcessMessages(kForever).  To do other
  // work, override Run().  To receive and dispatch messages, call
  // ProcessMessages occasionally.
  virtual void Run();

 private:
  // get, peek - control task queue
  absl::AnyInvocable<void() &&> Get(int cmsWait);

  // task queue
  std::queue<absl::AnyInvocable<void() &&>> messages_ RTC_GUARDED_BY(mutex_);
  std::priority_queue<DelayedMessage> delayed_messages_ RTC_GUARDED_BY(mutex_);

  // used for event handler
  SocketServer* const ss_;

  // real thread used in OS
  HANDLE thread_ = nullptr;
}
```

Important members in ```rtc::ThreadManager```

```c++
class ThreadManager{
  // singleton mode
  ThreadManager* instance();
  // store thread objects
  vector<Thread*> message_queues_;
  // mutex
  CriticalSection crit_
  
  const DWORD key_;
}
```

Threads in peerconnection client

```c++
// examples/peerconnection/client/main.cc
int wWinMain(){
  rtc::PhysicalSocketServer ss;
  rtc::AutoSocketServerThread main_thread(&ss);
}

// examples/peerconnection/client/conductor.cc

// create webrtc signaling thread
bool Conductor::InitializePeerConnection(){
  if (!signaling_thread_.get()) {
    signaling_thread_ = rtc::Thread::CreateWithSocketServer();
    signaling_thread_->Start();
  }

// create peer_connection factory
  peer_connection_factory_ = webrtc::CreatePeerConnectionFactory(
    nullptr /* network_thread */, nullptr /* worker_thread */,
    signaling_thread_.get(), nullptr /* default_adm */,
    webrtc::CreateBuiltinAudioEncoderFactory(),
    webrtc::CreateBuiltinAudioDecoderFactory(),
    std::make_unique<webrtc::VideoEncoderFactoryTemplate<
        webrtc::LibvpxVp8EncoderTemplateAdapter,
        webrtc::LibvpxVp9EncoderTemplateAdapter,
        webrtc::OpenH264EncoderTemplateAdapter,
        webrtc::LibaomAv1EncoderTemplateAdapter>>(),
    std::make_unique<webrtc::VideoDecoderFactoryTemplate<
        webrtc::LibvpxVp8DecoderTemplateAdapter,
        webrtc::LibvpxVp9DecoderTemplateAdapter,
        webrtc::OpenH264DecoderTemplateAdapter,
        webrtc::Dav1dDecoderTemplateAdapter>>(),
    nullptr /* audio_mixer */, nullptr /* audio_processing */);
}
```