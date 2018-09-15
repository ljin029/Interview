typedef uint16_t TickType_t;
// Multiple tasks
enum CtrlTaskType
{
    E_TASK1,
    E_TASK2,
    E_TASK3,
    E_TASK_MAX_NUMBER
}

// Elements in priority_queue
struct CtrlTaskElement
{
    CtrlTaskType type;
    Time tm;
}
// Callback function descriptor
struct CtrlTaskCallbackDesc
{
    TaskCallbackFunc cbFunc;
    void* thisObj;
}
struct CtrlTaskNode
{
    Time tm;
    CtrlTaskCallbackDesc desc;
}
enum SysStatus
{
    SUCCESS,
    FAILURE
}
enum StateMode
{
    IDLE,
    RUNNING,
    SUSPEND
}
struct Node
{
    CtrlTaskNode node;
    Node *next;
};

class Queue {
public:
    Queue()
    ~Queue();
    bool isEmpty() {return (m_size == 0)} const;
    bool pop(Node& var);
    Node* front() { return head;}
    void insert(CtrlTaskNode& item);
    int size() {return m_size;}
private:
    int m_size;
    Node* head;
    mutex m_mutex;
}

Queue::Queue() : 
    m_size(0),
    head(NULL)
{
}
Queue::~Queue()
{
    Node* temp = head;
    while(temp)
    {
        head = head->next;        
        delete(temp);
        temp = head;
    }
}

bool pop(Node& var)
{
    if(isEmpty()) return false;
    m_mutex(lock);
    Node* temp = head;
    var = *head;
    head = head->next;
    --m_size;
    m_mutex(unlock);
    return true;
}

void insert(CtrlTaskElement item)
{
    Node* temp, *prev, *current;
    Node* newNode = new Node;
    memset(newNode->item, item, sizeof(Node));
    newNode->next = NULL;
    m_mutex(lock);
    if(!head) head = newNode;
    else
    {
        prev = NULL;
        current = head;
        while(current && current->tm<=item->tm)
        {
            prev = current;
            current = current->next;
        }
        if(!current)
        {
            prev->next = newNode;
        }
        else
        {
            if(prev) //mid one
            {
                newNode->next = prev->next;
                prev->next = newNode;
            }
            else //first one
            {
                newNode->next = head;
                head = newNode;
            }
        }
    }
    ++m_size;
    m_mutex(unlock);
}
typedef void (*TaskCallbackFunc)(void *thisObj, CtrlTaskType type, const CtrlTaskElement &item)
class TaskManager {
public:
    TaskManager();
    SysStatus regCallback(CtrlTaskElement& item, TaskCallbackFunc cbFunc, void* thisObj);
    SysStatus nextCall();
    Time getNextTime()
    {
        Node* next = m_CtrlTaskQueue.front();
        return next->tm;
    }
    void setMode(StateMode m) {mode = m;}
private:
    Queue m_CtrlTaskQueue;
    StateMode m_mode;
    Time m_period;
}

TaskManager:TaskManager() : m_mode(IDLE)
{
}
SysStatus TaskManager::regCallback(CtrlTaskElement& item, TaskCallbackFunc cbFunc, void* thisObj)
{
    if(NULL == cbFunc || item.type >= E_TASK_MAX_NUMBER)
    {
        return FAILURE;
    }
    
    CtrlTaskNode newNode = new CtrlTaskNode;
    memcpy(newNode->item, item, sizeof(CtrlTaskElement));
    newNode->desc->cbFunc = cbFunc;
    newNode->desc->thisObj = thisObj;
    m_CtrlTaskQueue.insert(newNode);
    return SUCCESS;
}

SysStatus TaskManager::callNext()
{
    CtrlTaskNode* task;
    Time current = getTime();
    if((current - getNextTime())>5)
    {
        m_period = current - getNextTime();
        return FAILURE;
    }
    if(IDLE == m_mode && !m_CtrlTaskQueue.isEmpty())
    {
        m_CtrlTaskQueue.pop(task);
    }
    else
    {
        return FAILURE;
    }
    if (task->item.type >= E_TASK_MAX_NUMBER)
    {
        assert(false);
        return FAILURE;
    }
    if(NULL != task->desc->cbFun)
    {
        desc->cbFunc(task->desc->thisObj, task->item.type, task->item);
    }
    if(task) delete(task);
    if(!m_CtrlTaskQueue.isEmpty())
    {
        m_period = getTime() - getNextTime();
    }
    return SUCCESS;
}

void TaskManager::task()
{
    for(;;)
    {
        if(SUCCESS == callNext())
        {
        }
        delay(m_period);
    }

}