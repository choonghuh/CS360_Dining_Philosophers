CS360_Dining_Philosophers
Choong Huh
Washington State University - Vancouver

The attached C program simulates the Dining Philosophers problem using 5 POSIX threads and 5 mutex "chopsticks". Deadlock is prevented by making the philosophers drop its chopstick and wait for a random amount of time if the other chopstick cannot be aquired.
