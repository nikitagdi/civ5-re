#pragma once

namespace tbb
{
    class task
    {
    public:
        virtual ~task() {}
        virtual task* execute() = 0;
        virtual void note_affinity(unsigned short) {}
    };

    namespace internal
    {
        class observer_proxy;

        class task_scheduler_observer_v3
        {
        public:
            task_scheduler_observer_v3() : my_proxy(0), my_busy_count(0) {}
            virtual ~task_scheduler_observer_v3() {}
            virtual void on_scheduler_entry(bool) {}
            virtual void on_scheduler_exit(bool) {}

        private:
            observer_proxy* my_proxy;
            int my_busy_count;
        };
    }
}
