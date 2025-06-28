// the job system only works if jobs given to threads do not share memory
// with each other (they can share read-only memory if you're sure it won't
// cause a race condition); so when creating a job per thread, you need to
// give each job separate data you chunked out of some total data that you use
// for some operation you're trying to do

// this is where all threads will run, and this is the only place where
// you need to do synchronization, which you do on shared memory and in
// this case it's just the variables used for the job system by all threads,
// and you can achieve this easily just with atomic instructions

// the thread(s) that submits a job to the job queue will call this;
// mainly using the main thread to submit jobs so this synchronization is
// maybe a bit excessive, but anyway not a bad idea to have it
void submit_job(Job job)
{
    Job_queue* queue = JOB_QUEUE;

    while (true)
    {
        s32 write = ATOMIC_LOAD(&queue->write);
        s32 read  = ATOMIC_LOAD(&queue->read);

        s32 new_write = fast_mod(write + 1, NUM_JOBS);

        if (new_write == read) // queue is full
        {
            HINT_SPIN_LOOP();
            YIELD();
            continue;
        }

        // if num of threads that pass this point is bigger than 2
        // then in this compare and exchange one thread will set the new
        // value for write atomically, and then all other threads will
        // compare their own value with the current value and not be able to
        // pass the if statement, which makes sure we don't overflow the queue
        // with more jobs than it can handle

        s32 old_write = ATOMIC_COMPARE_AND_SWAP(&queue->write, write, new_write);
        if (old_write == write) // if comparison was true, and so swapping occured
        {
            queue->jobs[write] = job;
            ATOMIC_FETCH_AND_INCREMENT(&queue->available_jobs);
            break;
        }
    }
}

// this is what all the threads will run all the time
void worker_proc(void* data = 0)
{
    Job_queue* queue = JOB_QUEUE;

    while (true)
    {
        while (ATOMIC_LOAD(&queue->available_jobs) == 0)
        {
            HINT_SPIN_LOOP();
            YIELD();
        }

        while (true)
        {
            s32 read  = ATOMIC_LOAD(&queue->read);
            s32 write = ATOMIC_LOAD(&queue->write);

            // no jobs so go back to loop and wait for one
            if (read == write) break;

            s32 new_read = fast_mod(read + 1, NUM_JOBS);

            // if more than 1 thread passes this point, then only one will
            // atomically set the value of read to the new value, and then every
            // other thread will compare that value with their own local read
            // variable value and see that it's not the same, and will fail
            // the if statement, which ensures each thread does a different job

            s32 old_read = ATOMIC_COMPARE_AND_SWAP(&queue->read, read, new_read);
            if (old_read == read) // if comparison was true, and so swapping occured
            {
                Job job = queue->jobs[read];
                job.proc(job.data);
                ATOMIC_FETCH_AND_DECREMENT(&queue->available_jobs);
                break;
            }
        }
    }
}

inline void wait_for_all_jobs()
{
    Job_queue* queue = JOB_QUEUE;
    
    while (ATOMIC_LOAD(&queue->available_jobs) > 0)
    {
        HINT_SPIN_LOOP();
        YIELD();
    }
}

// @doc returns -1 for main thread
inline s32 get_thread_index()
{
    Engine_state* engine_state = ENGINE_STATE;
    s32 id = GET_CURRENT_THREAD_ID();

    for (s32 i = 0; i < NUM_WORKERS; i++)
    {
        if (engine_state->index_to_thread_id[i] == id) return i;
    }

    return -1;
}
