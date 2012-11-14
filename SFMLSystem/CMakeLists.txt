set(SFML_SYS_SRC
    Clock.cpp
    Err.cpp
    Lock.cpp
    Mutex.cpp
    String.cpp
    Thread.cpp
    ThreadLocal.cpp
    Time.cpp
    Unix/ClockImpl.cpp
    Unix/MutexImpl.cpp
    Unix/SleepImpl.cpp
    Unix/ThreadImpl.cpp
    Unix/ThreadLocalImpl.cpp
)

add_library(SFMLSystem ${SFML_SYS_SRC})
