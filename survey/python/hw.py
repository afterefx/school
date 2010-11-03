#Christopher Carlisle
#Python homework
#Survey of Programming Languages
#11/03/10

class Stack(object):
    ''' This is a stack class '''

    #Constructor
    def __init__(self):
        self.data = []

    #Push - pushes a value onto the stack
    def push(self, value):
        self.data.append(value)

    #Pop - removes and returns a value from the top of the stack
    def pop(self):
        return self.data.pop()

    #IsEmpty - checks if the stack is empty
    def isEmpty(self):
        return 0 == len(self.data)

    #Top - returns the value of at the top of the stack
    def top(self):
        return self.data[-1]

    #Count - returns the number of items on the stack
    def count(self):
        return len(self.data)

    #Display - prints each item in the stack on it's own line
    def display(self):
        for x in self.data:
            print x

class Queue(object):
    ''' A queue class '''

    #Constructor
    def __init__(self):
        self.data = []

    #Enqueue - inserts a value at the beginning of the queue
    def enqueue(self, value):
        self.data.insert(0,value)

    #Dequeue - removes and returns the next value in the queue
    def dequeue(self):
        return self.data.pop()

    #Front - returns the value at the front of the queue
    def front(self):
        return self.data[-1]

    #IsEmpty - checks to see if queue is empty
    def isEmpty(self):
        return 0 == len(self.data)

    #Count - returns the number of items in the queue
    def count(self):
        return len(self.data)

    #Display - prints each item in the queue on it's own line
    def display(self):
        for x in self.data:
            print x

